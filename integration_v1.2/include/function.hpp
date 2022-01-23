#ifndef _FUNCTION_H
#define _FUNCTION_H

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>
#include "mv_video_capture.hpp"
#include "serialport.h"
#include "toInference.hpp"

using namespace cv;
class Function
{
    public:
    static int find_mineral(cv::Mat src_img_,const std::vector<BoxInfo>& bboxes,toInference effect_,object_rect& effect_roi,int& is_arrive,int mode);

};

#endif