#include <memory>

#include "rclcpp/rclcpp.hpp"

#include "seahawk2/srv/control_function.hpp"

void call_function(const std::shared_ptr<seahawk2::srv::ControlFunction::Request> request,
                   std::shared_ptr<seahawk2::srv::ControlFunction::Response> response)
{
  request->function;
  request->value;
  request->type;




  response->state = 0.0; // TODO: give this meaning!
}

// https://docs.ros.org/en/rolling/Tutorials/Beginner-Client-Libraries/Writing-A-Simple-Cpp-Service-And-Client.html
