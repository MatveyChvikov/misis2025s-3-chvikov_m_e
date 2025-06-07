#ifdef _WIN32
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#endif

#include <opencv2/opencv.hpp>
#include <iostream>
#include <random>

void generateCheckerboard(const std::string& filename, int size = 256, int squareSize = 16) {
    cv::Mat image(size, size, CV_8UC1);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            int blockI = i / squareSize;
            int blockJ = j / squareSize;
            image.at<uchar>(i, j) = ((blockI + blockJ) % 2) * 255;
        }
    }

    cv::imwrite(filename, image);
    std::wcout << L"Создано: " << filename.c_str() << std::endl;
}

void generateStripes(const std::string& filename, int size = 256, int stripeWidth = 8, bool vertical = true) {
    cv::Mat image(size, size, CV_8UC1);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            int coord = vertical ? j : i;
            image.at<uchar>(i, j) = ((coord / stripeWidth) % 2) * 255;
        }
    }

    cv::imwrite(filename, image);
    std::wcout << L"Создано: " << filename.c_str() << std::endl;
}

void generateGradient(const std::string& filename, int size = 256, bool horizontal = true) {
    cv::Mat image(size, size, CV_8UC1);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (horizontal) {
                image.at<uchar>(i, j) = (j * 255) / (size - 1);
            }
            else {
                image.at<uchar>(i, j) = (i * 255) / (size - 1);
            }
        }
    }

    cv::imwrite(filename, image);
    std::wcout << L"Создано: " << filename.c_str() << std::endl;
}

void generateNoise(const std::string& filename, int size = 256, int seed = 42) {
    cv::Mat image(size, size, CV_8UC1);
    std::mt19937 gen(seed);
    std::uniform_int_distribution<> dis(0, 255);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            image.at<uchar>(i, j) = dis(gen);
        }
    }

    cv::imwrite(filename, image);
    std::wcout << L"Создано: " << filename.c_str() << std::endl;
}

void generateCircles(const std::string& filename, int size = 256) {
    cv::Mat image = cv::Mat::zeros(size, size, CV_8UC1);
    cv::Point center(size / 2, size / 2);

    // Концентрические круги
    for (int r = 20; r < size / 2; r += 40) {
        cv::circle(image, center, r, cv::Scalar(255), 10);
    }

    cv::imwrite(filename, image);
    std::wcout << L"Создано: " << filename.c_str() << std::endl;
}

void generateTexture(const std::string& filename, int size = 256) {
    cv::Mat image(size, size, CV_8UC1);

    // Текстура "кирпичная кладка"
    int brickHeight = 20;
    int brickWidth = 40;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            int row = i / brickHeight;
            int col = j / brickWidth;

            // Смещение для нечетных рядов
            if (row % 2 == 1) {
                col = (j + brickWidth / 2) / brickWidth;
            }

            // Швы между кирпичами
            bool isSeam = (i % brickHeight < 2) || (j % brickWidth < 2);

            if (isSeam) {
                image.at<uchar>(i, j) = 100; // Серый цвет швов
            }
            else {
                image.at<uchar>(i, j) = 200; // Светлый цвет кирпичей
            }
        }
    }

    cv::imwrite(filename, image);
    std::wcout << L"Создано: " << filename.c_str() << std::endl;
}

int main() {
#ifdef _WIN32
    // Включаем поддержку Unicode вывода для консоли Windows
    SetConsoleOutputCP(CP_UTF8);
    _setmode(_fileno(stdout), _O_U16TEXT);
#endif

    std::wcout << L"=== Генератор тестовых изображений для GLCM ===" << std::endl;

#ifdef _WIN32
    system("if not exist examples mkdir examples");
#else
    system("mkdir -p examples");
#endif

    generateCheckerboard("examples/checkerboard.png");
    generateStripes("examples/stripes_vertical.png", 256, 8, true);
    generateStripes("examples/stripes_horizontal.png", 256, 8, false);
    generateGradient("examples/gradient_horizontal.png", 256, true);
    generateGradient("examples/gradient_vertical.png", 256, false);
    generateNoise("examples/noise.png");
    generateCircles("examples/circles.png");
    generateTexture("examples/brick_texture.png");
    generateCheckerboard("examples/checkerboard_small.png", 256, 4);
    generateCheckerboard("examples/checkerboard_large.png", 256, 32);

    std::wcout << L"\n=== Описание тестовых изображений ===" << std::endl;
    std::wcout << L"checkerboard.png - Шахматная доска (высокий контраст)" << std::endl;
    std::wcout << L"stripes_*.png - Полосы (анизотропная текстура)" << std::endl;
    std::wcout << L"gradient_*.png - Градиенты (низкий контраст)" << std::endl;
    std::wcout << L"noise.png - Случайный шум (высокий контраст, низкая корреляция)" << std::endl;
    std::wcout << L"circles.png - Концентрические круги" << std::endl;
    std::wcout << L"brick_texture.png - Кирпичная кладка (сложная текстура)" << std::endl;

    std::wcout << L"\n=== Ожидаемые результаты GLCM ===" << std::endl;
    std::wcout << L"Шахматная доска: Высокий контраст, низкая корреляция" << std::endl;
    std::wcout << L"Градиент: Низкий контраст, высокая корреляция" << std::endl;
    std::wcout << L"Шум: Высокий контраст, низкая корреляция" << std::endl;
    std::wcout << L"Полосы: Зависит от направления анализа GLCM" << std::endl;

    return 0;
}
