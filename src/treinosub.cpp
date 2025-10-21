#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <std_msgs/msg/int64.hpp>
#include <iostream>
#include <random>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;
using std::placeholders::_1;

int numAle, cont = 0;

class Treinosub : public rclcpp::Node
{
  public:
    Treinosub()
    : Node("treininsubscry")
    {
      subscription_ = this->create_subscription<std_msgs::msg::Int64>(
      "topic1", 10, std::bind(&Treinosub::topic_callback, this, _1));

      publisher_ = this->create_publisher<std_msgs::msg::Int64>("topic2", 10);
      timer_ = this->create_wall_timer(
      500ms, std::bind(&Treinosub::timer_callback, this));

    }

  private:
    void topic_callback(const std_msgs::msg::Int64 & msg) const
    {
      RCLCPP_INFO(this->get_logger(), "Recebi o numero: '%ld'", msg.data);

      std::random_device rd;  
      std::mt19937 gen(rd()); 
      std::uniform_int_distribution<> dist(1, 10);
      numAle = dist(gen);
      if(numAle == msg.data){
        cont = cont + 1;
      }
      else{
        cont = cont;
      }
    }
    void timer_callback()
    {
      auto message = std_msgs::msg::Int64();
      message.data = cont;
      RCLCPP_INFO(this->get_logger(), "Contador de acertos: '%ld'", message.data);
      publisher_->publish(message);
    }

    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<std_msgs::msg::Int64>::SharedPtr publisher_;
    rclcpp::Subscription<std_msgs::msg::Int64>::SharedPtr subscription_;
    size_t count_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Treinosub>());
  rclcpp::shutdown();
  return 0;
}