
#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <vector>
#include <opencv2/opencv.hpp>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "glcm.h"

// Функция для отображения результатов
void displayResults(const GLCMAnalyzer& analyzer, double contrast, double correlation) {
    // Создаем окно для отображения результатов
    cv::Mat resultsImage = cv::Mat::zeros(500, 800, CV_8UC3); // Увеличили размер окна
    resultsImage.setTo(cv::Scalar(40, 40, 40)); // Темно-серый фон

    // Настройки для текста
    int fontFace = cv::FONT_HERSHEY_SIMPLEX;
    double fontScale = 0.7;
    cv::Scalar textColor(255, 255, 255); // Белый цвет
    int thickness = 2;
    int yPos = 80;

    // Заголовок
    cv::putText(resultsImage, "GLCM Texture Analysis Results",
        cv::Point(100, 50), fontFace, 0.9, cv::Scalar(0, 255, 255), thickness);

    // Результаты контраста
    std::ostringstream contrastStream;
    contrastStream << "Contrast: " << std::fixed << std::setprecision(4) << contrast;
    cv::putText(resultsImage, contrastStream.str(),
        cv::Point(50, yPos), fontFace, fontScale, textColor, thickness);
    yPos += 50;

    // Результаты корреляции
    std::ostringstream correlationStream;
    correlationStream << "Correlation: " << std::fixed << std::setprecision(4) << correlation;
    cv::putText(resultsImage, correlationStream.str(),
        cv::Point(50, yPos), fontFace, fontScale, textColor, thickness);
    yPos += 80;

    // Текстовая интерпретация
    std::string interpretation = GLCMAnalyzer::interpretResult(contrast, correlation);

    // Разбиваем текст на строки
    std::vector<std::string> lines;
    std::istringstream iss(interpretation);
    std::string line;

    while (std::getline(iss, line, '\n')) {
        lines.push_back(line);
    }

    // Выводим заголовок интерпретации
    cv::putText(resultsImage, "Texture Analysis:",
        cv::Point(50, yPos), fontFace, 0.7, cv::Scalar(0, 255, 0), thickness);
    yPos += 40;

    // Выводим строки интерпретации
    for (const auto& line : lines) {
        cv::putText(resultsImage, line,
            cv::Point(70, yPos), fontFace, 0.6, cv::Scalar(150, 200, 255), 1);
        yPos += 30;
    }

    // Инструкция
    cv::putText(resultsImage, "Press 'Q' to quit | 'N' for new image",
        cv::Point(150, 450), fontFace, 0.6, cv::Scalar(200, 200, 200), 1);

    cv::imshow("GLCM Results", resultsImage);
}

// Функция для выбора файла (упрощенная версия)
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

    std::cout << "=== GLCM Texture Analysis Application ===" << std::endl;
    std::cout << "Реализация методов getContrastFeatureValue() и getCorrelationFeatureValue()" << std::endl;
    std::cout << "=========================================" << std::endl;

    while (true) {
        // Получаем путь к изображению от пользователя
        std::string imagePath = selectImageFile();

        if (imagePath.empty()) {
            std::cout << "Выход из программы." << std::endl;
            break;
        }

        // Загружаем изображение
        if (!analyzer.loadImage(imagePath)) {
            std::cout << "Не удалось загрузить изображение. Попробуйте еще раз." << std::endl;
            continue;
        }

        std::cout << "Изображение успешно загружено." << std::endl;
        std::cout << "Вычисление GLCM матрицы..." << std::endl;

        // Вычисляем GLCM матрицу
        cv::Mat glcm = analyzer.calculateGLCM(analyzer.getImage());

        std::cout << "GLCM матрица вычислена." << std::endl;
        std::cout << "Вычисление текстурных характеристик..." << std::endl;

        // Вычисляем характеристики
        double contrast = analyzer.getContrastFeatureValue(glcm);
        double correlation = analyzer.getCorrelationFeatureValue(glcm);

        // Выводим результаты в консоль
        std::cout << std::fixed << std::setprecision(6);
        std::cout << "\n=== Результаты анализа ===" << std::endl;
        std::cout << "Контраст (Contrast): " << contrast << std::endl;
        std::cout << "Корреляция (Correlation): " << correlation << std::endl;
        std::cout << "========================" << std::endl;

        std::cout << "\n=== Анализ текстуры ===" << std::endl;
        std::cout << GLCMAnalyzer::interpretResult(contrast, correlation) << std::endl;

        // Отображаем исходное изображение
        cv::imshow("Original Image", analyzer.getImage());

        // Отображаем результаты
        displayResults(analyzer, contrast, correlation);

        // Ждем нажатия клавиши
        char key = cv::waitKey(0);
        if (key == 'q' || key == 'Q') {
            break;
        }

        // Закрываем окна для следующей итерации
        cv::destroyAllWindows();
    }

    cv::destroyAllWindows();
    return 0;
}
