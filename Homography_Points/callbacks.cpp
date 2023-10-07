#include "callbacks.h"

using namespace cv;

void callBackFuncSource(int event, int x, int y, int flags, void* u_data) {
    User_data* data = (User_data*)u_data;   //  get data from parameter

    switch (event) {
        case EVENT_LBUTTONDOWN: {
            (*data).p_down = Point(x, y);   //  save the point
            img_size = (*data).img.cols;

            if (!in_roi) {  //  global image case
                in_roi = true;
                ++point_cnt;

                roi_top_left = (*data).p_down;
                Point roi_bottom_right = Point(roi_top_left.x + roi_size, roi_top_left.y + roi_size);
                //  boundary conditions:
                if (roi_bottom_right.x > (*data).img.cols) {
                    roi_top_left.x = (*data).img.cols - roi_size;
                }
                if (roi_bottom_right.y > (*data).img.rows) {
                    roi_top_left.y = (*data).img.rows - roi_size;
                }

                roi = Mat((*data).img, Rect(roi_top_left, Point(roi_top_left.x + roi_size, roi_top_left.y + roi_size)));
                resize(roi, roi, Size(img_size, img_size));
                imshow("img", roi);
            }
            else {  //  ROI case
                in_roi = false;

                Point marker_roi = (*data).p_down;  //  point in ROI coordinates
                Point marker_global = Point(roi_top_left.x + (int)(marker_roi.x * roi_size / img_size), roi_top_left.y + (int)(marker_roi.y * roi_size / img_size));//  point in global coordinates
                circle((*data).img, marker_global, 0, MARK_COLOR, CIRCLE_THICKNESS/2);
                putText((*data).img, std::to_string(point_cnt), Point(marker_global.x - 20, marker_global.y - 10), FONT_HERSHEY_DUPLEX, 1, MARK_COLOR, 2);

                imshow("img", (*data).img);
            }
            break;
        }
        case EVENT_MOUSEMOVE: {     //  save the point
            (*data).p_move = Point(x, y);
            if (!in_roi) {  //  global image case
                Mat img_mark = (*data).img.clone();
                rectangle(img_mark, (*data).p_move, Point((*data).p_move.x + roi_size, (*data).p_move.y + roi_size), MARK_COLOR, RECT_THICKNESS);
                imshow("img", img_mark);   //  show roi border
            }
            else {  //  ROI case
                Mat img_mark = roi.clone();
                circle(img_mark, (*data).p_move, 0, MARK_COLOR, CIRCLE_THICKNESS);
                imshow("img", img_mark);   //  show roi border
            }
            break;
        }
    }
}