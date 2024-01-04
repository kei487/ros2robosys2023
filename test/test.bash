#!/bin/bash
# SPDX-FileCopyrightText: 2023 Keitaro Nakamura
# SPDX-License-Identifier: BSD-3-Clause


dir=~
[ "$1" != "" ] && dir="$1"   #引数があったら、そちらをホームに変える。

cd $dir/ros2_ws
source install/setup.bash
timeout 10 ros2 launch ros2robosys2023 test.launch.py > /tmp/ros2robosys2023.log

cat /tmp/ros2robosys2023.log |
grep 'Listen: 10' || ng
