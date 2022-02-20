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
  int is_arrive = 0;

  while (1) 
  {
    if (mv_capture_->isindustryimgInput()) 
    {
      src_img_ = mv_capture_->image();
    }
    if(!src_img_.empty())
    {
      switch(2)
      {
        case 2:
        Function::find_mineral(src_img_,is_arrive);
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

 