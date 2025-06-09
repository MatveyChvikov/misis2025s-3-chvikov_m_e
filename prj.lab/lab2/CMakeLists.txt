include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(lab2
    src/main.cpp
    src/lab2.cpp
)

target_link_libraries(lab2 semcv ${OpenCV_LIBS})

add_executable(lab2_test
    test/test_lab2.cpp
    src/lab2.cpp
)

target_link_libraries(lab2_test semcv ${OpenCV_LIBS})

set_target_properties(lab2 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

set_target_properties(lab2_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/test
)
