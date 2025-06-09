include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(lab7
    src/main.cpp
    src/lab7.cpp
)

target_link_libraries(lab7 semcv ${OpenCV_LIBS})

add_executable(lab7
    test/test_lab7.cpp
    src/lab7.cpp
)

target_link_libraries(lab7_test semcv ${OpenCV_LIBS})

set_target_properties(lab7 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

set_target_properties(lab7_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/test
)
