project(console-ui)

add_executable(${PROJECT_NAME}
    main.cpp
)

target_add_custom_compile_flags(${PROJECT_NAME})

find_package(fmt REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE
    core
    fmt::fmt
)

add_ipo(${PROJECT_NAME})
