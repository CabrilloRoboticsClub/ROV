cmake_minimum_required(VERSION 3.8)
project(seahawk2)

# Default to C++20
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 20)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

# FIXME: I'm not sure we need all these
find_package(ament_cmake_auto REQUIRED)
find_package(std_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(rosidl_default_generators REQUIRED)

# uhhhh... i think this is needed?
# find_package(rmw_implementation_cmake REQUIRED)
# get_available_rmw_implementations(rmw_implementations2)
# foreach(rmw_implementation ${rmw_implementations2})
  # find_package("${rmw_implementation}" REQUIRED)
# endforeach()

ament_auto_find_build_dependencies()

set(msg_files
  "msg/Matrix.msg"
  # "msg/SeahawkRelays.msg" # TODO: RENAME
)

set(srv_files
  "srv/ControlFunction.srv"
)

rosidl_generate_interfaces(${PROJECT_NAME} ${msg_files} ${srv_files}) #DEPENDENCIES seahawk2 SKIP_INSTALL) # WHAT DOES THIS LAST PART MEAN???

ament_export_dependencies(rosidl_default_runtime)

rosidl_get_typesupport_target(cpp_typesupport_target
  ${PROJECT_NAME} rosidl_typesupport_cpp)

# get typesupport of rmw implementation to include / link against the corresponding interfaces
# get_rmw_typesupport(typesupport_impls "${rmw_implementations2}" LANGUAGE "cpp")


# PWM interface
# add_executable(pwm_interface hardware_integration/pwm_interface/pwm_interface.cpp)
# ament_target_dependencies(pwm_interface rclcpp)
# target_include_directories(pwm_interface PUBLIC
#   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
#   $<INSTALL_INTERFACE:include>) # TODO: CHANGE INCLUDE DIRS
# target_link_libraries(pwm_interface "${cpp_typesupport_target}")
# target_compile_features(pwm_interface PUBLIC c_std_99 cxx_std_17)

# Controller input to control map
add_executable(input_map ux-input/input_map.cpp)
# add_dependencies(input_map ${PROJECT_NAME})
ament_target_dependencies(input_map rclcpp std_msgs sensor_msgs)
target_include_directories(input_map PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>) # TODO: CHANGE INCLUDE DIRS
target_link_libraries(input_map "${cpp_typesupport_target}")
target_compile_features(input_map PUBLIC c_std_99 cxx_std_20)


install(
  TARGETS
    # pwm_interface
    input_map
  DESTINATION lib/${PROJECT_NAME}
)

install(
  DIRECTORY launch
  DESTINATION share/${PROJECT_NAME}
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
endif()

ament_auto_package()
