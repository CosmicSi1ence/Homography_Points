#include <windows.h>
#include <iostream>

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include "INIReader.h"

//  Пространства имен:
using namespace cv;
using namespace std;

//  Макросы для удобства
#define MARK_COLOR          (Scalar(0, 255, 0))	//  B G R - цвет геометрии
#define RECT_THICKNESS      (2)					//	Толщина рамок
#define CIRCLE_THICKNESS    (20)				//	Размер точек

//  --- Глобальных переменные ---------------------------------------
Mat img;                            //  Матрица изображения
unsigned int img_size = 0;          //  Размер изображения
string img_path;                    //  Путь к файлу с изображением

Mat roi_img;                        //  Матрица ROI
unsigned int roi_size = 50;         //  Размер квадратного ROI
Point roi_top_left;                 //  Положение верхнего левого угла ROI

bool roi_opened = false;            //  false - работа с полным изображением, true - работа с ROI

unsigned int point_cnt  = 0;        //  Текущий номер точки

Point point_move, point_click;      //  Положения мыши при движении, зажатии, отжатии
//  -------------------------------------------------------------------


//  ФУНКЦИЯ ОБРАБОТКИ СОБЫТИЙ МЫШИ:
void callBackFuncSource(int event, int x, int y, int flags, void * data) {

    switch (event) {
        case EVENT_LBUTTONDOWN: {       //  Если нажата ЛКМ
            point_click = Point(x, y);  //  Сохраняем точку нажатия

            if (!roi_opened) {          //  Если открыто полное изображение
                roi_opened = true;
                ++point_cnt;

                roi_top_left = point_click;
                Point roi_bottom_right = Point(roi_top_left.x + roi_size, roi_top_left.y + roi_size);

                //  Граничные условия (когда ROI выходит за границы изображения):
                if (roi_bottom_right.x > img.cols) {
                    roi_top_left.x = img.cols - roi_size;
                }
                if (roi_bottom_right.y > img.rows) {
                    roi_top_left.y = img.rows - roi_size;
                }

                //  Выделяем ROI на исходном изображении:
                roi_img = Mat(img, Rect(roi_top_left, Point(roi_top_left.x + roi_size, roi_top_left.y + roi_size)));

                resize(roi_img, roi_img, Size(img_size, img_size)); //  Растягиваем ROI до размеров окна
                imshow("img", roi_img);
            }
            else {                      //  Если открыто ROI
                roi_opened = false;

                //  Точка на координатах ROI:
                Point marker_roi = point_click;
                //  Точка на полном изображении
                Point marker_global = Point(roi_top_left.x + (int)(marker_roi.x * roi_size / img_size), roi_top_left.y + (int)(marker_roi.y * roi_size / img_size));

                //  Рисуем точку в заданных координатах:
                circle(img, marker_global, 0, MARK_COLOR, CIRCLE_THICKNESS/2);
                //  Подписываем номер точки:
                putText(img, to_string(point_cnt), Point(marker_global.x - 20, marker_global.y - 10), FONT_HERSHEY_DUPLEX, 1, MARK_COLOR, 2);

                imshow("img", img);
            }
            break;
        }
        case EVENT_MOUSEMOVE: { //  Если произошло движение мыши
            point_move = Point(x, y);           //  Сохраняем точку движения

            if (!roi_opened) {  //  Если открыто полное изображение
                Mat img_mark = img.clone();     //  Создаем копию, чтобы не испортить исходник

                //  Рисуем прямоугольник, где будет ROI:
                rectangle(img_mark, point_move, Point(point_move.x + roi_size, point_move.y + roi_size), MARK_COLOR, RECT_THICKNESS);

                imshow("img", img_mark);        //  Выводим картинку с прямоугольником
            }
            else {  //  Если открыто ROI
                Mat img_mark = roi_img.clone(); //  Создаем копию, чтобы не испортить исходник

                //  Рисуем точку на ROI:
                circle(img_mark, point_move, 0, MARK_COLOR, CIRCLE_THICKNESS);

                imshow("img", img_mark);        //  Выводим ROI с точкой
            }
            break;
        }
    }
}


//  ФУНКЦИЯ ЧТЕНИЯ INI-ФАЙЛА
bool get_ini_params(const string& config) {

    setlocale(LC_NUMERIC, "en_US.UTF-8");
    INIReader reader(config);       //  Создаем объект Ридера

    if (reader.ParseError() < 0) {  //  Если ошибка, то останавливаем программу
        cout << "Can't load '" << config << "'\n";
        return 0;
    }

    // ------------------------------ source_img: ----------------------------
    img_size = reader.GetInteger("source_img", "img_size", -1); //  Читаем размер изображения (квадратного)
    if (img_size == -1) {   //  Если не задано, то останавливаем программу
        cout << "img_size not declared\n";
    }
    else {
        cout << "img_size = " << img_size << ";\n";
    }

    img_path = reader.Get("source_img", "img_path", "");        //  Читаем путь к изображению
    if (img_path == "") {   //  Если не задано, то останавливаем программу
        cout << "img_path not declared\n";
        return 0;
    }
    else {
        cout << "img_path = " << img_path << ";\n";
    }
    // ----------------------------------------------------------------------
    return 1;
}


int main() {

    //  Читаем INI-файл:
    bool is_ok = get_ini_params("img_data.ini");
    if (!is_ok) {   //  Если ошибка, то останавливаем программу
        cout << "INI-file error\n";
        return 0;
    }

    img = imread(img_path, IMREAD_COLOR);           //  Считываем изображение по указанному пути

    namedWindow("img", WINDOW_AUTOSIZE);
    imshow("img", img);                             //  Выводим изображение
    moveWindow("img", 0, 65);

    setMouseCallback("img", callBackFuncSource);    //  Задаем функцию обработчика событий мыши для окна "img"
    waitKey(0); //  Ожидаем нажатия любой клавиши

    return 0;   //  Завершаем программу
}
