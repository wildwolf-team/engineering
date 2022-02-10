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
  int a=0;//模拟等待串口
  int toJudge=0;//只走一次灯识别
  int temp=-1;//进入矿石检测
  int center_x=-1;
  SerialPort serialPort;

  while (1) 
  {
    if (mv_capture_->isindustryimgInput()) {
      src_img_ = mv_capture_->image();
    }
    if(!src_img_.empty())
    {
      // 初始化串口
      // 串口按键选择功能
      // 模拟提前开好摄像头等待
     
      int rm_recive[3];
      SerialPort::RMreceiveData(rm_recive);
      cout<<"recive "<<rm_recive[1]<<endl;
      switch(2)
      {
        // case 1:
        // // 如果说只判断一次的话应该加个判断走一次
        // // 如果只按照指示灯的话再稍微改改
        // if(toJudge == 0)
        // { 
        //   temp = Function::lightAndMineral(src_img_);
          
        //   cout<<"temp  "<<temp<<endl;
        // }
        // 加一个上面判断完后可以一直进入
        // if(temp != -1)
        // {
        //   toJudge++;
        //   center_x = temp;
        //   int result = Function::find_mineral(src_img_,center_x,0);
        //   if(result == 1)
        //   {
        //     toJudge == 0;
        //     temp=-1;
        //   }
        // }
        // break;
        case 2:
        Function::find_mineral(src_img_,0,1);
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

