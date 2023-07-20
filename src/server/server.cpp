#include <iostream>
#include <boost/asio.hpp>

#include "async.hpp"

namespace ba = boost::asio;

class session : public std::enable_shared_from_this<session> {
public:
    session(ba::ip::tcp::socket socket, const size_t _bulk_size):
            socket_(std::move(socket)), bulk_size(_bulk_size) {
        h = connect(bulk_size);
    }

    void start() {
        do_read();
    }

private:
    void do_read() {
        auto self(shared_from_this());
        socket_.async_read_some(boost::asio::buffer(data, max_length),
                                [this, self](boost::system::error_code ec, std::size_t length) {
                                    if (!ec && length > 0) {
                                        std::istringstream commandSequence;
                                        commandSequence.str(std::string{data, length});

                                        std::string command;
                                        while (std::getline(commandSequence, command, '\n')) {
                                            receive(command.c_str(), command.length(), h);
                                        }

                                        do_read();

                                    } else {
                                        //disconnect(h);
                                        //std::cerr << "Disconnect.Code Error: " << ec.message() << std::endl;
                                    }
                                });
    }

    ba::ip::tcp::socket socket_;
    const size_t bulk_size;
    size_t h;
    enum {
        max_length = 1024
    };
    char data[max_length];
};

class Server {
public:
    Server(const size_t port, const size_t _bulk_size):
        bulk_size(_bulk_size), acceptor_(io_context, ba::ip::tcp::endpoint(ba::ip::tcp::v4(), port)) {
        do_accept();
        io_context.run();
    }

private:
    void do_accept() {
        acceptor_.async_accept(
            [this](boost::system::error_code ec, ba::ip::tcp::socket socket) {
                if(!ec) {
                    std::make_shared<session>(std::move(socket), bulk_size)->start();
                } else
                    std::cerr << "Code Error: " << ec.message() << std::endl;
                do_accept();
            });
    }
    const size_t bulk_size;
    boost::asio::io_context io_context;
    ba::ip::tcp::acceptor acceptor_;
};

