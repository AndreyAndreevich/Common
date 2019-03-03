#include <iostream>

#include <amqpcpp.h>
#include <amqpcpp/linux_tcp.h>

class MyTcpHandler : public AMQP::TcpHandler
{
    void onAttached(AMQP::TcpConnection *connection) override {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }

    void onConnected(AMQP::TcpConnection *connection) override {
        std::cout << __PRETTY_FUNCTION__ << std::endl;

    }

    bool onSecured(AMQP::TcpConnection *connection, const SSL *ssl) override {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        return true;
    }

    void onReady(AMQP::TcpConnection *connection) override {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }

    void onError(AMQP::TcpConnection *connection, const char *message) override {
        std::cout << __PRETTY_FUNCTION__ << " " << message << std::endl;
    }

    void onClosed(AMQP::TcpConnection *connection) override {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }

    void onLost(AMQP::TcpConnection *connection) override {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }

    void onDetached(AMQP::TcpConnection *connection) override {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }

    virtual void monitor(AMQP::TcpConnection *connection, int fd, int flags) override
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }
};

int main() {

    MyTcpHandler myHandler;

    AMQP::Address address("amqp://guest:guest@localhost/vhost");

    AMQP::TcpConnection connection(&myHandler, address);

    AMQP::TcpChannel channel(&connection);

    std::cout << "start" << std::endl;

    channel.declareExchange("my-exchange", AMQP::fanout);
    channel.declareQueue("my-queue");
    channel.bindQueue("my-exchange", "my-queue", "my-routing-key");

    return 0;
}