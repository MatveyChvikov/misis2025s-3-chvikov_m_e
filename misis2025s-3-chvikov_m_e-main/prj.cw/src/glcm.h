#ifndef GLCM_H
#define GLCM_H

#include <opencv2/opencv.hpp>
#include <vector>

class GLCMAnalyzer {
public:

    static std::string interpretResult(double contrast, double correlation);

    // Конструктор
    GLCMAnalyzer();

    // Деструктор
    ~GLCMAnalyzer();

    // Загрузка изображения
    bool loadImage(const std::string& imagePath);

    // Вычисление GLCM матрицы
    cv::Mat calculateGLCM(const cv::Mat& image, int distance = 1, int angle = 0);

    // Вычисление контраста (формула из изображения)
    double getContrastFeatureValue(const cv::Mat& glcm);

    // Вычисление корреляции (формула из изображения)
    double getCorrelationFeatureValue(const cv::Mat& glcm);

    // Получение загруженного изображения
    cv::Mat getImage() const { return image_; }

    // Получение последней вычисленной GLCM матрицы
    cv::Mat getGLCM() const { return glcm_; }

private:
    cv::Mat image_;
    cv::Mat glcm_;

    // Вспомогательные функции
    void normalizeGLCM(cv::Mat& glcm);
    std::pair<double, double> calculateMeans(const cv::Mat& glcm);
    std::pair<double, double> calculateStandardDeviations(const cv::Mat& glcm, 
                                                          double meanX, double meanY);
};

#endif // GLCM_H
