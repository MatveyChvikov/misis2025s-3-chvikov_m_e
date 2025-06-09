# GLCM Texture Analysis Application

Приложение для анализа текстурных характеристик изображений с использованием Gray Level Co-occurrence Matrix (GLCM).

## Быстрый старт

### Требования
- CMake 3.20+
- vcpkg
- Компилятор с поддержкой C++17

### Сборка
```bash
mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=[путь_к_vcpkg]/scripts/buildsystems/vcpkg.cmake
cmake --build . --config Release
```

### Запуск
```bash
./build/Release/glcm_analyzer  # Windows
./build/glcm_analyzer          # Linux/macOS
```

## Функциональность

Приложение реализует два основных метода анализа текстуры:
- **getContrastFeatureValue()** - вычисление контраста по формуле Харалик
- **getCorrelationFeatureValue()** - вычисление корреляции по формуле Харалик

## Использованные формулы

### Контраст
```
contrast = ∑∑(i-j)²p(i,j)
```

### Корреляция  
```
correlation = (∑∑p(i,j)ij - μxμy) / (σx·σy)
```


