#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>
#include "function.hpp"
#include "serialport.h"
#include "mv_video_capture.hpp"
using namespace std;

int main()
{
	mindvision::VideoCapture* mv_capture_ = new mindvision::VideoCapture(mindvision::CameraParam(0, mindvision::RESOLUTION_1280_X_800, mindvision::EXPOSURE_1200));
  cv::Mat                   src_img_;
  cv::Mat tempframe, currentframe, previousframe;
	cv::Mat frame;
  int framenum = 0;
  vector <int> last_set;
  int is_arrive = 1;
  SerialPort serialPort;

  while (1) 
  {
    if (mv_capture_->isindustryimgInput()) {
      src_img_ = mv_capture_->image();
    }
    if(!src_img_.empty())
    {
     
      int rm_recive[3];
      SerialPort::RMreceiveData(rm_recive);
      cout<<"recive  "<<rm_recive[1]<<endl;
      switch(2)
      {
       
        case 2:
        Function::find_mineral(src_img_,0,is_arrive,1);
        break;
        case 3:
        tempframe = src_img_.clone();
        blur(tempframe,tempframe,cv::Size(5,5));
        framenum++;
        if(framenum == 1)
        {
          cvtColor(tempframe, previousframe, CV_BGR2GRAY);
        }
        else
        {
          Function::lightAndLight(tempframe,currentframe,previousframe,frame,last_set,framenum);
        }
        break;

      }
      // imshow("1",src_img_);
    }
    if (cv::waitKey(10) == 'q') 
    {
      return 0;
    }
    mv_capture_->cameraReleasebuff();
  }
}

