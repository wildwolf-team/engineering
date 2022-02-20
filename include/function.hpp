#ifndef _FUNCTION_H
#define _FUNCTION_H

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>
#include "mv_video_capture.hpp"
#include "serialport.h"

#define min_limit 495
#define max_limit 505
#define target_point 500

class Function
{
    public:
    static int  find_mineral(cv::Mat src_img_,int& is_arrive);

};

#endif