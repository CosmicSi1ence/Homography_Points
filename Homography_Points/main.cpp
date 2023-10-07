#include "callbacks.h"

#include <windows.h>
#include <iostream>

#include <opencv2/imgproc.hpp>
#include "INIReader.h"

using namespace cv;
using namespace std;

string img_path;

bool get_ini_params(const string& config) {

    setlocale(LC_NUMERIC, "en_US.UTF-8");
    INIReader reader(config);
    if (reader.ParseError() < 0) {
        cout << "Can't load '" << config << "'\n";
        return 0;
    }
    // ------------------------------ source_img: 
    img_size = reader.GetInteger("source_img", "img_size", -1);
    if (img_size == -1) {
        cout << "img_size not declared\n";
    }
    else {
        cout << "img_size = " << img_size << ";\n";
    }

    img_path = reader.Get("source_img", "img_path", "");
    if (img_path == "") {
        cout << "img_path not declared!\n";
        return 0;
    }
    else {
        cout << "img_path = " << img_path << ";\n";
    }
    // ----------------------------------------
    return 1;
}


int main() {

    bool is_ok = get_ini_params("img_data.ini");
    if (!is_ok) {
        cout << "ini file error\n";
        return 0;
    }

    Mat img = imread(img_path, IMREAD_COLOR);       //  считываем изображение из файла
    if (img_size > 0) {
        img = img(Rect(Point(), Size(img_size, img_size))).clone();
    }

    namedWindow("img", WINDOW_AUTOSIZE);
    imshow("img", img);                        //  выводим изображение
    moveWindow("img", WINDOW_X_SHIFT, WINDOW_Y_SHIFT);

    User_data u_data = User_data();
    u_data.img = img;
    setMouseCallback("img", callBackFuncSource, &u_data);
    waitKey(0);

    return 0;
}
