#include <algorithm>
#include <iterator>
#include <memory>
#include <utility>
#include <vector>
#include <ranges>

#include "rclcpp/rclcpp.hpp"

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


// When recieving joystick message rcv_msg: {
//   msg = translation layer (dpad)
//   buttons {
//     do event detection
//     function call
//   }
//   axes {
//     transform with curves
//     emit twist
//   }
// }
// Register schema by binding function calls to states using event type? then when event matches, call bound function
// how to bind: read schema, for each event bind to translated button index

class InputMap : public rclcpp::Node
{
public:
  InputMap(/*seahawk::input::XboxOne schema*/) : Node("input_map")
  {
    controller_sub_ = this->create_subscription<sensor_msgs::msg::Joy>(
        "joy", 10, std::bind(&InputMap::callback_fn, this, std::placeholders::_1));
    // twist_pub_ = this->create_publisher<geometry_msgs::msg::Twist>("input/twist", 10);
    // api_client_ = this->create_client<seahawk2::srv::ControlFunction>("input/function_api");
    // bindings = schema;
    old_data = sensor_msgs::msg::Joy();
  }

private:
  // Describe what ROV functions you want controlled by buttons
  // seahawk::input::Schema bindings;


  void callback_fn(const sensor_msgs::msg::Joy &rcv_msg);
  // Translate DPad axis to buttons
  const sensor_msgs::msg::Joy translate(const sensor_msgs::msg::Joy &msg_in);
  // Transform axes with curves
  const sensor_msgs::msg::Joy transform(const sensor_msgs::msg::Joy &msg_in);

  // Subscription to /joy topic to take controller inputs.
  rclcpp::Subscription<sensor_msgs::msg::Joy>::SharedPtr controller_sub_;
  // Publish "raw" unprocessed input movement vector.
  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr twist_pub_;
  // map axes to input/twist
  // separately pub buttons as keymap datastructure
  // void pub_relays(auto data); // change signature
  // void pub_twist(auto data);
  sensor_msgs::msg::Joy old_data;
};


void InputMap::callback_fn(const sensor_msgs::msg::Joy &rcv_msg)
{
  
  // This doesn't handle what happens if the controller changes, that has to be done in a launch file probably
  RCLCPP_INFO_STREAM(this->get_logger(), "Joystick axes: " << rcv_msg.axes << " buttons: " << rcv_msg.buttons);

  const sensor_msgs::msg::Joy msg = rcv_msg;//transform(translate(rcv_msg));

  for (std::size_t idx : std::ranges::iota_view(0, int(rcv_msg.buttons.size()))) {
    auto event = "up";
    if (old_data.buttons.at(idx) == 0 && msg.buttons.at(idx) == 1); // event button_down
    else if (old_data.buttons.at(idx) == 1 && msg.buttons.at(idx) == 0); // event button_up
    // schema.buttons.at(idx);
  }
  // Events:
  // Pressed, released
  // auto buttons_pair = std::make_pair(std::cbegin(msg.buttons), std::cbegin(old_data.buttons));
  // while (buttons_pair.first != std::cend(msg.buttons)
  //       && buttons_pair.second != std::cend(old_data.buttons)) {
  //   buttons_pair = std::mismatch(buttons_pair.first, std::cend(msg.buttons),
  //                                buttons_pair.second, std::cend(old_data.buttons));
    // TODO: call functions at scheme.buttons[std::distance(std::cbegin(msg.buttons), buttons_pair.first)][1]
  // }


  // always send twist info



  this->old_data = rcv_msg;
}

int main(int argc, char* argv[])
{
  rclcpp::init(argc, argv);
  // seahawk::input::XboxOne is an instance of seahawk::input::Schema
  // InputMap::SharedPtr node = std::make_shared<InputMap>();
  rclcpp::Node::SharedPtr node = std::make_shared<InputMap>(/*seahawk::input::Mapping::XboxOne*/);
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
