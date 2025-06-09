cmake_minimum_required(VERSION 3.10)
project(glcm_analyzer)

# Настройка стандарта C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Поиск пакета OpenCV
find_package(OpenCV REQUIRED)

# Главное приложение для анализа GLCM
add_executable(glcm_analyzer
    src/main.cpp
    src/glcm.cpp
)

# Подключение OpenCV для основного приложения
target_include_directories(glcm_analyzer PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(glcm_analyzer PRIVATE ${OpenCV_LIBS})

# Генератор тестовых изображений
add_executable(generate_test_images
    src/generate_test_images.cpp
)

# Подключение OpenCV для генератора
target_include_directories(generate_test_images PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(generate_test_images PRIVATE ${OpenCV_LIBS})

# Общие настройки для Windows
if(WIN32)
    target_link_libraries(glcm_analyzer PRIVATE Kernel32)
    target_link_libraries(generate_test_images PRIVATE Kernel32)
endif()
