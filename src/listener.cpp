// Copyright 2023 Keitaro Nakamura
// SPDX-License-Identifier: Apache 2.0
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "joint_msgs/msg/joint_angle.hpp"
using std::placeholders::_1;

class Listener : public rclcpp::Node
{
  public:
    Listener()
    : Node("listener")
    {
      listening_ = this->create_subscription<joint_msgs::msg::JointAngle>(
      "/joint_angle", 10, std::bind(&Listener::topic_callback, this, _1));
    }

  private:
    void topic_callback(const joint_msgs::msg::JointAngle & message) const
    {
      RCLCPP_INFO(this->get_logger(),"Subscripted::[1]: %lf, [2]: %lf, [3]: %lf, [4]: %lf, [5]: %lf, [6]: %lf, [7]: %lf", message.joint1, message.joint2, message.joint3, message.joint4, message.joint5, message.joint6, message.joint7);
    }
    rclcpp::Subscription<joint_msgs::msg::JointAngle>::SharedPtr listening_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Listener>());
  rclcpp::shutdown();
  return 0;
}
