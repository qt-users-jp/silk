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

import QtQuick.CSS 3.0

Rule {
    Rule {
        selector: '.button_group'
        text_align: 'center'
        property string width: '90%'
        property string margin: '0 5%'
    }

    Rule {
        selector: '.button'

        property string margin: '10px'
        text_decoration: 'none'
        display: 'inline-block'
        text_align: 'center'

        border: '1px solid rgba(0, 0, 0, 0.3)'

        property string text_shadow: '0 1px 0 rgba(0, 0, 0, 0.4)'

        property string box_shadow: '0 0 .05em rgba(0,0,0,0.4)'
        property string _moz_box_shadow: box_shadow
        property string _webkit_box_shadow: box_shadow

        property string border_radius: '.3em'
        property string _moz_border_radius: border_radius

        background: '-webkit-gradient(linear, left top, left bottom, from(#80C342), to(#006225) )'

        Rule {
            selector: 'span'
            property string border_radius: '.3em'
            property string _moz_border_radius: border_radius

            font: "bold 1.5em 'Trebuchet MS',Arial, Helvetica"
            border_top: '1px solid rgba(255, 255, 255, 0.5)'
            display: 'block'
            property string padding: '0.5em 1.5em'
            color: '#f0ffe1'
        }

        Rule {
            selector: ':hover'

            background: '-webkit-gradient(linear, left top, left bottom, from(#006225), to(#80C342) )'
            property string box_shadow: '0 0 .1em rgba(0,0,0,0.4)'
            property string _moz_box_shadow: box_shadow
            property string _webkit_box_shadow: box_shadow
        }


        Rule {
            selector: ':active'

            background: '#006225'

            property string position: 'relative'
            property string top: '1px'
            property string left: '1px'
        }
    }

    Rule {
        selector: '.button_disabled'

        property string margin: '10px'
        text_decoration: 'none'
        display: 'inline-block'
        text_align: 'center'

        border: '1px solid rgba(0, 0, 0, 0.3)'

        property string text_shadow: '0 1px 0 rgba(0, 0, 0, 0.4)'

        property string box_shadow: '0 0 .05em rgba(0,0,0,0.4)'
        property string _moz_box_shadow: box_shadow
        property string _webkit_box_shadow: box_shadow

        property string border_radius: '.3em'
        property string _moz_border_radius: border_radius

        background: '-webkit-gradient(linear, left top, left bottom, from(#C3C3C3), to(#626262) )'

        Rule {
            selector: 'span'
            property string border_radius: '.3em'
            property string _moz_border_radius: border_radius

            font: "bold 1.5em 'Trebuchet MS',Arial, Helvetica"
            border_top: '1px solid rgba(255, 255, 255, 0.5)'
            display: 'block'
            property string padding: '0.5em 1.5em'
            color: '#BCC8B1'
        }
    }
}
