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

function CppParser() {
    this.m_reserved = ['auto'
                       , 'true'
                       , 'false'
                       , 'for'
                       , 'foreach'
                       , 'if'
                       , 'else'
                       , 'while'
                       , 'switch'
                       , 'case'
                       , 'break'
                       , 'return'
                       , 'continue'
                       , 'new'
                       , 'delete'
                       , 'emit'
                       , 'SIGNAL'
                       , 'SLOT'
                       , 'void'
                       , 'bool'
                       , 'int'
                       , 'float'
                       , 'double'
                       , 'unsigned'
                       , 'sizeof'
                       , 'mutable'
                       , 'register'
                       , 'const'
                       , 'static'
                       , 'public'
                       , 'protected'
                       , 'private'
                       , 'signals'
                       , 'slots'
                       , 'explicit'
                       , 'class'
                       , 'struct'
                       , 'typedef'
                       , 'virtual'
         ]
}

CppParser.prototype = {
    parse: function(chars) {
        this.m_pos = 0
        this.m_chars = chars
        this.m_size = chars.length
        this.m_current = chars[0]
        this.m_flag = ''

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
//            this.m_flag = ' '
            break
        case '{':
            str = this.advance()
            type = 'curly brace begin'
            this.m_flag = '{'
            break
        case '}':
            str = this.advance()
            type = 'curly brace end'
            this.m_flag = '}'
            break
        case '(':
            str = this.advance()
            type = 'round parentheses begin'
            this.m_flag = '('
            break
        case ')':
            str = this.advance()
            type = 'round parentheses end'
            this.m_flag = ')'
            break
        case ':':
            str = this.advance()
            type = 'colon'
            this.m_flag = ':'
            break
        case ';':
            str = this.advance()
            type = 'semicolon'
            this.m_flag = ';'
            break
        case '\r':
            str += this.advance()
            while (this.m_current && this.m_current === '\n') {
                str += this.advance()
            }
            type = 'linefeed'
            this.m_flag = '$'
            break
        case '\n':
            str = this.advance()
            type = 'linefeed'
            this.m_flag = '$'
            break
        case '/':
            switch (this.next()) {
            case '/':
                str += this.advance() // /
                while (this.m_current && this.m_current !== '\n') {
                    str += this.advance()
                }
                type = 'comment'
                this.m_flag = '$'
                break
            case '*':
                str += this.advance() // /
                str += this.advance() // *
                while (this.m_current && !(this.m_current === '*' && this.next() === '/')) {
                    str += this.advance()
                }
                str += this.advance() // *
                str += this.advance() // /
                type = 'comment'
                break
            default:
                str = this.advance()
                type = 'operator'
                break
            }
            break
        case '"':
            while (this.m_current && !(this.m_current !== '\\' && this.next() === '"')) {
                str += this.advance()
            }
            str += this.advance() // ?
            str += this.advance() // "
            type = 'literal'
            this.m_flag = '""'
            break
        case '\'':
            while (this.m_current && !(this.m_current !== '\\' && this.next() === '\'')) {
                str += this.advance()
            }
            str += this.advance() // ?
            str += this.advance() // '
            type = 'literal'
            this.m_flag = '\''
            break
        case '<':
            if (this.m_flag === '#') {
                while (this.m_current && this.m_current !== '>') {
                    str += this.advance()
                }
                str += this.advance() // >
                type = 'literal'
                this.m_flag = '\''
            } else {
                str = this.advance()
                type = 'operator'
                this.m_flag = '<'
            }
            break
        case '#':
            if (this.m_flag === '$') {
                str += this.advance()
                while (this.m_current && this.m_current.match(/[A-Za-z0-9_]/)) {
                    str += this.advance()
                }
                type = 'preprocessor'
                this.m_flag = '#'
                break
            }

            // fall through
        default:
            if (/[A-Z]/.test(this.m_current)) {
                while (this.m_current && this.m_current.match(/[A-Za-z0-9_\.]/)) {
                    str += this.advance()
                }
                if (str.toUpperCase() === str)
                    type = 'macro'
                else if (str[0] === 'Q')
                    type = 'class'
                else
                    type = 'String'
                this.m_flag = 'S'
            } else if (/[a-z_]/.test(this.m_current)) {
                while (this.m_current && this.m_current.match(/[A-Za-z0-9_\.]/)) {
                    str += this.advance()
                }
                if (this.m_reserved.indexOf(str) < 0)
                    type = 'string'
                else
                    type = 'keyword'
                this.m_flag = 's'
            } else if (/[0-9\.]/.test(this.m_current)) {
                while (this.m_current && this.m_current.match(/[0-9\.x]/)) {
                    str += this.advance()
                }
                type = 'number'
                this.m_flag = '0'
            } else {
                console.debug(this.m_current)
                var head = this.m_current
                str += this.advance()
                while (this.m_current && this.m_current !== this.m_current) {
                    str += this.advance()
                }
                this.m_flag = '?'
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
