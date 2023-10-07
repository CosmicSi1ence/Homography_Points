#pragma once

#include "user_data.h"

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#define WINDOW_Y_SHIFT      (65)
#define WINDOW_X_SHIFT      (0)
#define MARK_COLOR          (Scalar(0, 255, 0))	//  B G R
#define RECT_THICKNESS      (2)					//	Rectangle border thickness
#define CIRCLE_THICKNESS    (20)				//	Point size

static int16_t img_size;	
static uint16_t roi_size = 50;
static uint8_t point_cnt = 0;
static bool in_roi = false;
static cv::Mat roi;
static cv::Point roi_top_left;

void callBackFuncSource(int event, int x, int y, int flags, void* u_data);
