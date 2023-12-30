// SPDX-FileCopyrightText: 2023 Keitaro Nakamura
// SPDX-License-Identifier: BSD-3-Clause
#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <math.h>

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/vector3.hpp"
#include "joint_msgs/msg/joint_angle.hpp"

using namespace std::chrono_literals;
using std::placeholders::_1;

class Calculater : public rclcpp::Node
{
  public:
    Calculater()
    : Node("calculater")
    {
      subscription_ = this->create_subscription<geometry_msgs::msg::Vector3>(
      "/position", 1, std::bind(&Calculater::topic_callback, this, _1));
      calculating_ = this->create_publisher<joint_msgs::msg::JointAngle>("/joint_angle", 10);
    }

  private:
    rclcpp::Subscription<geometry_msgs::msg::Vector3>::SharedPtr subscription_;
    rclcpp::Publisher<joint_msgs::msg::JointAngle>::SharedPtr calculating_;
    void calculate();
    void topic_callback(const geometry_msgs::msg::Vector3 & msg) const
    {
      auto message = joint_msgs::msg::JointAngle();
      calc(message,msg);
      RCLCPP_INFO(this->get_logger(), "Publishing::[0]: %lf, [1]: %lf, [2]: %lf, [3]: %lf, [4]: %lf, [5]: %lf, [6]: %lf", message.joint1, message.joint2, message.joint3, message.joint4, message.joint5, message.joint6, message.joint7);
      calculating_->publish(message);
    }
    void calc(joint_msgs::msg::JointAngle & message,const geometry_msgs::msg::Vector3 & msg) const
    { 
      double Rt[3] = { 1.57 ,0, 1.57 };
      double L[4] = { 105,250,250,60 };double psi=0;

      double rt[3][3];
      rt[0][0] = cos(Rt[1])*cos(Rt[2]) ;  rt[0][1] = -1*sin(Rt[1])*sin(Rt[2]) ;  rt[0][2] = sin(Rt[1]);
      rt[1][0] = sin(Rt[0])*sin(Rt[1])*cos(Rt[2]) + cos(Rt[1])*sin(Rt[2]);  rt[1][1] = -1*sin(Rt[0])*sin(Rt[1])*sin(Rt[2]) + cos(Rt[0])*cos(Rt[2]);  rt[1][2] = -1*sin(Rt[0])*cos(Rt[1]);
      rt[2][0] = -1*cos(Rt[0])*sin(Rt[1])*cos(Rt[2]) + sin(Rt[0])*sin(Rt[2]);  rt[2][1] = cos(Rt[0])*sin(Rt[1])*sin(Rt[2]) + sin(Rt[0])*cos(Rt[2]);  rt[2][2] = cos(Rt[0])*cos(Rt[1]);
      double x_sw[3];
      x_sw[0] = msg.x - rt[0][2] * L[3];
      x_sw[1] = msg.y - rt[1][2] * L[3];
      x_sw[2] = msg.z - L[0] - rt[2][2] * L[3];
      double nor_xsw2;
      nor_xsw2 = x_sw[0]*x_sw[0] + x_sw[1]*x_sw[1] + x_sw[2]*x_sw[2];
      double u_sw[3];
      u_sw[0] = x_sw[0]/sqrt(nor_xsw2);
      u_sw[1] = x_sw[1]/sqrt(nor_xsw2);
      u_sw[2] = x_sw[2]/sqrt(nor_xsw2);

      message.joint4 = -1*acos((nor_xsw2 - L[1]*L[1] - L[2]*L[2])/(2*L[1]*L[2]));

      double theta30=0,theta20,theta10,S20,C20,M,N;
      theta10 = atan2( x_sw[1] , x_sw[0] );
      N = sin(message.joint4)*L[2];
      M = cos(message.joint4)*L[2] + L[1];
      S20 = ( M*sqrt( x_sw[0]*x_sw[0] + x_sw[1]*x_sw[1] ) - N*x_sw[2] )/( N*N + M*M );
      C20 = ( N*sqrt( x_sw[0]*x_sw[0] + x_sw[1]*x_sw[1] ) + M*x_sw[2] )/( N*N + M*M );
      theta20 = atan2( S20 , C20 );
      double r[3][3];
      r[2][0] = -1*sin(theta20)*sin(theta30) ;  r[2][1] = -1*cos(theta20) ;  r[2][2] = sin(theta20)*sin(theta30);
      r[1][0] = sin(theta10)*cos(theta20)*cos(theta30) + cos(theta10)*sin(theta30);  r[1][1] = -1*sin(theta10)*sin(theta20);  r[1][2] = -1*sin(theta10)*cos(theta20)*sin(theta30) + cos(theta10)*cos(theta30);
      r[0][0] = cos(theta10)*cos(theta20)*cos(theta30) - sin(theta10)*sin(theta30);  r[0][1] = -1*sin(theta20)*cos(theta10);  r[0][2] = -1*cos(theta10)*cos(theta20)*sin(theta30) - sin(theta10)*cos(theta30);
    
      double As[3][3],Bs[3][3],Cs[3][3];
      As[0][0] = -1*u_sw[2]*r[1][0] + u_sw[1]*r[2][0];
      As[0][1] = -1*u_sw[2]*r[1][1] + u_sw[1]*r[2][1];
      As[0][2] = -1*u_sw[2]*r[1][2] + u_sw[1]*r[2][2];
      As[1][0] = u_sw[2]*r[0][0] - u_sw[0]*r[2][0];
      As[1][1] = u_sw[2]*r[0][1] - u_sw[0]*r[2][1];
      As[1][2] = u_sw[2]*r[0][2] - u_sw[0]*r[2][2];
      As[2][0] = u_sw[0]*r[1][0] - u_sw[1]*r[0][0];
      As[2][1] = u_sw[0]*r[1][1] - u_sw[1]*r[0][1];
      As[2][2] = u_sw[0]*r[1][2] - u_sw[1]*r[0][1];

      Bs[0][0] = -1*u_sw[0]*u_sw[1]*r[1][0] - u_sw[2]*u_sw[0]*r[2][0] + r[0][0]*( u_sw[2]*u_sw[2] + u_sw[1]*u_sw[1] );
      Bs[0][1] = -1*u_sw[0]*u_sw[2]*r[2][1] - u_sw[1]*u_sw[0]*r[1][1] + r[0][1]*( u_sw[2]*u_sw[2] + u_sw[1]*u_sw[1] );
      Bs[0][2] = -1*u_sw[0]*u_sw[2]*r[2][2] - u_sw[1]*u_sw[0]*r[1][2] + r[0][2]*( u_sw[2]*u_sw[2] + u_sw[1]*u_sw[1] );
      Bs[1][0] = -1*u_sw[2]*u_sw[1]*r[2][0] - u_sw[1]*u_sw[0]*r[0][0] + r[1][0]*( u_sw[2]*u_sw[2] + u_sw[0]*u_sw[0] );
      Bs[1][1] = -1*u_sw[1]*u_sw[2]*r[2][1] - u_sw[1]*u_sw[0]*r[0][1] + r[1][1]*( u_sw[2]*u_sw[2] + u_sw[0]*u_sw[0] );
      Bs[1][2] = -1*u_sw[1]*u_sw[2]*r[2][2] - u_sw[1]*u_sw[0]*r[0][2] + r[1][2]*( u_sw[2]*u_sw[2] + u_sw[0]*u_sw[0] );
      Bs[2][0] = -1*u_sw[0]*u_sw[2]*r[0][0] - u_sw[1]*u_sw[2]*r[1][0] + r[2][0]*( u_sw[0]*u_sw[0] + u_sw[1]*u_sw[1] );
      Bs[2][1] = -1*u_sw[0]*u_sw[2]*r[0][1] - u_sw[1]*u_sw[2]*r[1][1] + r[2][1]*( u_sw[0]*u_sw[0] + u_sw[1]*u_sw[1] );
      Bs[2][2] = -1*u_sw[0]*u_sw[2]*r[0][2] - u_sw[1]*u_sw[2]*r[1][2] + r[2][2]*( u_sw[0]*u_sw[0] + u_sw[1]*u_sw[1] );

      Cs[0][0] = u_sw[0]*u_sw[0]*r[0][0] + u_sw[1]*u_sw[0]*r[1][0] + r[2][0]*u_sw[2]*u_sw[0];
      Cs[0][1] = u_sw[0]*u_sw[0]*r[0][1] + u_sw[1]*u_sw[0]*r[1][1] + r[2][1]*u_sw[2]*u_sw[0];
      Cs[0][2] = u_sw[0]*u_sw[0]*r[0][2] + u_sw[1]*u_sw[0]*r[1][2] + r[2][2]*u_sw[2]*u_sw[0];
      Cs[1][0] = u_sw[1]*u_sw[0]*r[0][0] + u_sw[1]*u_sw[1]*r[1][0] + r[2][0]*u_sw[2]*u_sw[1];
      Cs[1][1] = u_sw[0]*u_sw[1]*r[0][1] + u_sw[1]*u_sw[1]*r[1][1] + r[2][1]*u_sw[2]*u_sw[1];
      Cs[1][2] = u_sw[0]*u_sw[1]*r[0][2] + u_sw[1]*u_sw[1]*r[1][2] + r[2][2]*u_sw[2]*u_sw[1];
      Cs[2][0] = u_sw[0]*u_sw[2]*r[0][0] + u_sw[1]*u_sw[2]*r[1][0] + r[2][0]*u_sw[2]*u_sw[2];
      Cs[2][1] = u_sw[0]*u_sw[2]*r[0][1] + u_sw[1]*u_sw[2]*r[1][1] + r[2][1]*u_sw[2]*u_sw[2];
      Cs[2][2] = u_sw[0]*u_sw[2]*r[0][2] + u_sw[1]*u_sw[2]*r[1][2] + r[2][2]*u_sw[2]*u_sw[2];

      double theta01,theta02,theta1,theta21,theta22;
      theta01 = -1*( As[1][1]*sin(psi) + Bs[1][1]*cos(psi) + Cs[1][1] );
      theta02 = -1*( As[0][1]*sin(psi) + Bs[0][1]*cos(psi) + Cs[0][1] );
      theta1  = -1*( As[2][1]*sin(psi) + Bs[2][1]*cos(psi) + Cs[2][1] );
      theta21 = As[2][2]*sin(psi) + Bs[2][2]*cos(psi) + Cs[2][2];
      theta22 = -1*( As[2][0]*sin(psi) + Bs[2][0]*cos(psi) + Cs[2][0] );
      message.joint1 = atan2( theta01 , theta02);
      message.joint2 = acos( theta1 );
      message.joint3 = atan2( theta21 , theta22 );
      if( message.joint2 > M_PI/2 ){
        message.joint2 = message.joint2 - M_PI;
      }

      double Aw[3][3],Bw[3][3],Cw[3][3];
      Aw[0][2] = rt[0][2]*( As[0][0]*cos(L[3]) + As[0][1]*sin(L[3]) ) + rt[1][2]*( As[1][0]*cos(L[3]) + As[1][1]*sin(L[3]) ) + rt[2][2]*( As[2][0]*cos(L[3]) + As[2][1]*sin(L[3]) );
      Aw[1][2] = rt[0][2]*As[0][2]+ rt[1][2]*As[1][2] + rt[2][2]*As[2][2];
      Aw[2][0] = rt[0][0]*( As[0][0]*sin(L[3]) - As[0][1]*cos(L[3]) ) + rt[1][0]*( As[1][0]*sin(L[3]) - As[1][1]*cos(L[3]) ) + rt[2][0]*( As[2][0]*sin(L[3]) + As[2][1]*cos(L[3]) );
      Aw[2][1] = rt[0][1]*( As[0][0]*sin(L[3]) - As[0][1]*cos(L[3]) ) + rt[1][1]*( As[1][0]*sin(L[3]) - As[1][1]*cos(L[3]) ) + rt[2][1]*( As[2][0]*sin(L[3]) + As[2][1]*cos(L[3]) );
      Aw[2][2] = rt[0][2]*( As[0][0]*sin(L[3]) - As[0][1]*cos(L[3]) ) + rt[1][2]*( As[1][0]*sin(L[3]) - As[1][1]*cos(L[3]) ) + rt[2][2]*( As[2][0]*sin(L[3]) + As[2][1]*cos(L[3]) );

      Bw[0][2] = rt[0][2]*( Bs[0][0]*cos(L[3]) + Bs[0][1]*sin(L[3]) ) + rt[1][2]*( Bs[1][0]*cos(L[3]) + Bs[1][1]*sin(L[3]) ) + rt[2][2]*( Bs[2][0]*cos(L[3]) + Bs[2][1]*sin(L[3]) );
      Bw[1][2] = rt[0][2]*Bs[0][2] + rt[1][2]*Bs[1][2] + rt[2][2]*Bs[2][2];
      Bw[2][0] = rt[0][0]*( Bs[0][0]*sin(L[3]) - Bs[0][1]*cos(L[3]) ) + rt[1][0]*( Bs[1][0]*sin(L[3]) - Bs[1][1]*cos(L[3]) ) + rt[2][0]*( Bs[2][0]*sin(L[3]) + Bs[2][1]*cos(L[3]) );
      Bw[2][1] = rt[0][1]*( Bs[0][0]*sin(L[3]) - Bs[0][1]*cos(L[3]) ) + rt[1][1]*( Bs[1][0]*sin(L[3]) - Bs[1][1]*cos(L[3]) ) + rt[2][1]*( Bs[2][0]*sin(L[3]) + Bs[2][1]*cos(L[3]) );
      Bw[2][2] = rt[0][2]*( Bs[0][0]*sin(L[3]) - Bs[0][1]*cos(L[3]) ) + rt[1][2]*( Bs[1][0]*sin(L[3]) - Bs[1][1]*cos(L[3]) ) + rt[2][2]*( Bs[2][0]*sin(L[3]) + Bs[2][1]*cos(L[3]) );

      Cw[0][2] = rt[0][2]*( Cs[0][0]*cos(L[3]) + Cs[0][1]*sin(L[3]) ) + rt[1][2]*( Cs[1][0]*cos(L[3]) + Cs[1][1]*sin(L[3]) ) + rt[2][2]*( Cs[2][0]*cos(L[3]) + Cs[2][1]*sin(L[3]) );
      Cw[1][2] = rt[0][2]*Cs[0][2] + rt[1][2]*Cs[1][2] + rt[2][2]*Cs[2][2];
      Cw[2][0] = rt[0][0]*( Cs[0][0]*sin(L[3]) - Cs[0][1]*cos(L[3]) ) + rt[1][0]*( Cs[1][0]*sin(L[3]) - Cs[1][1]*cos(L[3]) ) + rt[2][0]*( Cs[2][0]*sin(L[3]) + Cs[2][1]*cos(L[3]) );
      Cw[2][1] = rt[0][1]*( Cs[0][0]*sin(L[3]) - Cs[0][1]*cos(L[3]) ) + rt[1][1]*( Cs[1][0]*sin(L[3]) - Cs[1][1]*cos(L[3]) ) + rt[2][1]*( Cs[2][0]*sin(L[3]) + Cs[2][1]*cos(L[3]) );
      Cw[2][2] = rt[0][2]*( Cs[0][0]*sin(L[3]) - Cs[0][1]*cos(L[3]) ) + rt[1][2]*( Cs[1][0]*sin(L[3]) - Cs[1][1]*cos(L[3]) ) + rt[2][2]*( Cs[2][0]*sin(L[3]) + Cs[2][1]*cos(L[3]) );

      double theta41,theta42,theta5,theta61,theta62;
      theta41 = Aw[1][2]*sin(psi) + Bw[1][2]*cos(psi) + Cw[1][2] ;
      theta42 = Aw[0][2]*sin(psi) + Bw[0][2]*cos(psi) + Cw[0][2] ;
      theta5  = Aw[2][2]*sin(psi) + Bw[2][2]*cos(psi) + Cw[2][2] ;
      theta61 = Aw[2][1]*sin(psi) + Bw[2][1]*cos(psi) + Cw[2][1] ;
      theta62 = -1 * ( Aw[2][0]*sin(psi) + Bw[2][0]*cos(psi) + Cw[2][0] );
      message.joint5 = atan2( theta41 , theta42 );
      message.joint6 = acos( theta5 );
      message.joint7 = atan2( theta61 , theta62 );
      if( message.joint6 > M_PI/2 ){
        message.joint6 = message.joint6 - M_PI;
      }
    }
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Calculater>());
  rclcpp::shutdown();
  return 0;
}
