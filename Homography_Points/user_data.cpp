#include "user_data.h"

using namespace cv;

User_data::User_data(bool _b_down,
                     Point _p_move, Point _p_down, Point _p_up,
                     Mat _img)
    : b_down(_b_down)
    , p_move(_p_move)
    , p_down(_p_down)
    , p_up(_p_up)
    , img(_img)
{}