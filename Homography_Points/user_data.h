#pragma once

#include <opencv2/imgproc.hpp>

class User_data {
public:
    bool b_down;
    cv::Point p_move, p_down, p_up;
    cv::Mat img;

    User_data(bool _b_down = false,
              cv::Point _p_move = cv::Point(), cv::Point _p_down = cv::Point(), cv::Point _p_up = cv::Point(),
              cv::Mat _img = cv::Mat());

    ~User_data() = default;
};
