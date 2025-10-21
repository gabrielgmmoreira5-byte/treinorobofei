#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <std_msgs/msg/int64.hpp>
#include <iostream>
#include <random>
#include "std_msgs/msg/float64.hpp"

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;
using std::placeholders::_1;

class Treinopub : public rclcpp::Node
{
  public:
    Treinopub()
    : Node("treinin"), count_(0)
    {
      publisher_ = this->create_publisher<std_msgs::msg::Int64>("topic1", 10);
      timer_ = this->create_wall_timer(
      500ms, std::bind(&Treinopub::timer_callback, this));

      subscription_ = this->create_subscription<std_msgs::msg::Float64>(
      "topic3", 10, std::bind(&Treinopub::topic_callback, this, _1));
    }

  private:
    void timer_callback()
    {
      std::random_device rd;  
      std::mt19937 gen(rd()); 
      std::uniform_int_distribution<> dist(1, 10);

      auto message = std_msgs::msg::Int64();
      message.data = dist(gen);
      RCLCPP_INFO(this->get_logger(), "Numero aleatorio: '%ld'", message.data);
      publisher_->publish(message);
    }
    void topic_callback(const std_msgs::msg::Float64 & msg) const
    {
      RCLCPP_INFO(this->get_logger(), "A chance dos acertos eh: '%.4f'por cento", msg.data);
    }
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<std_msgs::msg::Int64>::SharedPtr publisher_;
    rclcpp::Subscription<std_msgs::msg::Float64>::SharedPtr subscription_;
    size_t count_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Treinopub>());
  rclcpp::shutdown();
  return 0;
}