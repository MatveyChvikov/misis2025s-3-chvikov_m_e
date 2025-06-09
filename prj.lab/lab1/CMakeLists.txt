# lab1 CMakeLists.txt

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(lab1
    src/main.cpp
    src/lab1.cpp
)

target_link_libraries(lab1 semcv ${OpenCV_LIBS})

add_executable(lab1_test
    test/test_lab1.cpp
    src/lab1.cpp
)

target_link_libraries(lab1_test semcv ${OpenCV_LIBS})

set_target_properties(lab1 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

set_target_properties(lab1_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/test
)
