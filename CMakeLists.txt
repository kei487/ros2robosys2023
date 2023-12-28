cmake_minimum_required(VERSION 3.8)
project(ros2cpprbsys)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)

add_executable(talker
  src/talker.cpp
)
ament_target_dependencies(talker
  rclcpp
  geometry_msgs
)
add_executable(listener
  src/listener.cpp
)
ament_target_dependencies(listener
  rclcpp
  geometry_msgs
)

install(TARGETS
  talker
  DESTINATION lib/${PROJECT_NAME}/
)
install(TARGETS
  listener
  DESTINATION lib/${PROJECT_NAME}/
)
ament_package()
