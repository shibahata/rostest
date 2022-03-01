#include <chrono>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

class MinimalPublisher : public rclcpp::Node {
    size_t count_;
    std::string text_;

    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;

    void timer_callback() {
        std_msgs::msg::String message;
        message.data = text_ + std::to_string(count_++);
        RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());
        publisher_->publish(message);
    }
public:
    MinimalPublisher() : Node("node_name1", rclcpp::NodeOptions().allow_undeclared_parameters(true)), count_(0) {
        // 使用するパラメータの宣言
        declare_parameter("param1", 1);
        declare_parameter("param2", 1.3);
        declare_parameter("param3", "hoge");

        count_ = get_parameter("param1").as_int();
        text_ = get_parameter("param3").as_string();

        publisher_ = this->create_publisher<std_msgs::msg::String>("topic", 10);
        timer_ = this->create_wall_timer(500ms, std::bind(&MinimalPublisher::timer_callback, this));
    }
};


int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);

    rclcpp::spin(std::make_shared<MinimalPublisher>());

    rclcpp::shutdown();
    return 0;
}