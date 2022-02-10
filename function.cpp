#include "function.hpp"

//传入图像，微调方向，模式（0为空接，1为资源岛）
int Function::find_mineral(cv::Mat src_img_,int center_x ,int is_arrive,int num )
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
    cv::inRange(img,cv::Scalar(17,52,34),cv::Scalar(41,104,77),img);

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

    if(num == 0)//空接 
    {


    }
    else if(num == 1)//资源岛
    {
        if(is_arrive == 1)
        {
            if(count.size()==1)
            {
                if((count[0].x+count[0].br().x)/2<495)//640需要改成转换后的值
                {
                    SerialPort::RMserialWrite((count[0].x+count[0].br().x)/2,0,0,0);
                }
                else if((count[0].x+count[0].br().x)/2>505)
                {
                    SerialPort::RMserialWrite((count[0].x+count[0].br().x)/2,0,0,1);
                }
                else
                {
                    SerialPort::RMserialWrite(500,0,0,-1);
                    is_arrive = 0 ;
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
                    if(abs((count[i].x+count[i].br().x)/2 - 495)<temp)//640记得改
                    {
                        temp = abs((count[i].x+count[i].br().x)/2 - 495);
                        num_index = i;
                    }
                }
                if(num_index != -1)
                {   
                    rectangle(img_src, count[num_index], cv::Scalar(0, 0, 255), 2);
                    cout<<(count[num_index].x+count[num_index].br().x)/2<<endl;
                    if((count[num_index].x+count[num_index].br().x)/2<495)
                    {
                        SerialPort::RMserialWrite((count[num_index].x+count[num_index].br().x)/2,0,0,0);
                    }
                    else if((count[num_index].x+count[num_index].br().x)/2>505)
                    {
                        SerialPort::RMserialWrite((count[num_index].x+count[num_index].br().x)/2,0,0,1);
                    }
                    else
                    {
                        SerialPort::RMserialWrite(500,0,0,-1);
                        is_arrive = 0;
                    }

                }

                
            }
        }
        else
        {
            SerialPort::RMserialWrite(500,0,0,-1);
            if(count.size()==1)
            {
                if(abs((count[0].x+count[0].br().x)/2 - 500) > 15)
                is_arrive = 1;
                cout<<(count[0].x+count[0].br().x)/2<<endl;

            }
            else if(count.size()==0)
            {

            }
            else
            {
                int temp=img.rows,num_index=-1;
                for(int i=0;i<count.size();i++)
                {
                    if(abs((count[i].x+count[i].br().x)/2 - 500)<temp)//640记得改
                    {
                        temp = abs((count[i].x+count[i].br().x)/2 - 500);
                        num_index = i;
                    }
                }
                if(num_index != -1)
                {
                    if(abs((count[num_index].x+count[num_index].br().x)/2 - 500) > 15)
                    is_arrive = 1;
                    cout<<(count[num_index].x+count[num_index].br().x)/2<<endl;
                }

            }
                    

        }
    
    }
  
    imshow(" ",img_src);
}

// 如果有多个灯条会被识别应该要加最优灯条
int Function::lightAndLight(Mat& tempframe,Mat& currentframe,Mat& previousframe,Mat& frame,vector<int> last_set,int framenum)
{

    
        cvtColor(tempframe, currentframe, CV_BGR2GRAY);  
        absdiff(currentframe, previousframe, currentframe);  
        threshold(currentframe, currentframe, 100, 255.0, CV_THRESH_BINARY);
        cvtColor(tempframe, previousframe, CV_BGR2GRAY);  

        std::vector<std::vector<cv::Point>> contours;  
        cv::Rect re;
        std::vector<cv::Rect> re_re;
        findContours(currentframe,contours,0,1);
        for(auto i=0;i<contours.size();i++)
        {
            re = boundingRect(contours[i]);
            if(re.area()>1000)
            {
                re_re.push_back(re);
                rectangle(tempframe,re,cv::Scalar(255,0,255));
            }
        }
        if(re_re.size() == 0)
        {   
            if(last_set.size()==0)
            SerialPort::RMserialWrite(500,0,0,-1);
            else if(last_set.size()!=0 && last_set[last_set.size()-1] < 495)
            SerialPort::RMserialWrite(last_set[last_set.size()-1],0,0,0);
            else if(last_set.size()!=0 && last_set[last_set.size()-1] > 505)
            SerialPort::RMserialWrite(last_set[last_set.size()-1],0,0,0);
        }
        else if(re_re.size() == 1 )
        {
            // 串口发送数据
            if((re_re[0].x+re_re[0].br().x)/2 < 495)
            {
                SerialPort::RMserialWrite((re_re[0].x+re_re[0].br().x)/2,0,0,0);
                last_set.push_back((re_re[0].x+re_re[0].br().x)/2);
            }
            else if((re_re[0].x+re_re[0].br().x)/2 > 505)
            {
                SerialPort::RMserialWrite((re_re[0].x+re_re[0].br().x)/2,0,0,1);
                last_set.push_back((re_re[0].x+re_re[0].br().x)/2);
            }
            else if((re_re[0].x+re_re[0].br().x)/2 >= 495 && (re_re[0].x+re_re[0].br().x)/2 <= 505)
            {
                SerialPort::RMserialWrite(500,0,0,-1);
                last_set.push_back(500);
            }
        }
        else
        {
            SerialPort::RMserialWrite(500,0,0,-1);
        }
        imshow("当前视频帧", tempframe);
        imshow("背景减图后区域",currentframe);	

    
            
    
}
