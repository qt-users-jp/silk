isEmpty(SILKRPATH_PRI) { SILKRPATH_PRI = 1

    silk_platform_mac {
        QMAKE_LFLAGS_SONAME = -Wl,-install_name,@rpath/PlugIns/
        QMAKE_LFLAGS += -Wl,-rpath,@loader_path/../
    }

    silk_platform_linux {
        # do the rpath by hand since it's not possible to use ORIGIN in QMAKE_RPATHDIR
        # this expands to $ORIGIN (after qmake and make), it does NOT read a qmake var
        SILK_PLUGIN_RPATH = $$join(QMAKE_RPATHDIR, ":")

        QMAKE_LFLAGS += -Wl,-z,origin \'-Wl,-rpath,$${SILK_PLUGIN_RPATH}\'
        QMAKE_RPATHDIR =
    }
}
