# SPDX-FileCopyrightText: 2023 Keitaro Nakamura
# SPDX-License-Identifier: BSD-3-Clause
import os

from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    talker_node = Node(
             package='ros2robosys2023',
             executable='talker',
        )
    calculater_node = Node(
             package='ros2robosys2023',
             executable='calculater',
        )
    listener_node = Node(
             package='ros2robosys2023',
             executable='listener',
             output='screen'
        )
    return LaunchDescription([
        talker_node,
        calculater_node,
        listener_node,
    ])
