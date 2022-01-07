#ifndef _TOINFERENCE_H
#define _TOINFERENCE_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <net.h>
#include "nanodet.h"
#include <benchmark.h>

using namespace cv;
using namespace std;

struct object_rect {
    int x;
    int y;
    int width;
    int height;
};

class toInference
{
public:
  
    static int resize_uniform(cv::Mat& src, cv::Mat& dst, cv::Size dst_size,object_rect& effect_area);
    static void draw_bboxes(const cv::Mat& bgr, const std::vector<BoxInfo>& bboxes,object_rect& effect_roi,vector<Rect>& rects);

};


#endif