
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(lab6
    src/main.cpp
    src/lab6.cpp
)

target_link_libraries(lab6g semcv ${OpenCV_LIBS})

add_executable(lab6_test
    test/test_lab6.cpp
    src/lab6.cpp
)

target_link_libraries(lab6_test semcv ${OpenCV_LIBS})

set_target_properties(lab6 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

set_target_properties(lab6_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/test
)
