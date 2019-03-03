#include <boost/asio.hpp>
#include <chrono>
#include <thread>
#include <iostream>

using namespace boost::asio;

int main()
{
    io_service ioservice;
    ip::tcp::endpoint endpoint(ip::tcp::v4(), 5555);
    ip::tcp::acceptor acceptor(ioservice, endpoint);
    ip::tcp::socket socket(ioservice);
    streambuf buf;


    acceptor.async_accept(socket, [&buf, &socket] (boost::system::error_code ec) {
        std::cout << "accept\n";
        async_read_until(socket, buf, "\n", [&buf, &socket] (boost::system::error_code ec, size_t s) {
            std::istream in(&buf);
            std::string str;
            in >> str;
            std::cout << "read: " << str << "\n";
            async_write(socket, buffer("pong\n"), [] (boost::system::error_code ec, size_t) {
                std::cout << "write: " << ec << "\n";
            });
        });
    });


    steady_timer timer{ioservice, std::chrono::seconds{1}};

    timer.async_wait([](const boost::system::error_code &ec)
    { std::cout << "1 sec\n"; });


    std::thread thread1{[&ioservice](){ ioservice.run(); }};
    std::thread thread2{[&ioservice](){ ioservice.run(); }};
    std::thread thread3{[&ioservice](){ ioservice.run(); }};

    thread1.join();
    thread2.join();
    thread3.join();
}