include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(lab4
    src/main.cpp
    src/lab4.cpp
)

target_link_libraries(lab4 semcv ${OpenCV_LIBS})

add_executable(lab4_test
    test/test_lab4.cpp
    src/lab4.cpp
)

target_link_libraries(lab4_test semcv ${OpenCV_LIBS})

set_target_properties(lab4 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

set_target_properties(lab4_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/test
)
