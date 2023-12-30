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

cd $dir/ros2_ws
colcon build
source install/setup.bash
timeout 20 ros2 launch ros2robosys2023 example.launch.py > /tmp/ros2robosys2023.log

cat /tmp/ros2robosys2023.log | grep 'Subscripted' || ng
exit $res
