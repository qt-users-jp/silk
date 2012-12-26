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

function JsonParser() {
    this.m_reserved = ['true', 'false', 'null']
}

JsonParser.prototype = {
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
        case '[':
            str = this.advance()
            type = 'square parentheses begin'
            break
        case ']':
            str = this.advance()
            type = 'square parentheses end'
            break
        case ':':
            str = this.advance()
            type = 'colon'
            break
        case ',':
            str = this.advance()
            type = 'comma'
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
        case '"':
//            str += this.advance() // "
            while (this.m_current && !(this.m_current !== '\\' && this.next() === '"')) {
                str += this.advance()
            }
            str += this.advance() // ?
            str += this.advance() // "
            type = 'literal'
            break
        default:
            if (/[a-z]/.test(this.m_current)) {
                while (this.m_current && this.m_current.match(/[a-z]/)) {
                    str += this.advance()
                }
                if (this.m_reserved.indexOf(str) < 0)
                    type = 'other'
                else
                    type = 'keyword'
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
