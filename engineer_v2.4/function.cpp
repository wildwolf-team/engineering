#include "function.hpp"

//传入图像，微调方向，模式（0为空接，1为资源岛）
int Function::find_mineral(cv::Mat src_img_,int center_x ,int num )
{
    cv::Mat img_src = src_img_.clone();
    Mat img = img_src.clone();
    cv::cvtColor(img,img,CV_BGR2HSV);
    // 矿石参数未给用了会出错
    // 曝光1200 ， 补偿 166, 207, 218
    // 9~10
    // cv::inRange(img,cv::Scalar(8,77,127),cv::Scalar(77,117,255),img);
    // 10～12
    // cv::inRange(img,cv::Scalar(10,56,132),cv::Scalar(86,107,236),img);
    // 14~15
    // cv::inRange(img,cv::Scalar(9,40,85),cv::Scalar(29,97,255),img);
    // 16~17
    // cv::inRange(img,cv::Scalar(10,57,29),cv::Scalar(42,94,72),img);
    // 曝光10000
    // 开场地灯
    // cv::inRange(img,cv::Scalar(17,52,34),cv::Scalar(41,104,77),img);
    // 15.办公室
    cv::inRange(img,cv::Scalar(14,104,47),cv::Scalar(23,149,40),img);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(img,contours,0,1);
    std::vector<cv::Rect> count;
    for (size_t j = 0; j < contours.size(); j++)  
    {    
        cv::Rect r = cv::boundingRect(contours[j]);
        if(r.area()>5000)
        {
            
            count.push_back(r);
            cv::rectangle(img_src, r, cv::Scalar(255, 0, 255), 2);
            
                
        }
    }
    // imshow(" ",img_src);
    // 感觉直接用资源岛模式就可以完成所有
    if(num == 0)//空接 
    {
        // 因为是一直朝一个方向移动所以只要符合其中一个就行了，除非车开的位置有问题，不行的话加一个判断位置的（类似资源岛）
        // 怕直接开过了但是感觉加了也没用，给个范围比较好
        for(int i=0;i<count.size();i++)
        {
            // 给范围更好
            // std::cout<<"center_x  "<<center_x<<std::endl;//发送的数据
            // std::cout<<"center_  "<<center_<<std::endl;
            // std::cout<<"now  "<<(count[i].x+count[i].br().x)/2<<std::endl;
            // cv::imshow(" ",img);
            int target_x = center_x>center_?1:0;
            SerialPort::RMserialWrite(center_x,0,0,target_x);
            if(center_ == (count[i].x+count[i].br().x)/2)
            {
                //停止
                SerialPort::RMserialWrite(0,0,0,-1);
                std::cout<<"stop"<<std::endl;
                return 1;
            }

        }
        return -1;
    }
    else if(num == 1)//资源岛
    {
        if(count.size()==1)
        {
            if((count[0].x+count[0].br().x)/2<640)//640需要改成转换后的值
            {
                SerialPort::RMserialWrite((count[0].x+count[0].br().x)/2,0,0,0);
            }
            else if((count[0].x+count[0].br().x)/2>640)
            {
                SerialPort::RMserialWrite((count[0].x+count[0].br().x)/2,0,0,1);
            }
            else
            {
                SerialPort::RMserialWrite((count[0].x+count[0].br().x)/2,0,0,-1);
            }
            cout<<(count[0].x+count[0].br().x)/2<<endl;
        }
        else if(count.size()==0)
        {
            return -1;
        }
        else
        {
            int temp=img.rows,num_index=-1;
            for(int i=0;i<count.size();i++)
            {
                if(abs((count[i].x+count[i].br().x)/2 - 640)<temp)//640记得改
                {
                    temp = abs((count[i].x+count[i].br().x)/2 - 640);
                    num_index = i;
                }
            }
            if(num_index != -1)
            {   
                rectangle(img_src, count[num_index], cv::Scalar(0, 0, 255), 2);
                cout<<(count[num_index].x+count[num_index].br().x)/2<<endl;
                if((count[num_index].x+count[num_index].br().x)/2<640)
                {
                    SerialPort::RMserialWrite((count[num_index].x+count[num_index].br().x)/2,0,0,0);
                }
                else if((count[num_index].x+count[num_index].br().x)/2>640)
                {
                    SerialPort::RMserialWrite((count[num_index].x+count[num_index].br().x)/2,0,0,1);
                }
                else
                {
                    SerialPort::RMserialWrite((count[num_index].x+count[num_index].br().x)/2,0,0,-1);
                }

            }
            else
            {
                return -1;
            }
            
        }
        
    
    }
    else
    {
        return -1;
    }   
    imshow(" ",img_src);
}


//有问题的话就应该是判断的问题了
// 想不要这个东西了
cv::Mat previousframe;
int framenum = 0;
int is_run=0;
// 有一个问题就是阈值的问题，灯一直是有的（不管闪不闪）多盏灯可能就会出错
int Function::lightAndMineral(cv::Mat src_img_)
{
	cv::Mat tempframe, currentframe;
		
    tempframe = src_img_;
    framenum++;
    cv::blur(tempframe,tempframe,cv::Size(5,5));
    if (framenum == 1)
    {
        cv::cvtColor(tempframe, previousframe, CV_BGR2GRAY);
    }
    else
    {
        cv::cvtColor(tempframe, currentframe, CV_BGR2GRAY);  
        absdiff(currentframe, previousframe, currentframe);  
        cv::threshold(currentframe, currentframe, 80, 255.0, CV_THRESH_BINARY);
   
        std::vector<std::vector<cv::Point>> contours;  
        cv::Rect re;
        std::vector<cv::Rect> re_re;
        findContours(currentframe,contours,0,1);
        for(auto i=0;i<contours.size();i++)
        {
            re = boundingRect(contours[i]);
            re_re.push_back(re);
            rectangle(tempframe,re,cv::Scalar(255,0,255));
        }
        // 判断逻辑有点问题，到时候调试找到最优
        // 工业相机亮度低的话应该可以直接判断（多盏灯的话应该会有问题）
        imshow("当前视频帧", tempframe);
        imshow("背景减图后区域",currentframe);
        if(!re_re.empty())
        {
            if(re_re.size()==1)
            {   
                std::cout<<"x  "<< re_re[0].x <<std::endl;
                std::cout<<"xbr  "<<  re_re[0].br().x<<std::endl;
                std::cout<<"xbr_all  "<<  (re_re[0].x + re_re[0].br().x)/2<<std::endl;
                int center_x = (re_re[0].x + re_re[0].br().x)/2;
                
                framenum = 0;	                                                                 
                return center_x;  
            }
            else
            {
                return -1;
            }    
            //只找一帧,后面退出是直接结束
            // std::cout<<"finding"<<std::endl;
            // if(is_run == 0)
            // {
            //     if(re_re.size() == 1 )
            //     {
            //         is_run++;
            //     }
            //     else
            //     {
            //         return -1;
            //     }				
            // }
            // else
            // 改成 if(is_run == 1)会不会好点后面可以试试
            // {
            //     //灯条的位置判断左右，只能有一个框
            //     int center_x = (re_re[0].x + re_re[0].br().x)/2;
            //     framenum = 0;
            //     is_run=0;
            //     return center_x;           
            // }
        }
        else
        {
            return -1;
        }   
    }  
}

// int Function::lightAndLight()
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
//                 //灯条的位置判断左右，只能有一个框
//                 int center_x = (re.x + re.br().x)/2;
//                 // 串口发送数据
//                 // wirte();
//                 if(1);//停止
            
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
