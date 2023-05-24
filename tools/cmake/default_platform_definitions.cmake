# --------------------
# --- Set defaults ---
# --------------------
if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set_default (LUEPP_PLATFORM_TYPE_NAME    "desktop"                          )
    set_default (LUEPP_SYSTEM_NAME           "linux"                            )
    set_default (LUEPP_PLATFORM_TYPE_ID      ${LUEPP_PLATFORM_TYPE_ID_DESKTOP}  )
    set_default (LUEPP_SYSTEM_ID             ${LUEPP_SYSTEM_ID_LINUX}           )
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set_default (LUEPP_PLATFORM_TYPE_NAME    "desktop"                          )
    set_default (LUEPP_SYSTEM_NAME           "windows"                          )
    set_default (LUEPP_PLATFORM_TYPE_ID      ${LUEPP_PLATFORM_TYPE_ID_DESKTOP}  )
    set_default (LUEPP_SYSTEM_ID             ${LUEPP_SYSTEM_ID_WINDOWS}         )
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
    set_default (LUEPP_PLATFORM_TYPE_NAME    "desktop"                          )
    set_default (LUEPP_SYSTEM_NAME           "browser"                          )
    set_default (LUEPP_PLATFORM_TYPE_ID      ${LUEPP_PLATFORM_TYPE_ID_WEB}      )
    set_default (LUEPP_SYSTEM_ID             ${LUEPP_SYSTEM_ID_BROWSER}         )
# TODO: Other platforms like osx, FreeRTOS
endif ()
