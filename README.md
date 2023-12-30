# Calculate Inverse Kinematics for 7 DOF Manipulator
![test](https://github.com/kei487/robosys2023/actions/workflows/test.yml/badge.svg)

* [イントロダクション](#イントロダクション)
* [パッケージ詳細](#パッケージ詳細)
  * [ノード](#calculaterノード)
* [使い方](#使い方)
  * [インストール方法](#インストール方法)
  * [実行方法](#コードの実行方法)
* [要求環境](#要求環境)
  * [バージョン情報](#バージョン情報)
  * [テスト環境](#テスト環境)
* [ライセンス](#ライセンス)

# イントロダクション
  このパッケージは7自由度マニピュレータの逆運動学解を計算するROS 2パッケージです.

# パッケージ詳細
## calculaterノード
![node,topic図](https://github.com/kei487/ros2robosys2023/assets/79034190/0ca10c8e-05cf-4074-a900-6ebb07085166)

geometry_msgs/msg/Vector3を先端の目標座標としてサブスクラブし、[joint_msgs/msg/JointAngle](https://github.com/kei487/joint_msgs)を関節角度としてパブリッシュします.

# 使い方
## インストール方法
```
$ cd ros2_ws/src
$ git clone https://github.com/kei487/ros2robosys2023.git
$ cd .. && colcon build
$ source install/setup.bash
```

## 実行方法
 launchファイルで実行を確認することができます.
* 実行例
```
$ ros2 launch ros2robosys2023 example.launch.py 
```

# 要求環境
## バージョン情報
* ROS 2 humble
## テスト環境
* Ubuntu 22.04 LTS

# ライセンス
* このソフトウェアパッケージは，3条項BSDライセンスの下，再頒布および使用が許可されます．
* このパッケージのコードの一部は，下記のスライド（CC-BY-SA 4.0 by Ryuichi Ueda）のものを，本人の許可を得て自身の著作としたものです．
   [ryuichiueda/robosys_2022/lesson10](https://ryuichiueda.github.io/my_slides/robosys_2022/lesson10.html#/)
 
 © 2023 Keitaro Nakamura 
