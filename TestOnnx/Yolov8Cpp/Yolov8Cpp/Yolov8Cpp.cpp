// Yolov8Cpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/video.hpp>

#include "inference.h"

using namespace cv;
using namespace std;

int main()
{
    std::cout << "Hello World!\n";

    cv::VideoCapture cap(0, CAP_DSHOW);
    if (!cap.isOpened()) {
        std::cout << "Caemra open failed\n";
        return 0;
    }

    std::string modelPath = "yolov8n.onnx";
    Inference inf(modelPath, Size(640, 640), "classes.txt", false);

    while (true) {
        Mat m;
        bool ret = cap.read(m);
        if (!ret) {
            break;
        }

        std::vector<Detection> output = inf.runInference(m);

        int detections = output.size();
        std::cout << "Number of detections:" << detections << std::endl;

        for (int i = 0; i < detections; ++i)
        {
            Detection detection = output[i];

            cv::Rect box = detection.box;
            cv::Scalar color = detection.color;

            // Detection box
            cv::rectangle(m, box, color, 2);

            // Detection box text
            std::string classString = detection.className + ' ' + std::to_string(detection.confidence).substr(0, 4);
            cv::Size textSize = cv::getTextSize(classString, cv::FONT_HERSHEY_DUPLEX, 1, 2, 0);
            cv::Rect textBox(box.x, box.y - 40, textSize.width + 10, textSize.height + 20);

            cv::rectangle(m, textBox, color, cv::FILLED);
            cv::putText(m, classString, cv::Point(box.x + 5, box.y - 10), cv::FONT_HERSHEY_DUPLEX, 1, cv::Scalar(0, 0, 0), 2, 0);
        }


        //cout << "Width: " << m.cols << ", Height: " << m.rows << endl;
        imshow("Mat", m);
        waitKey(30);
    }
    
    destroyAllWindows();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
