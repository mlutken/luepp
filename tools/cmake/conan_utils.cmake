# -----------------------
# --- Conan functions ---
# -----------------------

if (NOT CONAN_COMMAND)
    set (CONAN_COMMAND conan)
endif()


function(conan_utils_get_profile_name out_profile_name)
    set (profile_name "default")
    if (CONAN_PROFILE_NAME)
        set (profile_name ${CONAN_PROFILE_NAME})
    endif()
    set(${out_profile_name} ${profile_name} PARENT_SCOPE)
endfunction()


function(conan_utils_get_profile_path profile_name out_profile_path)
    if(EXISTS "${profile_name}")    # Are we given a path to a profile file?
        set(${out_profile_path} ${profile_name} PARENT_SCOPE)
    else()
        execute_process(COMMAND ${CONAN_COMMAND} profile path ${profile_name}
                        RESULT_VARIABLE return_code
                        OUTPUT_VARIABLE conan_stdout
                        ERROR_VARIABLE conan_stderr
                        ECHO_ERROR_VARIABLE    # show the text output regardless
                        ECHO_OUTPUT_VARIABLE
                        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        )

        string(STRIP ${conan_stdout} conan_stdout)
        set(${out_profile_path} ${conan_stdout} PARENT_SCOPE)
        if(NOT ${return_code} EQUAL "0")
            message(STATUS "CMake-Conan: The default profile doesn't exist! Please create it using '$ conan profile detect'.")
        endif()
    endif()
endfunction()


function(conan_utils_get_profile_settings out_profile_settings)
    conan_utils_get_profile_name(profile_name)
    conan_utils_get_profile_path(${profile_name} profile_path)

    FILE(READ "${profile_path}" contents)
    STRING(REGEX REPLACE ";" "\\\\;" contents "${contents}")
    STRING(REGEX REPLACE "\n" ";" contents "${contents}")

    set (profile_settings "")
    foreach(line ${contents})
        if(line MATCHES "^build_type=.*")
            if (NOT "${CMAKE_BUILD_TYPE}" STREQUAL "")
                set (line "build_type=${CMAKE_BUILD_TYPE}")
            endif()
            set(profile_settings ${profile_settings} ${line})
        elseif(line MATCHES "^compiler=.*")
            set(profile_settings ${profile_settings} ${line} )
        elseif(line MATCHES "^compiler\.version=.*")
            set(profile_settings ${profile_settings} ${line} )
        elseif(line MATCHES "^compiler\.libcxx=.*")
            set(profile_settings ${profile_settings} ${line} )
        elseif(line MATCHES "^compiler\.cppstd=.*")
            set(profile_settings ${profile_settings} ${line} )
       elseif(line MATCHES "^arch=.*")
           set(profile_settings ${profile_settings} ${line} )
        endif()
    endforeach()

    set (${out_profile_settings} ${profile_settings} PARENT_SCOPE)

endfunction()


function(conan_utils_cmake_install)
    if(DEFINED CONAN_COMMAND)
        set(CONAN_CMD ${CONAN_COMMAND})
    else()
        conan_check(REQUIRED)
    endif()

    set(installOptions UPDATE NO_IMPORTS OUTPUT_QUIET ERROR_QUIET)
    set(installOneValueArgs PATH_OR_REFERENCE REFERENCE REMOTE LOCKFILE LOCKFILE_OUT LOCKFILE_NODE_ID INSTALL_FOLDER OUTPUT_FOLDER)
    set(installMultiValueArgs GENERATOR BUILD ENV ENV_HOST ENV_BUILD OPTIONS_HOST OPTIONS OPTIONS_BUILD PROFILE
                              PROFILE_HOST PROFILE_BUILD SETTINGS SETTINGS_HOST SETTINGS_BUILD)
    cmake_parse_arguments(ARGS "${installOptions}" "${installOneValueArgs}" "${installMultiValueArgs}" ${ARGN})
    foreach(arg ${installOptions})
        if(ARGS_${arg})
            set(${arg} ${${arg}} ${ARGS_${arg}})
        endif()
    endforeach()
    foreach(arg ${installOneValueArgs})
        if(DEFINED ARGS_${arg})
            if("${arg}" STREQUAL "REMOTE")
                set(flag "--remote")
            elseif("${arg}" STREQUAL "LOCKFILE")
                set(flag "--lockfile")
            elseif("${arg}" STREQUAL "LOCKFILE_OUT")
                set(flag "--lockfile-out")
            elseif("${arg}" STREQUAL "LOCKFILE_NODE_ID")
                set(flag "--lockfile-node-id")
            elseif("${arg}" STREQUAL "INSTALL_FOLDER")
                set(flag "--install-folder")
            elseif("${arg}" STREQUAL "OUTPUT_FOLDER")
                set(flag "--output-folder")
            endif()
            set(${arg} ${${arg}} ${flag} ${ARGS_${arg}})
        endif()
    endforeach()
    foreach(arg ${installMultiValueArgs})
        if(DEFINED ARGS_${arg})
            if("${arg}" STREQUAL "GENERATOR")
                set(flag "--generator")
            elseif("${arg}" STREQUAL "BUILD")
                set(flag "--build")
            elseif("${arg}" STREQUAL "ENV")
                set(flag "--env")
            elseif("${arg}" STREQUAL "ENV_HOST")
                set(flag "--env:host")
            elseif("${arg}" STREQUAL "ENV_BUILD")
                set(flag "--env:build")
            elseif("${arg}" STREQUAL "OPTIONS")
                set(flag "--options")
            elseif("${arg}" STREQUAL "OPTIONS_HOST")
                set(flag "--options:host")
            elseif("${arg}" STREQUAL "OPTIONS_BUILD")
                set(flag "--options:build")
            elseif("${arg}" STREQUAL "PROFILE")
                set(flag "--profile")
            elseif("${arg}" STREQUAL "PROFILE_HOST")
                set(flag "--profile:host")
            elseif("${arg}" STREQUAL "PROFILE_BUILD")
                set(flag "--profile:build")
            elseif("${arg}" STREQUAL "SETTINGS")
                set(flag "--settings")
            elseif("${arg}" STREQUAL "SETTINGS_HOST")
                set(flag "--settings:host")
            elseif("${arg}" STREQUAL "SETTINGS_BUILD")
                set(flag "--settings:build")
            endif()
            list(LENGTH ARGS_${arg} numargs)
            foreach(item ${ARGS_${arg}})
                if(${item} STREQUAL "all" AND ${arg} STREQUAL "BUILD")
                    set(${arg} "--build")
                    break()
                endif()
                set(${arg} ${${arg}} ${flag} ${item})
            endforeach()
        endif()
    endforeach()
    if(DEFINED UPDATE)
        set(UPDATE --update)
    endif()
    if(DEFINED NO_IMPORTS)
        set(NO_IMPORTS --no-imports)
    endif()
    set(install_args install ${PATH_OR_REFERENCE} ${REFERENCE} ${UPDATE} ${NO_IMPORTS} ${REMOTE} ${LOCKFILE} ${LOCKFILE_OUT} ${LOCKFILE_NODE_ID} ${INSTALL_FOLDER} ${OUTPUT_FOLDER}
                                ${GENERATOR} ${BUILD} ${ENV} ${ENV_HOST} ${ENV_BUILD} ${OPTIONS} ${OPTIONS_HOST} ${OPTIONS_BUILD}
                                ${PROFILE} ${PROFILE_HOST} ${PROFILE_BUILD} ${SETTINGS} ${SETTINGS_HOST} ${SETTINGS_BUILD})

    string(REPLACE ";" " " _install_args "${install_args}")
    message(STATUS "Conan executing: ${CONAN_CMD} ${_install_args}")

    if(ARGS_OUTPUT_QUIET)
      set(OUTPUT_OPT OUTPUT_QUIET)
    endif()
    if(ARGS_ERROR_QUIET)
      set(ERROR_OPT ERROR_QUIET)
    endif()

    execute_process(COMMAND ${CONAN_CMD} ${install_args}
                    RESULT_VARIABLE return_code
                    ${OUTPUT_OPT}
                    ${ERROR_OPT}
                    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

    if(NOT "${return_code}" STREQUAL "0")
        if (ARGS_ERROR_QUIET)
            message(WARNING "Conan install failed='${return_code}'")
        else()
            message(FATAL_ERROR "Conan install failed='${return_code}'")
        endif()
    endif()

endfunction()
