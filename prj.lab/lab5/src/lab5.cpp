#include "lab5.h"
#include "semcv.h"
#include <iostream>

namespace lab5 {

void runLab5Demo() {
    std::cout << "=== Lab 5: Linear Image Filtering ===" << std::endl;

    cv::Mat testImage = cv::Mat::zeros(400, 600, CV_8UC3);

    cv::rectangle(testImage, cv::Point(50, 50), cv::Point(200, 200), cv::Scalar(100, 100, 100), -1);
    cv::circle(testImage, cv::Point(400, 150), 80, cv::Scalar(200, 200, 200), -1);

    cv::imshow("Original Test Image", testImage);

    std::cout << "Lab 5 demonstration completed." << std::endl;
    cv::waitKey(0);
    cv::destroyAllWindows();
}

} 
