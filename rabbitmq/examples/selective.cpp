//
// Created by Andrey on 2019-03-06.
//

#include <thread>

#include <boost/asio/io_service.hpp>

#include <amqpcpp.h>
#include <amqpcpp/libboostasio.h>

/// Рассылка сообщений в четко определенные очереди в зависимости от routing_key

int main() {

    constexpr auto exchange_name = "log";

    constexpr auto info = "info";
    constexpr auto error = "error";

    boost::asio::io_service service(1);

    AMQP::LibBoostAsioHandler handler(service);

    AMQP::TcpConnection connection(&handler, AMQP::Address(
            "localhost", 5672, AMQP::Login("guest", "guest"), "/"
    ));



    auto register_channel = [=, &connection] (auto & channel, auto key) {
        channel.onError([&connection] (auto message) {
            std::cout << " Error: " << message << std::endl;
            connection.close();
        });

        channel.declareExchange(exchange_name, AMQP::direct);
        channel.declareQueue(key, AMQP::ifunused | AMQP::autodelete | AMQP::durable);
        channel.bindQueue(exchange_name, key, key);
        channel.consume(key)
                .onReceived([&, key] (const AMQP::Message & message, uint64_t deliveryTag, bool redelivered) {
                    std::string_view string(message.body(), message.bodySize());
                    std::cout << key << " message received: " << string << " " << deliveryTag << std::endl;
                    channel.ack(deliveryTag);
                    static size_t count = 0;

                    if (++count == 2) {
                        connection.close();
                    }
                });
    };

    AMQP::TcpChannel channel_info(&connection);
    register_channel(channel_info, info);

    AMQP::TcpChannel channel_error(&connection);
    register_channel(channel_error, error);

    std::thread([&] {

        AMQP::TcpChannel publish_channel(&connection);

        publish_channel.onError([&connection] (auto message) {
            std::cout << "Error: " << message << std::endl;
            connection.close();
        });

        using namespace std::chrono_literals;
        std::this_thread::sleep_for(2s);

        publish_channel.declareExchange(exchange_name, AMQP::direct);

        publish_channel.publish(exchange_name, info, "information message");
        publish_channel.publish(exchange_name, error, "error message");

        service.run();

    }).detach();

    service.run();

    return 0;
}