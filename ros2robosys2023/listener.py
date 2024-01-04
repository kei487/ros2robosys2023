# SPDX-FileCopyrightText: 2023 Keitaro Nakamura
# SPDX-License-Identifier: BSD-3-Clause
import rclpy
from rclpy.node import Node
from std_msgs.msg import Int16

def callback(msg):
    global node
    node.get_logger().info("Listen: %d" % msg.data)

rclpy.init()
node = Node("listener")
sub = node.create_subscription(Int16, "countup", callback, 10)
rclpy.spin(node)
