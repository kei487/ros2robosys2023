# Copyright 2023 Keitaro Nakamura
# SPDX-License-Identifier: Apache 2.0
import os

from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    talker_node = Node(
             package='ros2rbsys',
             executable='talker',
        )
    calculater_node = Node(
             package='ros2rbsys',
             executable='calculater',
        )
    listener_node = Node(
             package='ros2rbsys',
             executable='listener',
             output='screen'
        )
    return LaunchDescription([
        talker_node,
        calculater_node,
        listener_node,
    ])
