#include <algorithm>
#include <iterator>
#include <memory>
#include <utility>
#include <vector>

#include "rclcpp/rclcpp.hpp"

#include "rclcpp/service.hpp"
#include "seahawk2/srv/detail/control_function__struct.hpp"
#include "sensor_msgs/msg/joy.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "seahawk2/srv/control_function.hpp"


// print vector
template <typename S>
std::ostream& operator<<(std::ostream& os, const std::vector<S>& vector)
{
    // Printing all the elements
    // using <<
    for (auto element : vector) {
        os << element << " ";
    }
    return os;
}


// namespace seahawk::input {
// struct Schema {
//   // temporary
//   int32_t valve_1;  // 12v
//   int32_t valve_2;  // 12v
//   int32_t valve_3;  // 12v
//   int32_t relay_1;  //  5v
//   int32_t relay_2;  //  5v
//   int32_t relay_3;  //  5v
// };
// struct XboxOne : Schema {
//   // temporary
//   int32_t valve_1 = 1;  // 12v
//   int32_t valve_2 = 4;  // 12v
//   int32_t valve_3 = 3;  // 12v
//   int32_t relay_1 = 2;  //  5v
//   int32_t relay_2 = 8;  //  5v
//   int32_t relay_3 = 7;  //  5v
// };
// struct Mapping {
//   XboxOne XboxOne {
//     // 1, 2
//   };
//   struct FlightStick : Schema {
//
//   };
// };
// // Enum interface
// // Schema map = Mapping::XboxOne
// } // namespace seahawk::input

// goal: bind inputs to functions on the control node
// XboxOne: {
//  buttons: [ // these should probably actually be lambda functions calling the service
//  { "A", [("set_claw", 2)] }, // 0 off 1 on 2 toggle
//  { "B", [("set_fish_release", 2)] },
//  { "X", [("set_bambi_mode", 2)] },
//  { "Y", [("set_bambi_mode", 0), ("set_ztrim", 3, 0.0)]}, // 3 set-absolute 4 rel-change(add)
//  { "LB", [("set_ztrim", 4, -0.5)] },
//  { "RB", [("set_ztrim", 4,  0.5)] },
//  { "View", [] },
//  { "Menu", [] },
//  { "Xbox", [] },
//  { "LS", [] },
//  { "RS", [] }
// ],
// axes: [ // bind axes // WORK OUT A BETTER WAY. Do math here?
//  { "Dpad_X", "" }, // Dpad horizontal, positive left
//  { "Dpad_Y", "" }, // Dpad vertical, positive up
// ]
//
//  { "LS_X", "linear_y" }, // positive left
//  { "LS_Y", "linear_x" }, // positive up
//  { "LT", "" }, // fully released positive 1, fully pressed -1
//  { "RS_X", "" },
//  { "RS_Y", "" },
//  { "RT", "" },

class ControlNode : public rclcpp::Node
{
public:
  ControlNode(/*seahawk::input::XboxOne schema*/) : Node("control_node")
  {
      this->create_service<seahawk2::srv::ControlFunction>("control_api_serv", control_api);
  }

private:
  // Describe what ROV functions you want controlled by buttons
  // seahawk::input::Schema bindings;

  // rclcpp::Service<seahawk2::srv::ControlFunction>::SharedPtr control_function_serv_;



  void control_api(const seahawk2::srv::ControlFunction &srv);
  // Publish "raw" unprocessed input movement vector.
  // map axes to input/twist
  // separately pub buttons as keymap datastructure
  // void pub_relays(auto data); // change signature
  // void pub_twist(auto data);
};


void ControlNode::control_api(const seahawk2::srv::ControlFunction &srv)
{

}

int main(int argc, char* argv[])
{
  rclcpp::init(argc, argv);
  // seahawk::input::XboxOne is an instance of seahawk::input::Schema
  rclcpp::Node::SharedPtr node = std::make_shared<ControlNode>(/*seahawk::input::Mapping::XboxOne*/);
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
