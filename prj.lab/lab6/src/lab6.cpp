#include "lab6.h"
#include "semcv.h"
#include <iostream>

namespace lab6 {

void runLab6Demo() {
    std::cout << "=== Lab 6: Object Detection using Filtering ===" << std::endl;

    cv::Mat testImage = cv::Mat::zeros(400, 600, CV_8UC3);

    cv::rectangle(testImage, cv::Point(50, 50), cv::Point(200, 200), cv::Scalar(100, 100, 100), -1);
    cv::circle(testImage, cv::Point(400, 150), 80, cv::Scalar(200, 200, 200), -1);

    cv::imshow("Original Test Image", testImage);

    std::cout << "Lab 6 demonstration completed." << std::endl;
    cv::waitKey(0);
    cv::destroyAllWindows();
}

}
