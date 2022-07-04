add_definitions(
    -DLUEPP_PLATFORM_TYPE_ID=${LUEPP_PLATFORM_TYPE_ID}
    -DLUEPP_SYSTEM_ID=${LUEPP_SYSTEM_ID}
    -DLUEPP_AUDIO_SYSTEM_ID=${LUEPP_AUDIO_SYSTEM_ID}
    -DLUEPP_GRAPHICS_SYSTEM_ID=${LUEPP_GRAPHICS_SYSTEM_ID}
    -DLUEPP_GUI_SYSTEM_ID=${LUEPP_GUI_SYSTEM_ID}
    -DLUEPP_FILE_SYSTEM_ID=${LUEPP_FILE_SYSTEM_ID}
)

message ("LUEPP_SYSTEM_NAME          : '${LUEPP_SYSTEM_NAME}'")
message ("LUEPP_AUDIO_SYSTEM_NAME    : '${LUEPP_AUDIO_SYSTEM_NAME}'")
message ("LUEPP_GRAPHICS_SYSTEM_NAME : '${LUEPP_GRAPHICS_SYSTEM_NAME}'")
message ("LUEPP_GUI_SYSTEM_NAME      : '${LUEPP_GUI_SYSTEM_NAME}'")
message ("LUEPP_FILE_SYSTEM_NAME     : '${LUEPP_FILE_SYSTEM_NAME}'")
