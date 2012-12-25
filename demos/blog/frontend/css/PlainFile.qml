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

Rule {
    selector: '.file'
    font_family: 'monospace'
    property string line_height: '1.2'
    border: '4px solid %1'.arg(css.darkColor)
    background: 'white'
    property string padding: '10px'
    property string _webkit_box_shadow: '0 5px 10px #aaa'
    property string _moz_box_shadow: _webkit_box_shadow
    property string box_shadow: _webkit_box_shadow
    property string overflow: 'auto'

    Rule {
        selector: ' .number'
        color: '#000000'
    }
    Rule {
        selector: ' .literal'
        color: '#339933'
    }
    Rule {
        selector: ' .key'
        color: '#993333'
    }

    Rule {
        selector: ' .keyword'
        color: '#999933'
    }

    Rule {
        selector: ' .element'
        color: '#993399'
    }

    Rule {
        selector: ' .namespace'
        color: '#993399'
    }

    Rule {
        selector: ' .property'
        color: '#993333'
    }

    Rule {
        selector: ' .comment'
        color: '#339933'
    }

    Rule {
        selector: ' .blah3'
        color: '#339933'
    }
}
