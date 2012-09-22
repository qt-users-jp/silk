unix {
    macx {
        CONFIG += silk_platform_mac silk_platform_desktop
        DEFINES += SILK_PLATFORM_MAC SILK_PLATFORM_DESKTOP
    } else {
        CONFIG += silk_platform_linux
        DEFINES += SILK_PLATFORM_LINUX
    }
} else {
    windows {
        CONFIG += silk_platform_windows
        DEFINES += SILK_PLATFORM_WINDOWS
    }
}
