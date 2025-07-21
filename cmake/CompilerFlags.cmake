function(target_add_custom_compile_flags TARGET_NAME)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(COMPILE_FLAGS
            -Wall
            -Wextra
            -Wpedantic
            -Wnon-virtual-dtor
        )
    endif()

    get_target_property(target_type ${TARGET_NAME} TYPE)
    if(target_type STREQUAL "INTERFACE_LIBRARY")
        target_compile_options(${TARGET_NAME} INTERFACE ${COMPILE_FLAGS})
    else()
        target_compile_options(${TARGET_NAME} PRIVATE ${COMPILE_FLAGS})
    endif()
endfunction()


