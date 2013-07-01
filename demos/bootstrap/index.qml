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

import Silk.HTML 5.0 as HTML
import Silk.Bootstrap 2.3 as Bootstrap
import Silk.Utils 1.0 as Utils
import QtQml 2.1 as QML
import QtQml.Models 2.1 as Models

Template {
    id: root

    HTML.Head {
        HTML.Title { text: 'Bootstrap' }
    }

    HTML.Div {
        _class: 'jumbotron masthead'
        Bootstrap.Container {
            HTML.H1 { text: 'Bootstrap' }
            HTML.P { text: 'Sleek, intuitive, and powerful front-end framework for faster and easier web development.' }
            HTML.P {
                Bootstrap.ABtnPrimary {
                    _class: 'btn-large'
                    href: 'http://twitter.github.io/bootstrap/assets/bootstrap.zip'
                    text: 'Download Bootstrap'
                }
            }
            HTML.Ul {
                _class: 'masthead-links'
                Utils.Repeater {
                    model: Models.ListModel {
                        Models.ListElement {
                            href: 'http://github.com/twitter/bootstrap'
                            text: 'GitHub project'
                        }
                        Models.ListElement {
                            href: '/getting-started.qml#examples'
                            text: 'Examples'
                        }
                        Models.ListElement {
                            href: '/extend.qml'
                            text: 'Extend'
                        }
                    }

                    QML.Component {
                        HTML.Li {
                            HTML.A {
                                href: model.href
                                text: model.text
                            }
                            HTML.Text { text: ' ' }
                        }
                    }
                }
                HTML.Li { text: 'Version 2.3.2' }
            }
        }
    }

    HTML.Div {
        _class: 'bs-docs-social'

        Bootstrap.Container {
            HTML.Ul {
                _class: 'bs-docs-social-buttons'
                HTML.Li {
                    HTML.IFrame {
                        _class: 'github-btn'
                        src: 'http://ghbtns.com/github-btn.html?user=twitter&repo=bootstrap&type=watch&count=true'
                        property string allowtransparency: 'true'
                        property string frameborder: '0'
                        property string scrolling: '0'
                        width: '100px'
                        height: '20px'
                    }
                }
                HTML.Text { text: ' ' }
                HTML.Li {
                    HTML.IFrame {
                        _class: 'github-btn'
                        src: 'http://ghbtns.com/github-btn.html?user=twitter&repo=bootstrap&type=fork&count=true'
                        property string allowtransparency: 'true'
                        property string frameborder: '0'
                        property string scrolling: '0'
                        width: '102px'
                        height: '20px'
                    }
                }
                HTML.Text { text: ' ' }
                HTML.Li {
                    _class: 'folllow-btn'
                    HTML.A {
                        href: 'https://twitter.com/twbootstrap'
                        _class: 'twitter-follow-button'
                        property string data_link_color: '#0069D6'
                        property string data_show_count: 'true'
                        text: 'Follow @twbootstrap'
                    }
                }
                HTML.Text { text: ' ' }
                HTML.Li {
                    _class: 'tweet-btn'
                    HTML.A {
                        href: 'https://twitter.com/share'
                        _class: 'twitter-share-button'
                        property string data_url: 'http://twitter.github.com/bootstrap/'
                        property string data_count: 'horizontal'
                        property string data_via: 'twbootstrap'
                        property string data_related: 'mdo:Creator of Twitter Bootstrap'
                        text: 'Tweet'
                    }
                }
            }
        }
    }

    Bootstrap.Container {
        HTML.Div {
            _class: 'marketing'
            HTML.H1 { text: 'Introducing Bootstrap.' }
            HTML.P {
                _class: 'marketing-byline'
                text: 'Need reasons to love Bootstrap? Look no further.'
            }
            Bootstrap.RowFluid {
                Bootstrap.Span4 {
                    HTML.Img { _class: 'marketing-img'; src: 'http://twitter.github.io/bootstrap/assets/img/bs-docs-twitter-github.png' }
                    HTML.H2 { text: 'By nerds, for nerds.' }
                    HTML.P {
                        text: 'Built at Twitter by <a href="http://twitter.com/mdo">@mdo</a> and <a href="http://twitter.com/fat">@fat</a>, Bootstrap utilizes <a href="http://lesscss.org">LESS CSS</a>, is compiled via <a href="http://nodejs.org">Node</a>, and is managed through <a href="http://github.com">GitHub</a> to help nerds do awesome stuff on the web.'
                    }
                }
                Bootstrap.Span4 {
                    HTML.Img { _class: 'marketing-img'; src: 'http://twitter.github.io/bootstrap/assets/img/bs-docs-responsive-illustrations.png' }
                    HTML.H2 { text: 'Made for everyone.' }
                    HTML.P {
                        text: 'Bootstrap was made to not only look and behave great in the latest desktop browsers (as well as IE7!), but in tablet and smartphone browsers via <a href="./scaffolding.html#responsive">responsive CSS</a> as well.'
                    }
                }
                Bootstrap.Span4 {
                    HTML.Img { _class: 'marketing-img'; src: 'http://twitter.github.io/bootstrap/assets/img/bs-docs-bootstrap-features.png' }
                    HTML.H2 { text: 'Packed with features.' }
                    HTML.P {
                        text: 'A 12-column responsive <a href="./scaffolding.html#gridSystem">grid</a>, dozens of components, <a href="./javascript.html">JavaScript plugins</a>, typography, form controls, and even a <a href="./customize.html">web-based Customizer</a> to make Bootstrap your own.'
                    }
                }
            }
            HTML.Hr { _class: 'soften' }

            HTML.H1 { text: 'Built with Bootstrap.' }
            HTML.P {
                _class: 'marketing-byline'
                text: 'For even more sites built with Bootstrap, <a href="http://builtwithbootstrap.tumblr.com/" target="_blank">visit the unofficial Tumblr</a> or <a href="./getting-started.html#examples">browse the examples</a>.'
            }
            Bootstrap.RowFluid {
                HTML.Ul {
                    _class: 'thumbnails example-sites'
                    Utils.Repeater {
                        model: Models.ListModel {
                            Models.ListElement {
                                href: 'http://soundready.fm/'
                                src: 'http://twitter.github.io/bootstrap/assets/img/example-sites/soundready.png'
                                alt: 'SourndReady.fm'
                            }
                            Models.ListElement {
                                href: 'http://kippt.com/'
                                src: 'http://twitter.github.io/bootstrap/assets/img/example-sites/kippt.png'
                                alt: 'Kippt'
                            }
                            Models.ListElement {
                                href: 'http://www.gathercontent.com/'
                                src: 'http://twitter.github.io/bootstrap/assets/img/example-sites/gathercontent.png'
                                alt: 'Gather Content'
                            }
                            Models.ListElement {
                                href: 'http://www.jshint.com/'
                                src: 'http://twitter.github.io/bootstrap/assets/img/example-sites/jshint.png'
                                alt: 'JS Hint'
                            }
                        }

                        QML.Component {
                            HTML.Li {
                                _class: 'span3'
                                HTML.A {
                                    _class: 'thumbnail'
                                    target: '_blank'
                                    href: model.href

                                    HTML.Img {
                                        src: model.src
                                        alt: model.alt
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
