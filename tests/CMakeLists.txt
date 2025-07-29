project(tests)

add_executable(${PROJECT_NAME}
    test.cpp
)

target_add_custom_compile_flags(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/src/core")

find_package(Threads REQUIRED)
find_package(snitch REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE
    core
    Threads::Threads
    snitch::snitch
)

add_ipo(${PROJECT_NAME})

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
