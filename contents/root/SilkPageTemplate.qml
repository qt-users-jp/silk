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

import Silk.HTML 5.0

Html {
    id: root

    property string subtitle
    default property alias contents: main.contents

    DocType {}

//    Comment { text: '[if lt IE 7]><html class="no-js lt-ie9 lt-ie8 lt-ie7"> <![endif]' } Text { text: "\n" }
//    Comment { text: '[if IE 7]><html class="no-js lt-ie9 lt-ie8"> <![endif]' } Text { text: "\n" }
//    Comment { text: '[if IE 8]><html class="no-js lt-ie9"> <![endif]' } Text { text: "\n" }
//    Comment { text: '[if gt IE 8]><!' } Text { text: '<html class="no-js">' } Comment { text: '<![endif]' } Text { text: "\n" }
    Head {
        Meta { charset: "utf-8" }
        Meta { http_equiv: "X-UA-Compatible"; content: "IE=edge,chrome=1" }
        Title { text: 'silk - %1'.arg(root.subtitle) }
        Meta { name: "description"; content: "" }
        Meta { name: "viewport"; content: "width=device-width" }
        Link { rel: "stylesheet"; href: "/3rdparty/css/normalize.min.css" }
        Link { rel: "stylesheet"; href: "/css/" }
        Script { type: 'text/javascript'; src: "/3rdparty/js/modernizr-2.6.1-respond-1.1.0.min.js" }
        Script { type: 'text/javascript'; src: "/js/gin.js" }
        Script { type: 'text/javascript'; src: "/js/highlighter.js" }
    }

    Body {
        onload: "highlight()"
        Comment { text: '[if lt IE 7]' +
            '<p class="chromeframe">You are using an outdated browser. <a href="http://browsehappy.com/">Upgrade your browser today</a> or <a href="http://www.google.com/chromeframe/?redirect=true">install Google Chrome Frame</a> to better experience this site.</p>' +
        '<![endif]' }

        Div {
            _class: "header-container"
            Header {
                _class: "wrapper clearfix"
                H1 {
                    _class: "title"
                    A {
                        href: '/'
                        text: 'silk'
                    }

                    Text {
                        enabled: root.subtitle.length > 0
                        text: ' - %1'.arg(root.subtitle)
                    }
                }

                Nav {
                    Ul {
                        Li { A { href: "/try.qml"; text: "1. Try now!" } }
                        Li { A { href: "/tutorials.qml"; text: "2. Tutorials" } }
                        Li { A { href: "/examples/"; text: "3. Examples" } }
                    }
                }
            }
        }

        Div {
            _class: "main-container"
            Div {
                _class: "main wrapper clearfix"

                Div {
                    id: main
                    property string style: "width: 100%"
                }

                Aside {

                    H3 { text: "Contents" }
                    Ul {
                        Li {
                            A { href: '/'; text: "Introduction" }
                            Ul {
                                Li { A { href: '/hellosilk.qml'; text: "Hello silk world" } }
                                Li { A { href: '/features.qml'; text: "Features" } }
                            }
                        }
                        Li {
                            A { href: '/try.qml'; text: "Installation" }
                            Ul {
                                Li { A { href: '/config.qml'; text: "Configration" } }
                            }
                        }
                        Li {
                            A { href: '/tutorials.qml'; text: "Tutorials" }
                            Ul {
                                Li { text: "Contents" }
                                Li { text: "Service" }
                            }
                        }
                        Li { A { href: '/examples/'; text: "Examples" } }
                    }
                }

                Section {
                    enabled: http.host == "silk.qtquick.me"
                    Div { _id: "disqus_thread" }
                    Script {
                        type: 'text/javascript'
                        text: "/* * * CONFIGURATION VARIABLES: EDIT BEFORE PASTING INTO YOUR WEBPAGE * * */
var disqus_shortname = 'silkqml'; // required: replace example with your forum shortname

/* * * DON'T EDIT BELOW THIS LINE * * */
(function() {
    var dsq = document.createElement('script'); dsq.type = 'text/javascript'; dsq.async = true;
    dsq.src = 'http://' + disqus_shortname + '.disqus.com/embed.js';
    (document.getElementsByTagName('head')[0] || document.getElementsByTagName('body')[0]).appendChild(dsq);
})();"
                    }
                    NoScript { text: 'Please enable JavaScript to view the <a href="http://disqus.com/?ref_noscript">comments powered by Disqus.</a>' }
                    A { href: "http://disqus.com"; _class: "dsq-brlink"; text: 'comments powered by <span class="logo-disqus">Disqus</span>' }
                }
            }
        }

        Div {
            _class: "footer-container"
            Footer {
                _class: "wrapper"
                H3 {
                    A {
                        href: 'http://silk.qtquick.me/'
                        text: "Copyright (c) 2012 - %1 Silk Project. All rights reserved.".arg(Qt.formatDate(new Date(), 'yyyy'))
                    }
                }
            }
        }

        GoogleAnalytics {
            enabled: http.host === "silk.qtquick.me"
            __trackingCode: 'UA-33461556-1'
        }
    }
//    Text { text: "\n</html>" }
}
