project(gui)

add_executable(${PROJECT_NAME}
    main.cpp
)

target_add_custom_compile_flags(${PROJECT_NAME})

find_package(Threads REQUIRED)
find_package(Qt6 REQUIRED Core Gui Widgets)

target_link_libraries(${PROJECT_NAME} PRIVATE
    core
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

add_ipo(${PROJECT_NAME})
