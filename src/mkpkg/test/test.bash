#!/bin/bash

dir=~
[ "$1" != "" ] && dir="$1"   #引数があったら、そちらをホームに変える。

cd $dir/git_robosys/ros2robosys2023
colcon build
source $dir/.bashrc
timeout 10 ros2 launch mkpkg talk_listen.launch.py > /tmp/mkpkg.log

cat /tmp/mkpkg.log |
grep 'Listen: 10'
