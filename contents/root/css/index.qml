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

    Rule {
        selector: 'body'
        property string overflow_y: 'scroll'
//        property string padding_top: '60px'
        property string padding_bottom: '60px'
    }

    Rule {
        selector: 'section'
        property string padding_top: '40px'
    }

    Rule {
        selector: '.silk-sidenav'
        property string width: '228px'
        property string margin: '60px 0 0'
        property string padding: '0'
        background_color: '#FFF'
        property string _webkit_border_radius: border_radius
        property string _moz_border_radius: border_radius
        border_radius: '6px'
        property string _webkit_box_shadow: box_shadow
        property string _moz_box_shadow: box_shadow
        box_shadow: '0 1px 4px rgba(0,0,0,.065)'

        Rule {
            selector: '> li > a'
            display: 'block'
            property string width: '190px \\9'
            property string margin: '0 0 -1px'
            property string padding: '8px 14px'
            property string border: '1px solid #e5e5e5'

            Rule {
                selector: '> .icon-chevron-right'
                property string _float: 'right'
                property string margin_top: '2px'
                property string margin_right: '-6px'
                opacity: '.25'
            }
        }

        Rule {
            selector: '> li:first-child > a'
            property string _webkit_border_radius: border_radius
            property string _moz_border_radius: border_radius
            border_radius: '6px 6px 0 0'
        }

        Rule {
            selector: '> li:last-child > a'
            property string _webkit_border_radius: border_radius
            property string _moz_border_radius: border_radius
            border_radius: '0 0 6px 6px'
        }
    }
}
