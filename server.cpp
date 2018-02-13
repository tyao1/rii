#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <memory>
#include <array>
#include <vector>
#include <functional>
#include <gamepad.hpp>

using boost::asio::ip::udp;

struct packet {
  uint8_t idx;
  uint8_t reserve;
  gamepad_report report;

  void print() {
    printf("Device: %d, Reserve: %d \n", idx, reserve);
    report.print();
  }
};


class rii
{
public:
  rii(boost::asio::io_context& io_context, int port)
    : socket_(io_context, udp::endpoint(udp::v4(), port)), running_{true}
  {
    start_receive();
    printf("Listening on port %d\n", port);
  }

  int count() {
    return gamepads_.size();
  }
  /*
  void stop() {
    running_ = true;
  }
  */
private:

  void start_receive()
  {
    socket_.async_receive_from(
        boost::asio::buffer(recv_buffer_), remote_endpoint_,
        boost::bind(&rii::handle_receive, this,
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));
  }

  // it's single threaded, otherwise thread safety need to be taken into account
  void handle_receive(const boost::system::error_code& error,
      std::size_t /*bytes_transferred*/)
  {

    if (!error || error == boost::asio::error::message_size)
    {
      // std::cout << remote_endpoint_.address() << std::endl;
      packet pckt = recv_buffer_.front();
      unsigned char id = pckt.idx;
      if (id == 255 || id >= gamepads_.size()) {
        // create a new gamepad
        unsigned char new_id = gamepads_.size();
        // auto data = std::make_shared<int>(id);
        std::shared_ptr<Gamepad> pad{new Gamepad(
          ("RII GAMEPAD " + std::to_string(new_id)).c_str(),
          ("SN 666" + std::to_string(new_id)).c_str()
        )};
        for (int i = 0; i < 3; i++) {
          socket_.async_send_to(boost::asio::buffer(&new_id, 2), remote_endpoint_,
              boost::bind(&rii::handle_send, this, nullptr,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
        }
        gamepads_.push_back(pad);
      } else {
        gamepads_[id]->send_state(pckt.report);
        pckt.print();
      }
    } else {
      std::cout << error.message() << std::endl;
    }
    // if (running_)
    start_receive();
  }

  void handle_send(boost::shared_ptr<std::string> /*message*/,
      const boost::system::error_code& error,
      std::size_t /*bytes_transferred*/)
  {
    if (error) {
      std::cout << error.message() << std::endl;
    }
  }
  std::vector<std::shared_ptr<Gamepad>> gamepads_;
  udp::socket socket_;
  udp::endpoint remote_endpoint_;
  std::array <packet, 1> recv_buffer_;
  bool running_;
};

int main()
{
  boost::asio::io_context io_context;
  rii server(io_context, 6666);
  auto t = std::thread(boost::bind(&boost::asio::io_context::run, &io_context));

  std::cout << "Input Command: c - show devices count, q - quit and cleanup, \
    \n";

  char c;
  while (c != 'q') {
    printf("> ");
    std::cin >> c;
    switch (c) {
      case 'c':
        printf("Devices: %d\n", server.count());
        break;
            default:
        break;
    }
  }

  io_context.stop();
  t.join();

  return 0;
}
