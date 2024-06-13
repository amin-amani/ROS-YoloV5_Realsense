# YOLOv5 Object Detection with ROS

## Overview

This repository contains a ROS (Robot Operating System) package for performing real-time object detection using YOLOv5 (You Only Look Once) and the Surena Robot's camera. The package includes a ROS node that 

subscribes to camera image topics and publishes object detection information.

## Prerequisites

Before using this package, you need to ensure the following prerequisites are met:

- [ROS](http://www.ros.org/) is installed.

- YOLOv5 engine file is available.

- Surena Robot with the required sensors and camera setup.


![alt text](https://github.com/amin-amani/ROS-YoloV5_Realsense/blob/main/doc/Screenshot.png)

## Installation

1. Clone this repository to your ROS workspace:

2. Build your ROS workspace:

## gpu informations:

![alt text](https://github.com/amin-amani/ROS-YoloV5_Realsense/blob/main/doc/gpu.png)

## contribution with RealSense camera

roslaunch realsense2_camera rs_camera.launch align_depth:=true




## License

This project is licensed under the [GNU General Public License (GNU GPL)](https://www.gnu.org/licenses/gpl-3.0.en.html). Please see the [LICENSE](LICENSE) file for the full text of the license.

## Acknowledgments

- Thanks to the YOLOv5 community for their contribution to real-time object detection.

- Special thanks to the Surena Robot project for providing a platform for testing.

For more details on YOLOv5, please refer to the official YOLOv5 repository: [YOLOv5 GitHub](https://github.com/ultralytics/yolov5).

For more information on ROS, visit the [ROS website](http://www.ros.org/).

Feel free to contribute and improve this package. Pull requests and issues are welcome!



