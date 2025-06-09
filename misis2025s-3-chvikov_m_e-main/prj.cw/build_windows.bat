@echo off
echo Building GLCM Analyzer Project
echo ==============================

REM Проверяем наличие vcpkg
if not exist "C:\vcpkg\vcpkg.exe" (
    echo Ошибка: vcpkg не найден в C:\vcpkg\
    echo Пожалуйста, установите vcpkg и повторите попытку
    pause
    exit /b 1
)

REM Создаем директорию сборки
if not exist "build" mkdir build
cd build

REM Конфигурация CMake
echo Конфигурация проекта...
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake -G "Visual Studio 17 2022" -A x64

if %ERRORLEVEL% neq 0 (
    echo Ошибка конфигурации CMake
    pause
    exit /b 1
)

REM Сборка проекта
echo Сборка проекта...
cmake --build . --config Release

if %ERRORLEVEL% neq 0 (
    echo Ошибка сборки проекта
    pause
    exit /b 1
)

echo.
echo Сборка завершена успешно!
echo Исполняемый файл: build\Release\glcm_analyzer.exe
pause
