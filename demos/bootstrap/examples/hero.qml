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
            Bootstrap.NavDropdown {
                a.href: '#'
                a.text: 'Dropdown <b class="caret"></b>'
                Utils.Repeater {
                    model: Models.ListModel {
                        Models.ListElement {
                            href: '#'
                            text: 'Action'
                        }
                        Models.ListElement {
                            href: '#'
                            text: 'Another action'
                        }
                        Models.ListElement {
                            href: '#'
                            text: 'Something else here'
                        }
                    }

                    QML.Component {
                        Bootstrap.NavItem {
                            a.href: model.href
                            a.text: model.text
                        }
                    }
                }
                Bootstrap.Divider {}
                Bootstrap.NavHeader { text: 'Nav header' }
                Utils.Repeater {
                    model: Models.ListModel {
                        Models.ListElement {
                            href: '#'
                            text: 'Separated link'
                        }
                        Models.ListElement {
                            href: '#'
                            text: 'One more separated link'
                        }
                    }

                    QML.Component {
                        Bootstrap.NavItem {
                            a.href: model.href
                            a.text: model.text
                        }
                    }
                }
            }
        }
        Bootstrap.NavbarForm {
            _class: 'pull-right'
            HTML.Input {
                _class: 'span2'
                type: 'text'
                placeholder: 'Email'
            }
            HTML.Text { text: ' ' }
            HTML.Input {
                _class: 'span2'
                type: 'password'
                placeholder: 'Password'
            }
            HTML.Text { text: ' ' }
            HTML.Button {
                _class: 'btn'
                type: 'submit'
                text: 'Sign in'
            }
        }
    }

    Bootstrap.Container {
        Bootstrap.HeroUnit {
            heading: 'Hello, world!'
            tagline: 'This is a template for a simple marketing or informational website. It includes a large callout called the hero unit and three supporting pieces of content. Use it as a starting point to create something more unique.'
            HTML.P {
                Bootstrap.ABtnPrimary {
                    _class: 'btn-large'
                    href: '#'
                    text: 'Learn more &raquo;'
                }
            }
        }

        Bootstrap.Row {
            Utils.Repeater {
                model: Models.ListModel {
                    Models.ListElement {
                        title: 'Heading'
                        description: 'Donec id elit non mi porta gravida at eget metus. Fusce dapibus, tellus ac cursus commodo, tortor mauris condimentum nibh, ut fermentum massa justo sit amet risus. Etiam porta sem malesuada magna mollis euismod. Donec sed odio dui. '
                        href: '#'
                        text: 'View details &raquo;'
                    }
                    Models.ListElement {
                        title: 'Heading'
                        description: 'Donec id elit non mi porta gravida at eget metus. Fusce dapibus, tellus ac cursus commodo, tortor mauris condimentum nibh, ut fermentum massa justo sit amet risus. Etiam porta sem malesuada magna mollis euismod. Donec sed odio dui. '
                        href: '#'
                        text: 'View details &raquo;'
                    }
                    Models.ListElement {
                        title: 'Heading'
                        description: 'Donec sed odio dui. Cras justo odio, dapibus ac facilisis in, egestas eget quam. Vestibulum id ligula porta felis euismod semper. Fusce dapibus, tellus ac cursus commodo, tortor mauris condimentum nibh, ut fermentum massa justo sit amet risus.'
                        href: '#'
                        text: 'View details &raquo;'
                    }
                }

                QML.Component {
                    Bootstrap.Span4 {
                        HTML.H2 { text: model.title }
                        HTML.P { text: model.description }
                        HTML.P { Bootstrap.ABtn { href: model.href; text: model.text } }
                    }
                }
            }
        }

        HTML.Hr {}

        HTML.Footer {
            HTML.P { text: '&copy; Company 2013' }
        }
    }
}
