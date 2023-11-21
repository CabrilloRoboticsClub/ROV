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
            package='seahawk2',
            executable='kinematics',
            name='kinematics',
            output='screen',
            parameters=[
                {'force_matrix': thruster_matrix},
                {'force_matrix_rows': len(thruster_matrix_2d)},
                {'force_matrix_columns': len(thruster_matrix_2d[0])},
            ],
        ),
        Node(
            package='seahawk2',
            executable='pwm_interface',
            name='pwm_interface',
            output='screen',
        ),
    ]
    return LaunchDescription(nodes)

