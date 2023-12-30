// Copyright 2023 Keitaro Nakamura
// SPDX-License-Identifier: Apache 2.0
#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/vector3.hpp"

using namespace std::chrono_literals;

/* This example creates a subclass of Node and uses std::bind() to register a
* member function as a callback from the timer. */

class Talker : public rclcpp::Node
{
  public:
    Talker()
    : Node("tallker"), count_(0)
    {
      talker_ = this->create_publisher<geometry_msgs::msg::Vector3>("/position", 1);
      timer_ = this->create_wall_timer(
      500ms, std::bind(&Talker::timer_callback, this));
    }

  private:
    void timer_callback()
    {
      auto message = geometry_msgs::msg::Vector3();
      message.x = count_++;
      message.y = count_++;
      message.z = count_++;
      RCLCPP_INFO(this->get_logger(), "Publishing::x: '%lf', y: '%lf', z: '%lf'", message.x, message.y, message.z);
      talker_->publish(message);
    }
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<geometry_msgs::msg::Vector3>::SharedPtr talker_;
    size_t count_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Talker>());
  rclcpp::shutdown();
  return 0;
}
