#ifndef _FUNCTION_H
#define _FUNCTION_H

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>
#include "mv_video_capture.hpp"
#include "serialport.h"

#define center_ 200

class Function
{
    public:
    static int find_mineral(cv::Mat src_img_,int center_x,int is_arrive,int num);
    static int lightAndLight(Mat& tempframe,Mat& currentframe,Mat& previousframe,Mat& frame,vector<int> last_set,int framenum);
};

// // 传统视觉版
// // 如果数据突然变动大会有影响那就再想想办法（手动改变数值）
// // inrange不给数据会显示不出图像
// // 这代码基本上就稳了，要搞一下矿石的参数
// int find_mineral(int center_re)
// {
// 	cv::VideoCapture capture(0);
//     while(1)
//      { 
//         cv::Mat img;
//         capture >> img;
//         cv::cvtColor(img,img,CV_BGR2HSV);
//         cv::inRange(img,cv::Scalar(0,0,0),cv::Scalar(100,100,100),img);
//         std::vector<std::vector<cv::Point>> contours;
//         cv::findContours(img,contours,1,1);
    
//         std::vector<cv::Rect> count;
//         for (size_t j = 0; j < contours.size(); j++)  
//         {    
//             cv::Rect r = cv::boundingRect(contours[j]);
//             cv::rectangle(img, r, cv::Scalar(0x27, 0xC1, 0x36), 2);
//             count.push_back(r);
//         }
//         if(count.size() == 0) continue;
//         for(int i=0;i<count.size();i++)
//         {
//             // 给范围更好
//             std::cout<<"center_re  "<<center_re<<std::endl;//发送的数据
//             std::cout<<"center  "<<center<<std::endl;
//             std::cout<<"now  "<<(count[i].x+count[i].br().x)/2<<std::endl;
//             cv::imshow(" ",img);
//             if(center == (count[i].x+count[i].br().x)/2)
//             {
//                 //停止
//                 // write();
//                 std::cout<<"stop"<<std::endl;
//                 return 1;
//             }
//         }
//         if(cv::waitKey(1)=='q')
//         {
//             return 0;
//         }	
//     }
// }
// //只找到灯条位置信息，一帧，还得稍微调一下预处理，噪点影响
// int lightAndMineral()
// {
//     //读入视频  
// 	// cv::VideoCapture capture("/home/oyc/视频/Kazam_screencast_00004.mp4");
// 	cv::VideoCapture capture(0);
// 	cv::Mat tempframe, currentframe, previousframe;
// 	cv::Mat frame;
// 	int framenum = 0;
// 	int is_run=0;

// 	while (true)
// 	{
// 		tempframe = capture.read(frame);  
// 		capture >> frame;
// 		if(frame.empty())
// 		{
// 			break;
// 		}
// 		tempframe = frame;
// 		framenum++;
// 		cv::blur(tempframe,tempframe,cv::Size(5,5));
// 		if (framenum == 1)
// 		{
// 			cv::cvtColor(tempframe, previousframe, CV_BGR2GRAY);
// 		}
// 		else
// 		{
// 			cv::cvtColor(tempframe, currentframe, CV_BGR2GRAY);  
//  			absdiff(currentframe, previousframe, currentframe);  
//   			cv::threshold(currentframe, currentframe, 80, 255.0, CV_THRESH_BINARY);
//             //显示图像 
// 			imshow("当前视频帧", tempframe);
//             imshow("背景减图后区域",currentframe);	  
//             //进行对位
//             std::vector<std::vector<cv::Point>> contours;  
//             cv::Rect re;
//             std::vector<cv::Rect> re_re;
//             findContours(currentframe,contours,0,1);
//             for(auto i=0;i<contours.size();i++)
//             {
//                 re = boundingRect(contours[i]);
//                 re_re.push_back(re);
//                 rectangle(tempframe,re,cv::Scalar(255,0,255));
//             }
//             //只找一帧,后面退出是直接结束
//             if(is_run == 0)
// 			{
// 				if(re_re.size() == 1 )
// 				{
// 					is_run++;
// 				}
// 				else
// 				{	
//                     continue;
// 				}						
// 		    }
// 		    else
// 		    {
// 			  //灯条的位置判断左右，只能有一个框
// 			    int center_re = (re.x + re.br().x)/2;   
//                 capture.release();
//                 return center_re;           
//             }  
//         }
//         if(cv::waitKey(20)=='q')
//         {
//             return -1;
//         }
           
//     }  
// }
 
// int LightAndLight()
// {
//     cv::VideoCapture capture(0);
//     cv::Mat tempframe, currentframe, previousframe;
// 	cv::Mat frame;
// 	int framenum = 0;
// 	int is_run=0;
//     while(1)
//     {
//         tempframe = capture.read(frame);  
// 		capture >> frame;
// 		if(frame.empty())
// 		{
// 			break;
// 		}
// 		tempframe = frame;
// 		framenum++;
// 		blur(tempframe,tempframe,cv::Size(5,5));
// 		if (framenum == 1)
// 		{
// 			cvtColor(tempframe, previousframe, CV_BGR2GRAY);
// 		}
//         else
//         {
//             cvtColor(tempframe, currentframe, CV_BGR2GRAY);  
//  			absdiff(currentframe, previousframe, currentframe);  
//   			threshold(currentframe, currentframe, 80, 255.0, CV_THRESH_BINARY);
// 			  	//显示图像 
// 			imshow("当前视频帧", tempframe);
// 			imshow("背景减图后区域",currentframe);	

//             std::vector<std::vector<cv::Point>> contours;  
// 		    cv::Rect re;
// 			std::vector<cv::Rect> re_re;
// 			findContours(currentframe,contours,0,1);
// 			for(auto i=0;i<contours.size();i++)
// 			{
// 				re = boundingRect(contours[i]);
// 				re_re.push_back(re);
// 				rectangle(tempframe,re,cv::Scalar(255,0,255));
// 			}
//             if(re_re.size() == 1 )
//             {
//             //灯条的位置判断左右，只能有一个框
//             int center_re = (re.x + re.br().x)/2;
//             // 串口发送数据
//             // wirte();
//             }
//             else
//             {
//                 continue;
//             }
//             if(cv::waitKey(20)=='q')
//             {
//                 return 0;
//             }
//         }
            
//     }
// }


#endif