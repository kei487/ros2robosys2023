#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/Vector3.hpp"
using std::placeholders::_1;

class Listener : public rclcpp::Node
{
  public:
    Listener()
    : Node("listener")
    {
      subscription_ = this->create_subscription<geometry_msgs::msg::Vector3>(
      "countup", 10, std::bind(&Listener::topic_callback, this, _1));
    }

  private:
    void topic_callback(const geometry_msgs::msg::Vector3 & msg) const
    {
      RCLCPP_INFO(this->get_logger(), "I heard: '%lf', '%lf', '%lf'", msg.x, msg.y, msg.z);
    }
    rclcpp::Subscription<geometry_msgs::msg::Vector3>::SharedPtr subscription_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Listener>());
  rclcpp::shutdown();
  return 0;
}
