#include "function.hpp"

//传入图像，是否到达目标值（0未到达，1已到达）
int Function::find_mineral(cv::Mat src_img_,int& is_arrive)
{
    Mat img_src = src_img_.clone();
    Mat img = img_src.clone();
    cvtColor(img,img,CV_BGR2HSV);
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
    inRange(img,Scalar(14,104,47),Scalar(23,149,40),img);

    vector<vector<Point>> contours;
    findContours(img,contours,0,1);
    vector<Rect> mineral_Rects;
    for(size_t i = 0 ;i < contours.size();i++)
    {
        Rect mineral_Rect = boundingRect(contours[i]);
        if(mineral_Rect.area() > 5000 && MIN(mineral_Rect.x,mineral_Rect.y)/MAX(mineral_Rect.x,mineral_Rect.y) > 0.9)
        {
            mineral_Rects.push_back(mineral_Rect);
            rectangle(img_src,mineral_Rect,Scalar(255,0,255),2);

        }
    }
// 判断是否到达目标点
    if(is_arrive == 0)
    {
        // 矿石个数为0
        if(mineral_Rects.size() == 0)
        {

        }                                         
        // 矿石个数为1
        else if(mineral_Rects.size() == 1)
        {
            // 中心点 X值位置
            int mineral_center = (mineral_Rects[0].x + mineral_Rects[0].br().x)/2;
            // 向左
            if(mineral_center < min_limit)
            {
                SerialPort::RMserialWrite(mineral_center,0,0,0);
            }
            // 向右
            else if(mineral_center > max_limit)
            {
                SerialPort::RMserialWrite(mineral_center,0,0,1);
            }
            // 到达
            else
            {
                SerialPort::RMserialWrite(target_point,0,0,-1);
                is_arrive = 1;
            }
        }
        // 有多个矿石
        else 
        {
            // 最优矿石数据，索引
            int mineral_optimal = img.rows;
            int mineral_index = -1;
            // 筛选最优矿石
            for(int i = 0;i < mineral_Rects.size();i++)
            {
                int mineral_center = (mineral_Rects[i].x + mineral_Rects[i].br().x)/2;
                if(abs(mineral_center - target_point) < mineral_optimal)
                {
                    mineral_optimal = abs(mineral_center - mineral_center);
                    mineral_index = i;
                }
            }
            // 发送最优矿石数据
            if(mineral_index != -1)
            {
                int mineral_center = (mineral_Rects[mineral_index].x+mineral_Rects[mineral_index].br().x)/2;
                rectangle(img_src,mineral_Rects[mineral_index],Scalar(0,0,255),2);
                cout<<mineral_center<<endl;
                if(mineral_center < min_limit)
                {
                    SerialPort::RMserialWrite(mineral_center,0,0,0);
                }
                else if(mineral_center >max_limit)
                {
                    SerialPort::RMserialWrite(mineral_center,0,0,1);

                }
                else
                {
                    SerialPort::RMserialWrite(target_point,0,0,-1);
                    is_arrive = 1;
                }
            }
            
        }
    }
    // 处理抖动问题
    else
    {
        SerialPort::RMserialWrite(target_point,0,0,-1);
        if(mineral_Rects.size() == 0)
        {
            
        }
        else if(mineral_Rects.size() == 1)
        {
            int mineral_center = (mineral_Rects[0].x + mineral_Rects[0].br().x)/2;
            if(abs(mineral_center - target_point) > 20)
            {
                is_arrive = 0;
                cout<<mineral_center<<endl;
            }   
        }
        else
        {
            int mineral_optimal = img.rows;
            int mineral_index = -1;
            for(int i = 0;i < mineral_Rects.size();i++)
            {
                int mineral_center = (mineral_Rects[i].x + mineral_Rects[i].br().x)/2;
                if(abs(mineral_center - target_point) < mineral_optimal)
                {
                    mineral_optimal = abs(mineral_center - mineral_center);
                    mineral_index = i;
                }
            }
            if(mineral_index != -1)
            {
                int mineral_center = (mineral_Rects[mineral_index].x+mineral_Rects[mineral_index].br().x)/2;
                if(abs((mineral_Rects[mineral_index].x+mineral_Rects[mineral_index].br().x)/2 - target_point) > 20)
                {
                    is_arrive = 0;
                    cout<<mineral_center<<endl;
                }
            }
        }
    }


    imshow(" ",img_src);
}
