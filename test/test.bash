#!/bin/bash

dir=~
[ "$1" != "" ] && dir="$1"   #引数があったら、そちらをホームに変える。

cd $dir/ros2_ws
colcon build
source install/setup.bash
timeout 10 ros2 launch ros2robosys2023 example.launch.py > /tmp/ros2robosys2023.log

cat /tmp/ros2robosys2023.log |
grep 'Subscripted::[1]: -1.570796, [2]: -0.794462, [3]: 3.141593, [4]: -2.657895, [5]: -3.141593, [6]: -0.465413, [7]: -3.139241'
