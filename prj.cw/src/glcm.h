#ifndef GLCM_H
#define GLCM_H

#include <opencv2/opencv.hpp>
#include <vector>
#include <string>

class GLCMAnalyzer {
public:
    static std::string interpretResult(double contrast, double correlation);

    GLCMAnalyzer();
    ~GLCMAnalyzer();

    bool loadImage(const std::string& imagePath);
    cv::Mat calculateGLCM(const cv::Mat& image);

    double getContrastFeatureValue(const cv::Mat& glcm);
    double getCorrelationFeatureValue(const cv::Mat& glcm);

    cv::Mat getImage() const { return image_; }
    cv::Mat getGLCM() const { return glcm_; }

    // Методы для настройки параметров
    void setAngles(const std::vector<int>& angles) { angles_ = angles; }
    void setDistance(int distance) { distance_ = distance; }

private:
    cv::Mat image_;
    cv::Mat glcm_;
    std::vector<int> angles_;   // Углы в градусах (0, 45, 90, 135)
    int distance_ = 1;          // Расстояние (радиус)

    // Вспомогательные функции
    void normalizeGLCM(cv::Mat& glcm);
    std::pair<double, double> calculateMeans(const cv::Mat& glcm);
    std::pair<double, double> calculateStandardDeviations(const cv::Mat& glcm,
        double meanX, double meanY);
};

#endif // GLCM_H
