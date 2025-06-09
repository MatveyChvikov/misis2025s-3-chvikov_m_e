include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(lab5
    src/main.cpp
    src/lab5.cpp
)

target_link_libraries(lab5 semcv ${OpenCV_LIBS})

add_executable(lab5
    test/test_lab5.cpp
    src/lab5.cpp
)

target_link_libraries(lab5_test semcv ${OpenCV_LIBS})

set_target_properties(lab5 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

set_target_properties(lab5_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/test
)
