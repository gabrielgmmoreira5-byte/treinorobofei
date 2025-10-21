#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <std_msgs/msg/int64.hpp>
#include <iostream>
#include <random>
#include <cmath>
#include "std_msgs/msg/float64.hpp"

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;
using std::placeholders::_1;

float pct, a;

class Treinobub : public rclcpp::Node
{
  public:
    Treinobub()
    : Node("treinito"), count_(0)
    {
      subscription_ = this->create_subscription<std_msgs::msg::Int64>(
      "topic2", 10, std::bind(&Treinobub::topic_callback, this, _1)); 
        
      publisher_ = this->create_publisher<std_msgs::msg::Float64>("topic3", 10);
      timer_ = this->create_wall_timer(
      500ms, std::bind(&Treinobub::timer_callback, this));

    }

  private:
    void topic_callback(const std_msgs::msg::Int64 & msg) const
    {
      RCLCPP_INFO(this->get_logger(), "Recebi a quantidade de acertos: '%ld'", msg.data);
      if(msg.data == 0){
        a = 0;
      }
      else{
        a = std::pow(0.1,msg.data);
      }
      pct = a*100;
    }
    void timer_callback()
    {
      auto message = std_msgs::msg::Float64();
      message.data = pct;
      RCLCPP_INFO(this->get_logger(), "Chance dos seus acertos: '%.4f' por cento", message.data);
      publisher_->publish(message);
    }
    
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr publisher_;
    rclcpp::Subscription<std_msgs::msg::Int64>::SharedPtr subscription_;
    size_t count_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Treinobub>());
  rclcpp::shutdown();
  return 0;
}