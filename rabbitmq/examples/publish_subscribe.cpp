#include <thread>

#include <boost/asio/io_service.hpp>

#include <amqpcpp.h>
#include <amqpcpp/libboostasio.h>

/// Рассылка сообщений всем слушателям

int main() {

    constexpr auto exchange_name = "ex_hello3";
    constexpr auto routing_key = "foo-key";

    boost::asio::io_service service(1);

    AMQP::LibBoostAsioHandler handler(service);

    AMQP::TcpConnection connection(&handler
            , AMQP::Address(
                    "localhost"
                    , 5672
                    , AMQP::Login("guest","guest")
                    , "/"
            ));

    AMQP::TcpChannel consume_channel(&connection);

    consume_channel.onError([&connection] (auto message) {
        std::cout << "Error: " << message << std::endl;
        connection.close();
    });

    consume_channel.declareExchange(exchange_name, AMQP::fanout);

    /// Create queue with random name
    consume_channel.declareQueue(AMQP::ifunused | AMQP::autodelete | AMQP::durable)
    .onSuccess([&] (const std::string & queue_name, uint32_t messagecount, uint32_t consumercount) {

        std::cout << "success declare queue: " << queue_name << std::endl;
        consume_channel.bindQueue(exchange_name, queue_name, routing_key);
        consume_channel.consume(queue_name)
                .onReceived([&connection, &consume_channel](const AMQP::Message & message, uint64_t deliveryTag,
                                                            bool redelivered) {
                    std::string_view string(message.body(), message.bodySize());
                    constexpr size_t count = 5;
                    std::cout << "message received: " << string << " " << deliveryTag << std::endl;
                    if (count != deliveryTag) {
                        consume_channel.reject(deliveryTag, AMQP::requeue);
                    } else {
                        consume_channel.ack(deliveryTag);
                        connection.close();
                    }
                });
    });


    std::thread([&] {

        AMQP::TcpChannel publich_channel(&connection);

        publich_channel.onError([&connection] (auto message) {
            std::cout << "Error: " << message << std::endl;
            connection.close();
        });

        using namespace std::chrono_literals;
        std::this_thread::sleep_for(2s);

        publich_channel.declareExchange(exchange_name, AMQP::fanout);
        publich_channel.publish(exchange_name, routing_key, "hello_world");

        service.run();

    }).detach();


    service.run();

    return 0;
}