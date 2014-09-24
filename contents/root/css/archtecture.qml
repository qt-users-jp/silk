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

import Silk.CSS 3.0

Css {
    id: css
    property bool requestHeaderAvailable: typeof(http.requestHeader['user-agent']) === 'string'
    property bool firefox: requestHeaderAvailable && http.requestHeader['user-agent'].indexOf('Firefox') > -1
    property bool msie: requestHeaderAvailable && http.requestHeader['user-agent'].indexOf('Trident') > -1

    Rule {
        id: root
        Rule {
            selector: '.architecture_block'

            property string margin: '10px'
            property string padding: '1.0em 0em'
            property string border_radius: '.3em'
            property string _moz_border_radius: border_radius

            font: "bold 1.5em 'Trebuchet MS',Arial, Helvetica"
            text_align: 'center'
        }

        Rule {
            selector: '.arch_html'
            background: '#F16427'
            color: 'white'
        }

        Rule {
            selector: '.arch_js'
            background: '#029A9C'
            color: 'white'
        }

        Rule {
            selector: '.arch_css'
            background: '#27A7E2'
            color: 'white'
        }

        Rule {
            selector: '.arch_qml'
            background: css.firefox ? '-moz-linear-gradient(left, #80C342, #029A9C)' : css.msie ? 'linear-gradient(to right, #80C342, #029A9C)' : '-webkit-gradient(linear, center center, right center, from(#80C342), to(#029A9C) )'
            color: 'white'
        }

        Rule {
            selector: '.arch_plangs'
            background: css.firefox ? '-moz-linear-gradient(left, #B60800, #F30100)' : css.msie ? 'linear-gradient(to right, #B60800, #F30100)' : '-webkit-gradient(linear, left top, right bottom, from(#B60800), to(#F30100) )'
            color: 'white'
        }
        Rule {
            selector: '.arch_server'
            background: css.firefox ? '-moz-linear-gradient(left, #FD00CB, #1A3F98)' : css.msie ? 'linear-gradient(to right, #FD00CB, #1A3F98)' : '-webkit-gradient(linear, left top, right bottom, from(#FD00CB), to(#1A3F98) )'
            color: 'white'
        }
        Rule {
            selector: '.arch_qt'
            background: '#80C342'
            color: 'white'
        }
        Rule {
            selector: '.arch_op50'
            opacity: '0.50'
            color: '#ccc'
        }
    }
}
