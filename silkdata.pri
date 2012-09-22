isEmpty(SILKDATA_PRI) { SILKDATA_PRI = 1

    include(./silk.pri)

#    defineReplace(cleanPath) {
#        win32:1 ~= s|\\\\|/|g
#        contains(1, ^/.*):pfx = /
#        else:pfx =
#        segs = $$split(1, /)
#        out =
#        for(seg, segs) {
#            equals(seg, ..):out = $$member(out, 0, -2)
#            else:!equals(seg, .):out += $$seg
#        }
#        return($$join(out, /, $$pfx))
#    }

    # For use in custom compilers which just copy files
    win32:i_flag = i
    defineReplace(stripSrcDir) {
        win32 {
            !contains(1, ^.:.*):1 = $$OUT_PWD/$$1
        } else {
            !contains(1, ^/.*):1 = $$OUT_PWD/$$1
        }
        out = $$cleanPath($$1)
        out ~= s|^$$re_escape($$PWD/)||$$i_flag
        return($$out)
    }

    copy_files.path = $$SILK_BUILD_TREE/$$SILK_TARGET_PATH/$$SILK_DATA_PATH/$$COPY_TARGET
    copy_files.input = COPY_FILES
    copy_files.output = $$SILK_BUILD_TREE/$$SILK_TARGET_PATH/$$SILK_DATA_PATH/$$COPY_TARGET/${QMAKE_FUNC_FILE_IN_stripSrcDir}
    copy_files.commands = $$QMAKE_COPY ${QMAKE_FILE_IN} ${QMAKE_FILE_OUT}
    copy_files.name = COPY $$COPY_FILES
    copy_files.CONFIG += no_link
    QMAKE_EXTRA_COMPILERS += copy_files
    PRE_TARGETDEPS += compiler_copy_files_make_all

    install_files.files = $$COPY_FILES
    install_files.path = $$PREFIX/$$SILK_TARGET_PATH/$$SILK_DATA_PATH/$$COPY_TARGET
    INSTALLS += install_files

} # SILKDATA_PRI
