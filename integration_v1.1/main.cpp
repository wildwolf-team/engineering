// #include "toInference.hpp"

// int webcam_demo(NanoDet& detector, int cam_id)
// {
//     cv::Mat image;
//     cv::VideoCapture cap(cam_id);
//     int height = detector.input_size[0];
//     int width = detector.input_size[1];
//     toInference effect_;
//     object_rect effect_roi;
//     while (true)
//     {
//         cap >> image;
//         double t = (double)cv::getTickCount();//开始计时
//         cv::Mat resized_img;
//         effect_.resize_uniform(image, resized_img, cv::Size(width, height),effect_roi);
//         auto results = detector.detect(resized_img, 0.4, 0.5);
//         effect_.draw_bboxes(image, results,effect_roi);
//         t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();//结束计时
//         int fps = int(1.0 / t);//转换为帧率
//         std::cout << "FPS: " << fps<<std::endl;//输出帧率
//         if(cv::waitKey(1) == 'q')
//         {
//             break;
//         }
//     }
//     return 0;
// }


// int main(int argc, char** argv)
// {
    
//     NanoDet detector = NanoDet("./nanodet.param", "./nanodet.bin", true);
//     webcam_demo(detector, 0);
      
// }

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>
#include "function.hpp"
#include "serialport.h"
#include "mv_video_capture.hpp"
using namespace std;

int main()
{
	mindvision::VideoCapture* mv_capture_ = new mindvision::VideoCapture(mindvision::CameraParam(0, mindvision::RESOLUTION_1280_X_800, mindvision::EXPOSURE_0000));
  cv::Mat                   src_img_;
  SerialPort serialPort;
  toInference effect_;
  object_rect effect_roi;
  NanoDet detector = NanoDet("./nanodet.param", "./nanodet.bin", true);
  int height = detector.input_size[0];
  int width = detector.input_size[1];

  while (1) 
  {
    if (mv_capture_->isindustryimgInput()) {
      src_img_ = mv_capture_->image();
    }
    if(!src_img_.empty())
    {
      int rm_recive[3];
      SerialPort::RMreceiveData(rm_recive);
      cout<<"recive "<<rm_recive[1]<<endl;
      switch(2)
      {
        case 2:
        cv::Mat resized_img;
        effect_.resize_uniform(src_img_, resized_img, cv::Size(width, height),effect_roi);
        auto results = detector.detect(resized_img, 0.4, 0.5);
        Function::find_mineral(src_img_,results,effect_,effect_roi,1);
        break;

      }
      imshow("1",src_img_);
    }
    
    if (cv::waitKey(1) == 'q') 
    {
      return 0;
    }
    mv_capture_->cameraReleasebuff();
  }
}

