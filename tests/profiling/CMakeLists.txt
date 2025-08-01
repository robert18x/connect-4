project(profiling)

add_executable(${PROJECT_NAME}
    main.cpp
)

target_add_custom_compile_flags(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/src/core")

find_package(Threads REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE
    core
    Threads::Threads
)

add_ipo(${PROJECT_NAME})

add_custom_target("callgrind-${PROJECT_NAME}"
    COMMAND valgrind --tool=callgrind --dump-instr=yes --collect-jumps=yes ./${PROJECT_NAME} || exit 0
    DEPENDS "${PROJECT_NAME}"
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    COMMENT "Profiling ${PROJECT_NAME} with Valgrind callgrind"
)
