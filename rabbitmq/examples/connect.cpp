#include <boost/asio/io_service.hpp>

#include <amqpcpp.h>
#include <amqpcpp/libboostasio.h>

/// Простое соединение с отправкой и получением

int main() {

    constexpr auto exchange_name = "ex_hello";
    constexpr auto queue_name = "hello";
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

    AMQP::TcpChannel channel(&connection);

    channel.onError([&connection] (auto message) {
        std::cout << "Error: " << message << std::endl;
        connection.close();
    });

    channel.declareExchange(exchange_name, AMQP::direct, AMQP::durable);
    channel.declareQueue(queue_name, AMQP::ifunused | AMQP::autodelete);
    channel.bindQueue(exchange_name, queue_name, routing_key);
    channel.publish(exchange_name, routing_key, "hello_world");

    channel.consume(queue_name, AMQP::noack)
    .onReceived([&connection, &channel] (const AMQP::Message & message, uint64_t deliveryTag, bool redelivered) {
        std::string_view string(message.body(), message.bodySize());
        std::cout << "message received: " << string << " " << deliveryTag << std::endl;
        connection.close();
    });

    service.run();

    return 0;
}