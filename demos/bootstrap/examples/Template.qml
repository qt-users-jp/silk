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
import Silk.CSS 3.0 as CSS
import Silk.Bootstrap 2.3 as Bootstrap
import QtQml 2.1 as QML

HTML.Html {
    id: html
    lang: 'en'

    property var title
    HTML.Head {
        id: head
        HTML.Meta { charset: 'utf-8' }
        HTML.Title { text: html.title }
        HTML.Meta { name: 'viewport'; content: 'width=device-width, initial-scale=1.0' }
        HTML.Meta { name: 'description'; content: '' }
        HTML.Meta { name: 'author'; content: '' }

        HTML.Comment { text: ' Le styles '}
        HTML.Link { href: 'http://twitter.github.io/bootstrap/assets/css/bootstrap.css'; rel: 'stylesheet' }
        HTML.Style {
            CSS.Rule {
                selector: 'body'
                property string padding_top: '60px'
            }
        }
        HTML.Link { href: 'http://twitter.github.io/bootstrap/assets/css/bootstrap-responsive.css'; rel: 'stylesheet' }

        HTML.Comment { text: ' Le HTML5 shim, for IE6-8 support of HTML5 elements '}
        HTML.Comment { text: '[if lt IE 9]>
      <script src="http://twitter.github.io/bootstrap/assets/js/html5shiv.js"></script>
    <![endif]' }
        HTML.Comment { text: ' Fav and touch icons ' }

        HTML.Link { rel: 'apple-touch-icon-precomposed'; sizes: '144x144'; href: 'http://twitter.github.io/bootstrap/assets/ico/apple-touch-icon-144-precomposed.png' }
        HTML.Link { rel: 'apple-touch-icon-precomposed'; sizes: '114x114'; href: 'http://twitter.github.io/bootstrap/assets/ico/apple-touch-icon-114-precomposed.png' }
        HTML.Link { rel: 'apple-touch-icon-precomposed'; sizes: '72x72'; href: 'http://twitter.github.io/bootstrap/assets/ico/apple-touch-icon-72-precomposed.png' }
        HTML.Link { rel: 'apple-touch-icon-precomposed'; href: 'http://twitter.github.io/bootstrap/assets/ico/apple-touch-icon-57-precomposed.png' }
        HTML.Link { rel: 'shortcut icon'; href: 'http://twitter.github.io/bootstrap/assets/ico/favicon.png' }
    }

    HTML.Body {
        id: body
        HTML.Tag {
            id: main
            QML.Component.onCompleted: {
                for (var i = 0; i < main.contents.length; i++) {
                    var child = main.contents[i]
                    if (child.tagName === 'head') {
                        child.tagName = ''
                        main.takeAt(i--)
                        head.insert(head.contents.length, child)
                    } else if (child.tagName === 'body') {
                        child.tagName = ''
                        main.takeAt(i--)
                        body.insert(body.contents.length, child)
                    }
                }
            }
        }

        HTML.Comment { text: ' Le javascript
    ================================================== ' }
        HTML.Comment { text: ' Placed at the end of the document so the pages load faster ' }
        HTML.Script { src: 'http://twitter.github.io/bootstrap/assets/js/jquery.js' }
        HTML.Script { src: 'http://twitter.github.io/bootstrap/assets/js/bootstrap-transition.js' }
        HTML.Script { src: 'http://twitter.github.io/bootstrap/assets/js/bootstrap-alert.js' }
        HTML.Script { src: 'http://twitter.github.io/bootstrap/assets/js/bootstrap-modal.js' }
        HTML.Script { src: 'http://twitter.github.io/bootstrap/assets/js/bootstrap-dropdown.js' }
        HTML.Script { src: 'http://twitter.github.io/bootstrap/assets/js/bootstrap-scrollspy.js' }
        HTML.Script { src: 'http://twitter.github.io/bootstrap/assets/js/bootstrap-tab.js' }
        HTML.Script { src: 'http://twitter.github.io/bootstrap/assets/js/bootstrap-tooltip.js' }
        HTML.Script { src: 'http://twitter.github.io/bootstrap/assets/js/bootstrap-popover.js' }
        HTML.Script { src: 'http://twitter.github.io/bootstrap/assets/js/bootstrap-button.js' }
        HTML.Script { src: 'http://twitter.github.io/bootstrap/assets/js/bootstrap-collapse.js' }
        HTML.Script { src: 'http://twitter.github.io/bootstrap/assets/js/bootstrap-carousel.js' }
        HTML.Script { src: 'http://twitter.github.io/bootstrap/assets/js/bootstrap-typeahead.js' }
    }
}
