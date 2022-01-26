#include "function.hpp"

//传入图像，推理容器，推理类，目标位置，是否到达，模式（0为空接，1为资源岛），
int Function::find_mineral(cv::Mat src_img_,const std::vector<BoxInfo>& bboxes ,toInference effect_,object_rect& effect_roi,int& is_arrive,int mode)
{
    cv::Mat img_src = src_img_.clone();
    Mat img = img_src.clone();
    vector<cv::Rect> count;
    
    effect_.draw_bboxes(img, bboxes,effect_roi,count);

    if(mode == 0)//空接 
    {
        
    }
    else if(mode == 1)//资源岛
    {
        if(is_arrive == 1)
        {        
            if(count.size()==1)
            {
                if((count[0].x+count[0].br().x)/2 < 495)//640需要改成转换后的值
                {
                    SerialPort::RMserialWrite((count[0].x+count[0].br().x)/2,0,0,0);
                }
                else if((count[0].x+count[0].br().x)/2 > 505)
                {
                    SerialPort::RMserialWrite((count[0].x+count[0].br().x)/2,0,0,1);
                }
                else  
                {   
                    SerialPort::RMserialWrite(500,0,0,-1);
                    is_arrive = 0;
                }
                cout<<(count[0].x+count[0].br().x)/2<<endl;
                return 1;
            }
            else if(count.size()==0)
            {
                SerialPort::RMserialWrite(500,0,0,-1);
                return -1;
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
                    cout<<(count[0].x+count[0].br().x)/2<<endl;
                    return 1;
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
                }

            }

        }
    
    } 
    imshow(" ",img_src);
}


