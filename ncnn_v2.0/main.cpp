#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <net.h>
#include "nanodet.h"
#include <benchmark.h>
#include "toInference.hpp"


// struct object_rect {
//     int x;
//     int y;
//     int width;
//     int height;
// };

int webcam_demo(NanoDet& detector, int cam_id)
{
    cv::Mat image;
    cv::VideoCapture cap(cam_id);
    int height = detector.input_size[0];
    int width = detector.input_size[1];
    toInference effect_;
    object_rect effect_roi;
    while (true)
    {
        cap >> image;
        // double t = (double)cv::getTickCount();//开始计时
        cv::Mat resized_img;
        effect_.resize_uniform(image, resized_img, cv::Size(width, height),effect_roi);
        // std::cout<<"x1 "<<effect_.x<<std::endl;
        // std::cout<<"y1 "<<effect_.y<<std::endl;
        // std::cout<<"w1 "<<effect_.width<<std::endl;
        // std::cout<<"h1 "<<effect_.height<<std::endl;
        auto results = detector.detect(resized_img, 0.4, 0.5);
        effect_.draw_bboxes(image, results,effect_roi);
        // std::cout<<"x2 "<<effect_.x<<std::endl;
        // std::cout<<"y2 "<<effect_.y<<std::endl;
        // std::cout<<"w2 "<<effect_.width<<std::endl;
        // std::cout<<"h2 "<<effect_.height<<std::endl;
        // t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();//结束计时
        // int fps = int(1.0 / t);//转换为帧率
        // std::cout << "FPS: " << fps<<std::endl;//输出帧率
        cv::waitKey(1);
    }
    return 0;
}


int main(int argc, char** argv)
{
    
    NanoDet detector = NanoDet("./nanodet.param", "./nanodet.bin", true);
    webcam_demo(detector, 0);
      
}
