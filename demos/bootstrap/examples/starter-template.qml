/* Copyright (c) 2012 - 2013 Silk Project.
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

import Silk.HTML 5.0 as HTML
import Silk.Bootstrap 2.3 as Bootstrap
import Silk.Utils 1.0 as Utils
import QtQml 2.1 as QML
import QtQml.Models 2.1 as Models

Template {
    title: 'Bootstrap, from Twitter'
    Navbar {
        Bootstrap.Nav {
            Utils.Repeater {
                model: Models.ListModel {
                    Models.ListElement {
                        href: '#'
                        text: 'Home'
                    }
                    Models.ListElement {
                        href: '#about'
                        text: 'About'
                    }
                    Models.ListElement {
                        href: '#contact'
                        text: 'Contact'
                    }
                }

                QML.Component {
                    Bootstrap.NavItem {
                        _class: model.index === 0 ? 'active': ''
                        a.href: model.href
                        a.text: model.text
                    }
                }
            }
        }
    }

    Bootstrap.Container {
        HTML.H1 { text: 'Bootstrap starter template' }
        HTML.P { text: 'Use this document as a way to quick start any new project.<br> All you get is this message and a barebones HTML document.' }
    }
}
