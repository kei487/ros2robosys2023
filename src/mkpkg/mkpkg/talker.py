import rclpy                     #ROS 2のクライアントのためのライブラリ
from rclpy.node import Node      #ノードを実装するためのNodeクラス（クラスは第10回で）
from std_msgs.msg import Int16 #通信の型（16ビットの符号付き整数）

class Talker():
    def __init__(self,node):
        self.pub = node.create_publisher(Int16,"countup",10)
        self.n = 0
        node.create_timer(0.5, self.cb)  #タイマー設定

    def cb(self):          #17行目で定期実行されるコールバック関数
        msg = Int16()  #メッセージの「オブジェクト」
        msg.data = self.n
        self.pub.publish(msg)        #pubの持つpublishでメッセージ送信
        self.n += 1

def main():
    rclpy.init()
    node = Node("talker")            #ノード作成（nodeという「オブジェクト」を作成）
    talker = Talker(node)
    rclpy.spin(node)            #実行（無限ループ

if __name__=='__main__':
    main()
