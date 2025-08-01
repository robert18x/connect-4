project(core)

add_library(${PROJECT_NAME} INTERFACE
    utilities/int_types.h
    game/Bot.h
    game/Board.h
    game/Game.h
)

target_include_directories(${PROJECT_NAME} INTERFACE .)

target_add_custom_compile_flags(${PROJECT_NAME})

find_package(Threads REQUIRED)
find_package(magic_enum REQUIRED)
find_package(inplace_vector REQUIRED)

target_link_libraries(${PROJECT_NAME} INTERFACE
    magic_enum::magic_enum
    Threads::Threads
    inplace_vector::inplace_vector
)

add_ipo(${PROJECT_NAME})
