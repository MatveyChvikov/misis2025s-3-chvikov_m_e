#include <iostream>
#include <string>
#include <iomanip>
#include <opencv2/opencv.hpp>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "glcm.h"

void displayResults(const GLCMAnalyzer& analyzer, double contrast, double correlation) {
    double correctedCorrelation = correlation;
    if (correlation < 0) {
        std::cerr << "WARNING: Negative correlation detected! Using absolute value.\n";
        correctedCorrelation = std::abs(correlation);
    }
    cv::Mat resultsImage = cv::Mat::zeros(500, 800, CV_8UC3);
    resultsImage.setTo(cv::Scalar(40, 40, 40));

    int fontFace = cv::FONT_HERSHEY_SIMPLEX;
    double fontScale = 0.7;
    cv::Scalar textColor(255, 255, 255);
    int thickness = 2;
    int yPos = 80;

    cv::putText(resultsImage, "GLCM Texture Analysis Results",
        cv::Point(100, 50), fontFace, 0.9, cv::Scalar(0, 255, 255), thickness);

    std::ostringstream contrastStream;
    contrastStream << "Contrast: " << std::fixed << std::setprecision(4) << contrast;
    cv::putText(resultsImage, contrastStream.str(),
        cv::Point(50, yPos), fontFace, fontScale, textColor, thickness);
    yPos += 50;

    std::ostringstream correlationStream;
    correlationStream << "Correlation: " << std::fixed << std::setprecision(4) << correctedCorrelation;
    cv::putText(resultsImage, correlationStream.str(),
        cv::Point(50, yPos), fontFace, fontScale, textColor, thickness);
    yPos += 80;

    std::string interpretation = GLCMAnalyzer::interpretResult(contrast, correctedCorrelation);
    std::vector<std::string> lines;
    std::istringstream iss(interpretation);
    std::string line;

    while (std::getline(iss, line, '\n')) {
        lines.push_back(line);
    }

    cv::putText(resultsImage, "Texture Analysis:",
        cv::Point(50, yPos), fontFace, 0.7, cv::Scalar(0, 255, 0), thickness);
    yPos += 40;

    for (const auto& line : lines) {
        cv::putText(resultsImage, line,
            cv::Point(70, yPos), fontFace, 0.6, cv::Scalar(150, 200, 255), 1);
        yPos += 30;
    }

    cv::putText(resultsImage, "Press 'Q' to quit | 'N' for new image",
        cv::Point(150, 450), fontFace, 0.6, cv::Scalar(200, 200, 200), 1);

    cv::imshow("GLCM Results", resultsImage);
}

std::string selectImageFile() {
    std::string imagePath;
    std::cout << "Введите путь к изображению: ";
    std::getline(std::cin, imagePath);
    return imagePath;
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    setvbuf(stdout, nullptr, _IOFBF, 1000);

    std::cout << "=== GLCM Texture Analysis Application ===" << std::endl;
    GLCMAnalyzer analyzer;

    while (true) {
        std::string imagePath = selectImageFile();
        if (imagePath.empty()) {
            std::cout << "Выход из программы." << std::endl;
            break;
        }

        if (!analyzer.loadImage(imagePath)) {
            std::cout << "Не удалось загрузить изображение. Попробуйте еще раз." << std::endl;
            continue;
        }

        std::cout << "Изображение успешно загружено." << std::endl;
        std::cout << "Вычисление GLCM матрицы для всех углов..." << std::endl;

        cv::Mat glcm = analyzer.calculateGLCM(analyzer.getImage());

        std::cout << "GLCM матрица вычислена." << std::endl;
        std::cout << "Вычисление текстурных характеристик..." << std::endl;

        double contrast = analyzer.getContrastFeatureValue(glcm);
        double correlation = analyzer.getCorrelationFeatureValue(glcm);

        std::cout << std::fixed << std::setprecision(6);
        std::cout << "\n=== Результаты анализа ===" << std::endl;
        std::cout << "Контраст (Contrast): " << contrast << std::endl;
        std::cout << "Корреляция (Correlation): " << correlation << std::endl;
        std::cout << "========================" << std::endl;
        std::cout << "\n=== Анализ текстуры ===" << std::endl;
        std::cout << GLCMAnalyzer::interpretResult(contrast, correlation) << std::endl;

        cv::imshow("Original Image", analyzer.getImage());
        displayResults(analyzer, contrast, correlation);

        char key = cv::waitKey(0);
        if (key == 'q' || key == 'Q') {
            break;
        }
        cv::destroyAllWindows();
    }

    cv::destroyAllWindows();
    return 0;
}
