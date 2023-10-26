/**
 * @file
 *
 * @author      Amin Amani
 * @brief       YoloV5 example: inference on a single image
 *
 * Copyright (c) 2021, Noah van der Meer
 *
 *
 */

#include "yolov5_detector.hpp"
#include <QCoreApplication>
#include <QDebug>
#include <iostream>
#include <chrono>
#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>
#include <surena_object/DetectionInfo.h>
#include <surena_object/DetectionInfoArray.h>
yolov5::Detector detector;
yolov5::Result r;
cv::Mat image;
cv::Mat Depthimage;
ros::Publisher detection_info_pub;
const cv::Scalar magenta(255, 51, 153);
std::vector<yolov5::Detection> detections;

float getDistance(cv::Rect box)
{
    cv::Rect rr=box;
    cv::Rect rsmall;
    int xc=rr.x+(rr.width/2);
    int yc=rr.y+(rr.height/2);

    rsmall.x=xc-5;
    rsmall.y=yc-5;
    rsmall.width=10;
    rsmall.height=10;
    cv::Scalar color(0, 0, 255);
    return  cv::mean(Depthimage(rsmall))[0];

}
void cameraCallbackDepth(const sensor_msgs::ImageConstPtr& image_msg) {
    try {

        cv_bridge::CvImagePtr cv_ptr = cv_bridge::toCvCopy(image_msg, image_msg->encoding);
        Depthimage = cv_ptr->image;
        // cv::imshow("Camera depth", Depthimage);
        //cv::waitKey(1);
    } catch (cv_bridge::Exception& e) {
        ROS_ERROR("cv_bridge exception: %s", e.what());
    }
}

void visualizeAndPopulateDetection( const yolov5::Detection& detection, cv::Mat& image,
                                    std::vector<surena_object::DetectionInfo>& detections_array, cv::Scalar magenta)
{

    yolov5::visualizeDetection(detection, &image, magenta, 1.0);


    float distance = getDistance(detection.boundingBox());
    cv::Scalar textColor(0, 0, 255);
    cv::putText(image, std::to_string(distance),
                cv::Point(detection.boundingBox().x + 20, detection.boundingBox().y + 20),
                cv::FONT_HERSHEY_SIMPLEX, 1.0, textColor, 3);


    surena_object::DetectionInfo detection_msg;
    detection_msg.distance = distance;
    detection_msg.class_id = detection.classId();
    detection_msg.x = detection.boundingBox().x;
    detection_msg.y = detection.boundingBox().y;
    detection_msg.height = detection.boundingBox().height;
    detection_msg.width = detection.boundingBox().width;

    detections_array.push_back(detection_msg);
}

void cameraCallback(const sensor_msgs::ImageConstPtr& image_msg) {
    try {
        cv_bridge::CvImagePtr cv_ptr = cv_bridge::toCvCopy(image_msg, sensor_msgs::image_encodings::BGR8);
        image = cv_ptr->image;
        r = detector.detect(image, &detections, yolov5::INPUT_BGR);

        if (r != yolov5::RESULT_SUCCESS) {
            std::cout << "detect() failed: " << yolov5::result_to_string(r) << std::endl;
            return;
        }

        surena_object::DetectionInfoArray detection_array_msg;
        std::vector<surena_object::DetectionInfo> detections_array;

        for (unsigned int i = 0; i < detections.size(); ++i) {
            visualizeAndPopulateDetection(detections[i], image, detections_array, magenta);
        }

        detection_array_msg.detections = detections_array;
        detection_info_pub.publish(detection_array_msg);

        cv::imshow("Camera Feed", image);
        cv::waitKey(1);
    } catch (cv_bridge::Exception& e) {
        ROS_ERROR("cv_bridge exception: %s", e.what());
    }
}

int main(int argc, char** argv) {
    // Initialize the ROS node
    ros::init(argc, argv, "camera_subscriber");
    ros::NodeHandle nh;
    detection_info_pub = nh.advertise<surena_object::DetectionInfoArray>("/detection_info", 1);
    r = detector.init();
    if(r != yolov5::RESULT_SUCCESS)
    {
        std::cout << "init() failed: " << yolov5::result_to_string(r)
                  << std::endl;
        return 1;
    }
    r = detector.loadEngine("/home/surenav/Yolo-TensorRT-UBUNTU-main/models/yolov5.engine");
    if(r != yolov5::RESULT_SUCCESS)
    {
        std::cout << "loadEngine() failed: " << yolov5::result_to_string(r)
                  << std::endl;
        return 1;
    }
    image=cv::imread("/home/surenav/Yolo-TensorRT-UBUNTU-main/zidane.png");
    if(image.empty())
    {
        std::cout << "Failed to load input image" << std::endl;
        return 1;
    }
    detector.detect(image, nullptr);
    detector.detect(image, nullptr);


    ros::Subscriber sub = nh.subscribe("/camera/color/image_raw", 1, cameraCallback);
    ros::Subscriber sub2 = nh.subscribe("/camera/depth/image_rect_raw", 1, cameraCallbackDepth);


    cv::namedWindow("Camera Feed", cv::WINDOW_AUTOSIZE);


    ros::spin();


    cv::destroyWindow("Camera Feed");

    return 0;
}
