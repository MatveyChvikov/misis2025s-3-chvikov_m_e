include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(lab3
    src/main.cpp
    src/lab3.cpp
)

target_link_libraries(lab3 semcv ${OpenCV_LIBS})

add_executable(lab3_test
    test/test_lab3.cpp
    src/lab3.cpp
)

target_link_libraries(lab3_test semcv ${OpenCV_LIBS})

set_target_properties(lab3 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

set_target_properties(lab3_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/test
)
