#include <boost/asio/io_service.hpp>
#include <boost/asio/strand.hpp>
#include <boost/asio/deadline_timer.hpp>

#include <amqpcpp.h>
#include <amqpcpp/libboostasio.h>


/**
 *  Main program
 *  @return int
 */
int main() {
    constexpr size_t max_messages = 24000;

    boost::asio::io_service service(1);

    AMQP::LibBoostAsioHandler handler(service);
    AMQP::TcpConnection connection(&handler, AMQP::Address("amqp://guest:guest@localhost/"));
    AMQP::TcpChannel channel(&connection);

    channel.onError([&connection](auto message) {
        std::cout << "Error: " << message << std::endl;
        connection.close();
    });

    channel.declareExchange("my-exchange", AMQP::fanout);
    channel.declareQueue("my-queue", AMQP::durable);
    channel.bindQueue("my-exchange", "my-queue", "my-routing-key");

    channel.confirmSelect()
            .onAck([&](uint64_t deliveryTag, bool multiple) {
                std::cout << "ack " << deliveryTag << " " << multiple << std::endl;
            }).onNack([&](uint64_t deliveryTag, bool multiple, bool requeue) {
                std::cout << "nack " << deliveryTag << " " << multiple << std::endl;
            });

    channel.consume("my-queue")
            .onSuccess([] {
                std::cout << "consume operation started" << std::endl;
            })
            .onReceived([&, max_messages](const AMQP::Message & message, uint64_t deliveryTag, bool redelivered) {
                std::string_view string(message.body(), message.bodySize());
                std::cout << "message received " << deliveryTag << " " << string << std::endl;
                channel.ack(deliveryTag);
                if (deliveryTag == max_messages) {
                    connection.close();
                }
            });

    for (int i = 1; i <= max_messages; i++) {
        channel.publish("my-exchange", "my-key", std::to_string(i));
    }

    service.run();

    return 0;
}