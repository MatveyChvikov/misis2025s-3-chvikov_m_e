#include "glcm.h"
#include <iostream>
#include <cmath>
#include <stdexcept>

GLCMAnalyzer::GLCMAnalyzer() : distance_(1), angles_({ 0, 45, 90, 135 }) {}

GLCMAnalyzer::~GLCMAnalyzer() {}

bool GLCMAnalyzer::loadImage(const std::string& imagePath) {
    image_ = cv::imread(imagePath, cv::IMREAD_GRAYSCALE);
    if (image_.empty()) {
        std::cerr << "Ошибка: Не удалось загрузить изображение " << imagePath << std::endl;
        return false;
    }
    return true;
}

cv::Mat GLCMAnalyzer::calculateGLCM(const cv::Mat& image) {
    const int numLevels = 256;
    cv::Mat glcmSum = cv::Mat::zeros(numLevels, numLevels, CV_64F);

    for (int angle : angles_) {
        // Вычисляем смещение на основе угла и расстояния
        double rad = angle * CV_PI / 180.0;
        int dx = static_cast<int>(std::round(distance_ * std::cos(rad)));
        int dy = static_cast<int>(std::round(distance_ * std::sin(rad)));

        cv::Mat glcm = cv::Mat::zeros(numLevels, numLevels, CV_64F);

        // Заполняем GLCM матрицу
        for (int i = 0; i < image.rows; i++) {
            for (int j = 0; j < image.cols; j++) {
                int x2 = j + dx;  // Обратите внимание: x = j (столбец), y = i (строка)
                int y2 = i + dy;

                if (x2 >= 0 && x2 < image.cols && y2 >= 0 && y2 < image.rows) {
                    int pixel1 = image.at<uchar>(i, j);
                    int pixel2 = image.at<uchar>(y2, x2);
                    glcm.at<double>(pixel1, pixel2)++;
                }
            }
        }

        // Нормализуем и суммируем
        normalizeGLCM(glcm);
        glcmSum += glcm;
    }

    // Усреднение результатов
    glcmSum /= angles_.size();
    glcm_ = glcmSum;
    return glcmSum;
}

void GLCMAnalyzer::normalizeGLCM(cv::Mat& glcm) {
    double sum = cv::sum(glcm)[0];
    if (sum > 0) {
        glcm /= sum;
    }
}

double GLCMAnalyzer::getContrastFeatureValue(const cv::Mat& glcm) {
    double contrast = 0.0;
    for (int i = 0; i < glcm.rows; i++) {
        for (int j = 0; j < glcm.cols; j++) {
            double p_ij = glcm.at<double>(i, j);
            contrast += (i - j) * (i - j) * p_ij;
        }
    }
    return contrast;
}

double GLCMAnalyzer::getCorrelationFeatureValue(const cv::Mat& glcm) {
    auto means = calculateMeans(glcm);
    double meanX = means.first;
    double meanY = means.second;

    auto stdDevs = calculateStandardDeviations(glcm, meanX, meanY);
    double sigmaX = stdDevs.first;
    double sigmaY = stdDevs.second;

    if (sigmaX < 1e-9 || sigmaY < 1e-9) {
        return 1.0;  
    }

    double numerator = 0.0;
    for (int i = 0; i < glcm.rows; i++) {
        for (int j = 0; j < glcm.cols; j++) {
            double p_ij = glcm.at<double>(i, j);
            numerator += p_ij * i * j;
        }
    }
    numerator -= meanX * meanY;

    
    double correlation = numerator / (sigmaX * sigmaY);
    return std::abs(correlation);  
}

std::pair<double, double> GLCMAnalyzer::calculateMeans(const cv::Mat& glcm) {
    double meanX = 0.0, meanY = 0.0;
    for (int i = 0; i < glcm.rows; i++) {
        for (int j = 0; j < glcm.cols; j++) {
            double p_ij = glcm.at<double>(i, j);
            meanX += i * p_ij;
            meanY += j * p_ij;
        }
    }
    return std::make_pair(meanX, meanY);
}

std::pair<double, double> GLCMAnalyzer::calculateStandardDeviations(
    const cv::Mat& glcm, double meanX, double meanY) {
    double varX = 0.0, varY = 0.0;
    for (int i = 0; i < glcm.rows; i++) {
        for (int j = 0; j < glcm.cols; j++) {
            double p_ij = glcm.at<double>(i, j);
            varX += (i - meanX) * (i - meanX) * p_ij;
            varY += (j - meanY) * (j - meanY) * p_ij;
        }
    }
    return std::make_pair(std::sqrt(varX), std::sqrt(varY));
}

std::string GLCMAnalyzer::interpretResult(double contrast, double correlation) {
    std::ostringstream oss;
    if (contrast < 1e-3 && correlation > 0.99) {
        oss << "Uniform or smooth image (gradient or flat area)";
    }
    else if (contrast > 1000 && correlation < 0.3) {
        oss << "Highly textured image (checkerboard or strong transitions)";
    }
    else if (contrast > 100 && correlation > 0.7) {
        oss << "Textured image with strong linear patterns";
    }
    else if (contrast < 20 && correlation > 0.8) {
        oss << "Low-contrast, highly correlated image";
    }
    else if (contrast > 200 && correlation < 0.5) {
        oss << "Image with sharp transitions (possibly noise or strong texture)";
    }
    else {
        oss << "Intermediate texture properties";
    }
    return oss.str();
}
