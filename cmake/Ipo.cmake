include(CheckIPOSupported)

# adds interprocedural optimization, when is suppored by the compiler
function(add_ipo target)
    check_ipo_supported(RESULT result OUTPUT output)
    if(result)
        set_property(TARGET ${target} PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
    else()
        message(WARNING "IPO is not supported: ${output}")
    endif()
endfunction()