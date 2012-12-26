/* Copyright (c) 2012 Silk Project.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Silk nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL SILK BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
.pragma library

function ProParser() {
    this.m_variables = ['BACKUP_REGISTRATION_FILE_MAEMO'
                        , 'CCFLAG'
                        , 'CONFIG'
                        , 'DEFINES'
                        , 'DEF_FILE'
                        , 'DEPENDPATH'
                        , 'DEPLOYMENT'
                        , 'DEPLOYMENT_PLUGIN'
                        , 'DESTDIR'
                        , 'DESTDIR_TARGET'
                        , 'DISTFILES'
                        , 'DLLDESTDIR'
                        , 'DISTFILES'
                        , 'DSP_TEMPLATE'
                        , 'FORMS'
                        , 'FORMS3'
                        , 'GUID'
                        , 'HEADERS'
                        , 'ICON'
                        , 'INCLUDEPATH'
                        , 'INSTALLS'
                        , 'LEXIMPLS'
                        , 'LEXOBJECTS'
                        , 'LEXSOURCES'
                        , 'LIBS'
                        , 'LITERAL_HASH'
                        , 'MAKEFILE'
                        , 'MAKEFILE_GENERATOR'
                        , 'MOBILITY'
                        , 'MOC_DIR'
                        , 'OBJECTIVE_HEADERS'
                        , 'OBJECTIVE_SOURCES'
                        , 'OBJECTS'
                        , 'OBJECTS_DIR'
                        , 'OBJMOC'
                        , 'OTHER_FILES'
                        , 'PKGCONFIG'
                        , 'POST_TARGETDEPS'
                        , 'PRE_TARGETDEPS'
                        , 'PRECOMPILED_HEADER'
                        , 'PWD'
                        , 'OUT_PWD'
                        , 'QMAKE'
                        , 'QMAKESPEC'
                        , 'QMAKE_APP_FLAG'
                        , 'QMAKE_APP_OR_DLL'
                        , 'QMAKE_AR_CMD'
                        , 'QMAKE_BUNDLE_DATA'
                        , 'QMAKE_BUNDLE_EXTENSION'
                        , 'QMAKE_CC'
                        , 'QMAKE_CFLAGS_DEBUG'
                        , 'QMAKE_CFLAGS_MT'
                        , 'QMAKE_CFLAGS_MT_DBG'
                        , 'QMAKE_CFLAGS_MT_DLL'
                        , 'QMAKE_CFLAGS_MT_DLLDBG'
                        , 'QMAKE_CFLAGS_RELEASE'
                        , 'QMAKE_CFLAGS_SHLIB'
                        , 'QMAKE_CFLAGS_THREAD'
                        , 'QMAKE_CFLAGS_WARN_OFF'
                        , 'QMAKE_CFLAGS_WARN_ON'
                        , 'QMAKE_CLEAN'
                        , 'QMAKE_CXX'
                        , 'QMAKE_CXXFLAGS'
                        , 'QMAKE_CXXFLAGS_DEBUG'
                        , 'QMAKE_CXXFLAGS_MT'
                        , 'QMAKE_CXXFLAGS_MT_DBG'
                        , 'QMAKE_CXXFLAGS_MT_DLL'
                        , 'QMAKE_CXXFLAGS_MT_DLLDBG'
                        , 'QMAKE_CXXFLAGS_RELEASE'
                        , 'QMAKE_CXXFLAGS_SHLIB'
                        , 'QMAKE_CXXFLAGS_THREAD'
                        , 'QMAKE_CXXFLAGS_WARN_OFF'
                        , 'QMAKE_CXXFLAGS_WARN_ON'
                        , 'QMAKE_DISTCLEAN'
                        , 'QMAKE_EXTENSION_SHLIB'
                        , 'QMAKE_EXT_MOC'
                        , 'QMAKE_EXT_UI'
                        , 'QMAKE_EXT_PRL'
                        , 'QMAKE_EXT_LEX'
                        , 'QMAKE_EXT_YACC'
                        , 'QMAKE_EXT_OBJ'
                        , 'QMAKE_EXT_CPP'
                        , 'QMAKE_EXT_H'
                        , 'QMAKE_EXTRA_COMPILERS'
                        , 'QMAKE_EXTRA_TARGETS'
                        , 'QMAKE_FAILED_REQUIREMENTS'
                        , 'QMAKE_FILETAGS'
                        , 'QMAKE_FRAMEWORK_BUNDLE_NAME'
                        , 'QMAKE_FRAMEWORK_VERSION'
                        , 'QMAKE_INCDIR'
                        , 'QMAKE_INCDIR_EGL'
                        , 'QMAKE_INCDIR_OPENGL'
                        , 'QMAKE_INCDIR_OPENGL_ES1'
                        , 'QMAKE_INCDIR_OPENGL_ES2'
                        , 'QMAKE_INCDIR_OPENVG'
                        , 'QMAKE_INCDIR_QT'
                        , 'QMAKE_INCDIR_THREAD'
                        , 'QMAKE_INCDIR_X11'
                        , 'QMAKE_INFO_PLIST'
                        , 'QMAKE_LFLAGS'
                        , 'QMAKE_LFLAGS_CONSOLE'
                        , 'QMAKE_LFLAGS_CONSOLE_DLL'
                        , 'QMAKE_LFLAGS_DEBUG'
                        , 'QMAKE_LFLAGS_PLUGIN'
                        , 'QMAKE_LFLAGS_RPATH'
                        , 'QMAKE_LFLAGS_QT_DLL'
                        , 'QMAKE_LFLAGS_RELEASE'
                        , 'QMAKE_LFLAGS_SHAPP'
                        , 'QMAKE_LFLAGS_SHLIB'
                        , 'QMAKE_LFLAGS_SONAME'
                        , 'QMAKE_LFLAGS_THREAD'
                        , 'QMAKE_LFLAGS_WINDOWS'
                        , 'QMAKE_LFLAGS_WINDOWS_DLL'
                        , 'QMAKE_LIBDIR'
                        , 'QMAKE_LIBDIR_FLAGS'
                        , 'QMAKE_LIBDIR_EGL'
                        , 'QMAKE_LIBDIR_OPENGL'
                        , 'QMAKE_LIBDIR_OPENVG'
                        , 'QMAKE_LIBDIR_QT'
                        , 'QMAKE_LIBDIR_X11'
                        , 'QMAKE_LIBS'
                        , 'QMAKE_LIBS_CONSOLE'
                        , 'QMAKE_LIBS_EGL'
                        , 'QMAKE_LIBS_OPENGL'
                        , 'QMAKE_LIBS_OPENGL_QT'
                        , 'QMAKE_LIBS_OPENGL_ES1'
                        , 'QMAKE_LIBS_OPENGL_ES2'
                        , 'QMAKE_LIBS_OPENVG'
                        , 'QMAKE_LIBS_QT'
                        , 'QMAKE_LIBS_QT_DLL'
                        , 'QMAKE_LIBS_QT_OPENGL'
                        , 'QMAKE_LIBS_QT_THREAD'
                        , 'QMAKE_LIBS_RT'
                        , 'QMAKE_LIBS_RTMT'
                        , 'QMAKE_LIBS_THREAD'
                        , 'QMAKE_LIBS_WINDOWS'
                        , 'QMAKE_LIBS_X11'
                        , 'QMAKE_LIBS_X11SM'
                        , 'QMAKE_LIB_FLAG'
                        , 'QMAKE_LINK_SHLIB_CMD'
                        , 'QMAKE_LN_SHLIB'
                        , 'QMAKE_POST_LINK'
                        , 'QMAKE_PRE_LINK'
                        , 'QMAKE_PROJECT_NAME'
                        , 'QMAKE_MAC_SDK'
                        , 'QMAKE_MACOSX_DEPLOYMENT_TARGET'
                        , 'QMAKE_MAKEFILE'
                        , 'QMAKE_MOC_SRC'
                        , 'QMAKE_QMAKE'
                        , 'QMAKE_QT_DLL'
                        , 'QMAKE_RESOURCE_FLAGS'
                        , 'QMAKE_RPATH'
                        , 'QMAKE_RPATHDIR'
                        , 'QMAKE_RUN_CC'
                        , 'QMAKE_RUN_CC_IMP'
                        , 'QMAKE_RUN_CXX'
                        , 'QMAKE_RUN_CXX_IMP'
                        , 'QMAKE_TARGET'
                        , 'QMAKE_UIC'
                        , 'QT'
                        , 'QTPLUGIN'
                        , 'QT_VERSION'
                        , 'QT_MAJOR_VERSION'
                        , 'QT_MINOR_VERSION'
                        , 'QT_PATCH_VERSION'
                        , 'RCC_DIR'
                        , 'RC_FILE'
                        , 'REQUIRES'
                        , 'RESOURCES'
                        , 'RES_FILE'
                        , 'RSS_RULES'
                        , 'SIGNATURE_FILE'
                        , 'SOURCES'
                        , 'SRCMOC'
                        , 'STATECHARTS'
                        , 'SUBDIRS'
                        , 'TEMPLATE'
                        , 'TRANSLATIONS'
                        , 'UICIMPLS'
                        , 'UICOBJECTS'
                        , 'UI_DIR'
                        , 'UI_HEADERS_DIR'
                        , 'UI_SOURCES_DIR'
                        , 'VER_MAJ'
                        , 'VER_MIN'
                        , 'VER_PAT'
                        , 'VERSION'
                        , 'VPATH'
                        , 'YACCIMPLS'
                        , 'YACCOBJECTS'
                        , 'YACCSOURCES'
                        , '_PRO_FILE_'
                        , '_PRO_FILE_PWD_, '
         ]
    this.m_functions = ['basename'
                       , 'contains'
                       , 'count'
                       , 'dirname'
                       , 'error'
                       , 'eval'
                       , 'exists'
                       , 'find'
                       , 'for'
                       , 'include'
                       , 'infile'
                       , 'isEmpty'
                       , 'join'
                       , 'member'
                       , 'message'
                       , 'packagesExist'
                       , 'prompt'
                       , 'quote'
                       , 'replace'
                       , 'sprintf'
                       , 'system'
                       , 'unique'
                       , 'warning'
         ]
}

ProParser.prototype = {
    parse: function(chars) {
        this.m_pos = 0
        this.m_chars = chars
        this.m_size = chars.length
        this.m_current = chars[0]

        var ret = []

        while (this.m_pos < this.m_size ) {
            ret.push(this.lex())
        }
        return ret
    }

    , to_s: function(obj) {
        var r = ''
        for (var i = 0; i < obj.length; i++) {
            r += '[%1] %2'.arg(obj[i].type).arg(obj[i].str)
        }
        return r
    }

    , to_html: function(obj) {
        var r = ''
        for (var i = 0; i < obj.length; i++) {
            var type = obj[i].type
            var str = obj[i].str.replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;').replace(/"/g, '&quot;').replace(/ /g, '&nbsp;')
            switch (type) {
            case 'space':
                r += str
                break
            case 'linefeed':
                r += '<br />'
                break
            default:
                r += '<span class="' + type + '">' + str + '</span>'
                break
            }

        }
        return r
    }

    , lex: function() {
        var str = ''
        var type = 'other'
        switch (this.m_current) {
        case ' ':
            while (this.m_current && this.m_current === ' ') {
                str += this.advance()
            }
            type = 'space'
            break
        case '{':
            str = this.advance()
            type = 'curly brace begin'
            break
        case '}':
            str = this.advance()
            type = 'curly brace end'
            break
        case '(':
            str = this.advance()
            type = 'round parentheses begin'
            break
        case ')':
            str = this.advance()
            type = 'round parentheses end'
            break
        case ':':
            str = this.advance()
            type = 'colon'
            break
        case ';':
            str = this.advance()
            type = 'semicolon'
            break
        case '\r':
            str += this.advance()
            while (this.m_current && this.m_current === '\n') {
                str += this.advance()
            }
            type = 'linefeed'
            break
        case '\n':
            str = this.advance()
            type = 'linefeed'
            break
        case '#':
            str += this.advance() // #
            while (this.m_current && this.m_current !== '\n') {
                str += this.advance()
            }
            type = 'comment'
            break
        case '"':
//            str += this.advance() // "
            while (this.m_current && !(this.m_current !== '\\' && this.next() === '"')) {
                str += this.advance()
            }
            str += this.advance() // ?
            str += this.advance() // "
            type = 'literal'
            break
        case '\'':
//            str += this.advance() // '
            while (this.m_current && !(this.m_current !== '\\' && this.next() === '\'')) {
                str += this.advance()
            }
            str += this.advance() // ?
            str += this.advance() // '
            type = 'literal'
            break
        default:
            if (/[A-Z]/.test(this.m_current)) {
                while (this.m_current && this.m_current.match(/[A-Za-z0-9_\.]/)) {
                    str += this.advance()
                }
                if (this.m_variables.indexOf(str) < 0)
                    type = 'String'
                else
                    type = 'variable'
            } else if (/[a-z_]/.test(this.m_current)) {
                while (this.m_current && this.m_current.match(/[A-Za-z0-9_\.]/)) {
                    str += this.advance()
                }
                if (this.m_functions.indexOf(str) < 0)
                    type = 'string'
                else
                    type = 'function'
            } else if (/[\-0-9\.]/.test(this.m_current)) {
                while (this.m_current && this.m_current.match(/[\-0-9\.]/)) {
                    str += this.advance()
                }
                type = 'number'
            } else {
                console.debug(this.m_current)
                while (this.m_current && this.m_current !== ' ') {
                    str += this.advance()
                }
            }
            break
        }

        return {str: str, type: type}
    }

    , advance: function() {
        var ret = this.m_current
        if (this.m_pos < this.m_size)
            this.m_current = this.m_chars[++this.m_pos]
        else
            this.m_current = undefined
        return ret
    }
    , next: function() {
        if (this.m_pos < this.m_size - 1)
            return this.m_chars[this.m_pos + 1]
        return undefined
    }
}
