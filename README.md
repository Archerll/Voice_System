# Voice_System
![C](https://img.shields.io/badge/c-%2300599C.svg?style=for-the-badge&logo=c&logoColor=white)
![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)

基于ros的语音交互系统
------
运行环境：
1.Ubuntu 16.04 （TLS）x64或x86

2.ROS Kinetic Kame

执行：
首先必须安装sox软件，使其可以在命令行下播放音乐。

之后将包中libs中的 .so文件根据电脑的位数，在相应文件夹下将对应的.so文件copy到usr/lib中

sudo cp libmsc.so /usr/lib

将以上准备做完之后即可以执行ros

首先 catkin_make

之后 roscore

最后 source devel/setup.bash  
开启每一个node，和topic

其中xf_asr_topic 是执行语音交互的topic
向xf_asr_topic发送的topic为  rostopic pub -1 voice/xf_asr_topic std_msgs/Int32 1
