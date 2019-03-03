#include <iostream>

#include <amqpcpp.h>
#include <amqpcpp/linux_tcp.h>


class MyConnectionHandler : public AMQP::ConnectionHandler
{
    void onData(AMQP::Connection *connection, const char *data, size_t size) override {
        std::string_view string(data, size);
        std::cout << __PRETTY_FUNCTION__ << " " << string << std::endl;
    }

    void onReady(AMQP::Connection *connection) override {
        std::cout << __PRETTY_FUNCTION__ << " " << connection->vhost() << std::endl;
    }

    void onError(AMQP::Connection *connection, const char *message) override {
        std::cout << __PRETTY_FUNCTION__ << " " << message << std::endl;
    }

    void onClosed(AMQP::Connection *connection) override {
        std::cout << __PRETTY_FUNCTION__ << " " << connection->vhost() << std::endl;
    }
};

int main() {

    MyConnectionHandler myHandler;

    AMQP::Connection connection(&myHandler, AMQP::Login("guest","guest"), "/");

    AMQP::Channel channel(&connection);

    std::cout << "start" << std::endl;

    // use the channel object to call the AMQP method you like
    channel.declareExchange("my-exchange", AMQP::fanout)
            .onSuccess([]() {
                std::cout << "Chanel success" << std::endl;
            })

            .onError([](const char *message) {
                std::cout << "Chanel error " << message << std::endl;
            });
    channel.declareQueue("my-queue");
    channel.bindQueue("my-exchange", "my-queue", "my-routing-key");

    return 0;
}