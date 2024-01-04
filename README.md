# Calculate Inverse Kinematics for 7 DOF Manipulator
![test](https://github.com/kei487/ros2robosys2023/tree/submission/actions/workflows/test.yml/badge.svg)

* [パッケージ詳細](#パッケージ詳細)
  * [ノード](#calculaterノード)
* [使い方](#使い方)
  * [インストール方法](#インストール方法)
  * [実行方法](#コードの実行方法)
* [要求環境](#要求環境)
  * [バージョン情報](#バージョン情報)
  * [テスト環境](#テスト環境)
* [ライセンス](#ライセンス)


# パッケージ詳細
## topic:countup
<img width="254" alt="スクリーンショット 2024-01-04 230836" src="https://github.com/kei487/ros2robosys2023/assets/79034190/1c25f194-52d2-446f-bd71-82cb47aefbf9">
  
このパッケージではtalkerノードとlistenerノードが実装されておりtalkerノードは0.5秒おきに１ずつ増える整数値をcountupにパブリッシュしlistenerノードはcountupからサブスクライブします.  

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
$ ros2 launch ros2robosys2023 example.launch.xml 
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
