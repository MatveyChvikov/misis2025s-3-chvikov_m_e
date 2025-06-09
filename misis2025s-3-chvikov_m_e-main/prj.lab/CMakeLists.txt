
add_subdirectory(lab1)
add_subdirectory(lab2)
add_subdirectory(lab3)
add_subdirectory(lab4)
add_subdirectory(lab5)
add_subdirectory(lab6)
add_subdirectory(lab7)

add_custom_target(all_labs)
add_dependencies(all_labs 
    lab1
    lab2
    lab3
    lab4
    lab5
    lab6
    lab7
)
