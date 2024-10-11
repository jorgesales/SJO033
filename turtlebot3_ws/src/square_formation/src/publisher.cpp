#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "geometry_msgs/msg/twist.hpp"

using namespace std::chrono_literals;
using namespace std;

/* This example creates a subclass of Node and uses std::bind() to register a
* member function as a callback from the timer. */

class MinimalPublisher : public rclcpp::Node
{
  public:
    MinimalPublisher()
    : Node("minimal_publisher"), count_(0)
    {
      publisher_ = this->create_publisher<std_msgs::msg::String>("topic", 10);
      publisher_cmd_vel = this->create_publisher<geometry_msgs::msg::Twist>("cmd_vel", 10);
      timer_ = this->create_wall_timer(
      500ms, std::bind(&MinimalPublisher::timer_callback, this));
    }

  private:
    void timer_callback()
    {
      auto message = std_msgs::msg::String();
      auto message2 = geometry_msgs::msg::Twist();    
      message.data = "Hello, world! v02" + std::to_string(count_++);
      message2.linear.x = 0.05;
      message2.linear.y = 0.00;
      message2.linear.z = 0.00;
      message2.angular.x = 0.0;
      message2.angular.y = 0.0;
      message2.angular.z = 0.0;

      RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());
      RCLCPP_INFO(this->get_logger(), "Publishing cmd_vel:  x='%s' y='%s'", to_string(message2.linear.x).c_str(), to_string(message2.linear.y).c_str());

      publisher_->publish(message);
      publisher_cmd_vel->publish(message2);
    }
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_cmd_vel;
    size_t count_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MinimalPublisher>());
  rclcpp::shutdown();
  return 0;
}