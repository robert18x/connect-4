function(win_deploy_qt TARGET_NAME)

endfunction()


function(deploy_qt TARGET_NAME)
    if(WIN32)
        win_deploy_qt(${TARGET_NAME})
    else()

    endif()
endfunction()
