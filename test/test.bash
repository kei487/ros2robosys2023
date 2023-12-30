#!/bin/bash
# SPDX-FileCopyrightText: 2023 Keitaro Nakamura
# SPDX-License-Identifier: BSD-3-Clause

ng () {
        echo NG
        res=1
}

res=0

dir=~
[ "$1" != "" ] && dir="$1"   #引数があったら、そちらをホームに変える。

cd $dir/ros2_ws/src
git clone https://github.com/kei487/joint_msgs.git
cd .. && colcon build
source install/setup.bash
timeout 10 ros2 launch ros2robosys2023 example.launch.py > /tmp/ros2robosys2023.log

cat /tmp/ros2robosys2023.log |
grep 'Subscripted::[1]: -1.570796, [2]: -0.794462, [3]: 3.141593, [4]: -2.657895, [5]: -3.141593, [6]: -0.465413, [7]: -3.139241' || ng
exit $res
