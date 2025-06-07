#include "glcm.h"
#include <iostream>
#include <cmath>
#include <sstream>

GLCMAnalyzer::GLCMAnalyzer() {}

GLCMAnalyzer::~GLCMAnalyzer() {}

bool GLCMAnalyzer::loadImage(const std::string& imagePath) {
    image_ = cv::imread(imagePath, cv::IMREAD_GRAYSCALE);
    if (image_.empty()) {
        std::cerr << "Ошибка: Не удалось загрузить изображение " << imagePath << std::endl;
        return false;
    }
    return true;
}

cv::Mat GLCMAnalyzer::calculateGLCM(const cv::Mat& image, int distance, int angle) {
    // Количество уровней серого (обычно 256, но можем уменьшить для производительности)
    const int numLevels = 256;

    // Создаем GLCM матрицу
    cv::Mat glcm = cv::Mat::zeros(numLevels, numLevels, CV_64F);

    // Вычисляем смещения на основе угла и расстояния
    int dx = 0, dy = 0;
    switch (angle) {
        case 0:   // горизонтальное направление
            dx = distance; dy = 0;
            break;
        case 45:  // диагональное направление
            dx = distance; dy = -distance;
            break;
        case 90:  // вертикальное направление
            dx = 0; dy = distance;
            break;
        case 135: // диагональное направление
            dx = -distance; dy = -distance;
            break;
        default:
            dx = distance; dy = 0; // по умолчанию горизонтальное
    }

    // Заполняем GLCM матрицу
    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
            int x2 = i + dx;
            int y2 = j + dy;

            // Проверяем границы
            if (x2 >= 0 && x2 < image.rows && y2 >= 0 && y2 < image.cols) {
                int pixel1 = image.at<uchar>(i, j);
                int pixel2 = image.at<uchar>(x2, y2);
                glcm.at<double>(pixel1, pixel2)++;
            }
        }
    }

    // Нормализуем GLCM
    normalizeGLCM(glcm);
    glcm_ = glcm;

    return glcm;
}

void GLCMAnalyzer::normalizeGLCM(cv::Mat& glcm) {
    double sum = cv::sum(glcm)[0];
    if (sum > 0) {
        glcm /= sum;
    }
}

// Реализация формулы контраста из прикрепленного изображения
double GLCMAnalyzer::getContrastFeatureValue(const cv::Mat& glcm) {
    double contrast = 0.0;

    // Согласно формуле: contrast = ∑∑(i-j)²p(i,j)
    for (int i = 0; i < glcm.rows; i++) {
        for (int j = 0; j < glcm.cols; j++) {
            double p_ij = glcm.at<double>(i, j);
            contrast += (i - j) * (i - j) * p_ij;
        }
    }

    return contrast;
}

// Реализация формулы корреляции из прикрепленного изображения
double GLCMAnalyzer::getCorrelationFeatureValue(const cv::Mat& glcm) {
    // Согласно формуле: correlation = ∑∑p(i,j)ij - μₓμᵧ / σₓ(i)σᵧ(j)

    // Вычисляем средние значения μₓ и μᵧ
    auto means = calculateMeans(glcm);
    double meanX = means.first;
    double meanY = means.second;

    // Вычисляем стандартные отклонения σₓ и σᵧ
    auto stdDevs = calculateStandardDeviations(glcm, meanX, meanY);
    double sigmaX = stdDevs.first;
    double sigmaY = stdDevs.second;

    // Избегаем деления на ноль
    if (sigmaX == 0 || sigmaY == 0) {
        return 1.0; // Как указано в примечании из изображения
    }

    // Вычисляем числитель формулы корреляции
    double numerator = 0.0;
    for (int i = 0; i < glcm.rows; i++) {
        for (int j = 0; j < glcm.cols; j++) {
            double p_ij = glcm.at<double>(i, j);
            numerator += p_ij * i * j;
        }
    }
    numerator -= meanX * meanY;

    // Возвращаем корреляцию
    return numerator / (sigmaX * sigmaY);
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
        oss << "Low-contrast, highly correlated image (possibly smooth or gently varying)";
    }
    else if (contrast > 200 && correlation < 0.5) {
        oss << "Image with sharp transitions (possibly noise or strong texture)";
    }
    else {
        oss << "Intermediate texture properties";
    }
    return oss.str();
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

std::pair<double, double> GLCMAnalyzer::calculateStandardDeviations(const cv::Mat& glcm, 
                                                                   double meanX, double meanY) {
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
