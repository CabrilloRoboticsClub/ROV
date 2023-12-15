# import os, re
# import pathlib
# from sys import exit
from launch import LaunchDescription
from launch_ros.actions import Node

thruster_matrix_2d = [
    [      0,      0,      0,      0,  0.7071,  0.7071, -0.7071, -0.7071 ],  # Fx
    [      0,      0,      0,      0, -0.7071,  0.7071, -0.7071,  0.7071 ],  # Fy
    [      1,      1,      1,      1,       0,       0,       0,       0 ],  # Fz
    [  -0.19,  -0.19,   0.19,   0.19,  -0.105,  -0.105,   0.105,   0.105 ],  # Rx (mm)
    [  -0.12,   0.12,  -0.12,   0.12,   -0.15,    0.15,   -0.15,    0.15 ],  # Ry (mm)
    [ -0.047, -0.047, -0.047, -0.047,   0.038,   0.038,   0.038,   0.038 ],  # Rz (mm)
]

thruster_matrix = [value for row in thruster_matrix_2d for value in row]

def generate_launch_description():
    nodes = [
        Node(
            package='seahawk2',
            executable='input_map',
            name='input_map',
            output='screen',
        ),
        Node(
            package='joy_linux',        # TEMPORARY: I can't get the controller to work
            executable='joy_linux_node',# in the docker container otherwise, even when
            name='joy_node',            # I use --device=/dev/input/js0:/dev/input/js0
            output='screen',
        ),
        # Node(
        #     package='seahawk2',
        #     executable='kinematics',
        #     name='kinematics',
        #     output='screen',
        #     # // auto param = this->declare_parameter("thruster_force_matrix", rclcpp::ParameterType::PARAMETER_DOUBLE_ARRAY);
        #     # // auto param = this->declare_parameter("config", default_value);
        #     parameters=[
        #         {'force_matrix': thruster_matrix},
        #         {'force_matrix_rows': len(thruster_matrix_2d)},
        #         {'force_matrix_columns': len(thruster_matrix_2d[0])},
        #     ],
        # ),
        # Node(
        #     package='seahawk2',
        #     executable='pwm_interface',
        #     name='pwm_interface',
        #     output='screen',
        # ),
    ]
    return LaunchDescription(nodes)

