#include <thread>

#include <boost/asio/io_service.hpp>

#include <amqpcpp.h>
#include <amqpcpp/libboostasio.h>

/// Распределение очереди между слушателями

int main() {

    constexpr auto exchange_name = "ex_hello2";
    constexpr auto queue_name = "hello";
    constexpr auto routing_key = "";

    auto run = [=] (auto func, auto name) {
        boost::asio::io_service service(1);

        AMQP::LibBoostAsioHandler handler(service);

        AMQP::TcpConnection connection(&handler, AMQP::Address(
                "localhost", 5672, AMQP::Login("guest", "guest"), "/"
        ));

        AMQP::TcpChannel channel(&connection);

        channel.onError([&connection, name] (auto message) {
            std::cout << name << " Error: " << message << std::endl;
            connection.close();
        });

        channel.declareExchange(exchange_name, AMQP::fanout, AMQP::ifunused | AMQP::autodelete);
        channel.declareQueue(queue_name, AMQP::ifunused | AMQP::autodelete | AMQP::durable);
        channel.bindQueue(exchange_name, queue_name, routing_key);

        func(connection, channel, name);

        service.run();
    };

    auto consume = [=] (auto & connection, auto & channel, auto name, auto num) {
        channel.consume(queue_name)
                .onReceived([&connection, &channel, name, num](const AMQP::Message & message, uint64_t deliveryTag, bool redelivered) {
                    std::string_view string(message.body(), message.bodySize());
                    std::cout << name << " message received: " << string << " " << deliveryTag << std::endl;
                    if (num != deliveryTag) {
                        channel.reject(deliveryTag, AMQP::requeue);
                    } else {
                        channel.ack(deliveryTag);
                        connection.close();
                    }
                });
    };

    auto publish = [=] (auto & connection, auto & channel, auto name) {
        channel.publish(exchange_name, routing_key, "1_message");
        channel.publish(exchange_name, routing_key, "2_message");
        connection.close();
    };


    std::thread thread_1([=] {
        run(std::bind(consume, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, 2), "consume_1");
    });

    std::thread thread_2([=] {
        run(std::bind(consume, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, 1), "consume_2");
    });

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(1s);

    run(publish, "publisher");

    thread_1.join();
    thread_2.join();

    return 0;
}