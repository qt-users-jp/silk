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
    id: root

    HTML.Head {
        HTML.Title { text: 'Getting · Bootstrap' }
    }

    JumboTron {
        title: 'Getting started'
        description: 'Overview of the project and how to get started with a simple template.'
    }

    Bootstrap.Container {
        Bootstrap.Row {
            Bootstrap.Span3 {
                _class: 'bs-docs-sidebar'
                Bootstrap.NavList {
                    _class: 'bs-docs-sidenav'
                    Utils.Repeater {
                        model: Models.ListModel {
                            Models.ListElement {
                                href: '#download-bootstrap'
                                text: ' Download'
                            }
                            Models.ListElement {
                                href: '#file-structure'
                                text: ' File structure'
                            }
                            Models.ListElement {
                                href: '#contents'
                                text: ' What\'s included'
                            }
                            Models.ListElement {
                                href: '#html-template'
                                text: ' HTML template'
                            }
                            Models.ListElement {
                                href: '#examples'
                                text: ' Examples'
                            }
                            Models.ListElement {
                                href: '#what-next'
                                text: ' What next?'
                            }
                        }

                        QML.Component {
                            Bootstrap.NavItem {
                                a.href: model.href
                                Bootstrap.IconChevronRight {}
                                HTML.Text { text: model.text }
                            }
                        }
                    }
                }
            }
            Bootstrap.Span9 {
                HTML.Section {
                    _id: 'download-bootstrap'
                    Bootstrap.PageHeader {
                        HTML.H1 { text: '1. Download' }
                    }
                    Bootstrap.Lead {
                        text: 'Before downloading, be sure to have a code editor (we recommend <a href="http://sublimetext.com/2">Sublime Text 2</a>) and some working knowledge of HTML and CSS. We won\'t walk through the source files here, but they are available for download. We\'ll focus on getting started with the compiled Bootstrap files.'
                    }

                    Bootstrap.RowFluid {
                        Bootstrap.Span6 {
                            HTML.H2 { text: 'Download compiled' }
                            HTML.P { text: '<strong>Fastest way to get started:</strong> get the compiled and minified versions of our CSS, JS, and images. No docs or original source files.' }
                            HTML.P {
                                Bootstrap.ABtnPrimary {
                                    _class: 'btn-large'
                                    href: 'http://twitter.github.io/bootstrap/assets/bootstrap.zip'
                                    text: 'Download Bootstrap'
                                }
                            }
                        }
                        Bootstrap.Span6 {
                            HTML.H2 { text: 'Download source' }
                            HTML.P { text: 'Get the original files for all CSS and JavaScript, along with a local copy of the docs by downloading the latest version directly from GitHub.' }
                            HTML.P {
                                Bootstrap.ABtn {
                                    _class: 'btn-large'
                                    href: 'https://github.com/twitter/bootstrap/zipball/master'
                                    text: 'Download Bootstrap source'
                                }
                            }
                        }
                    }
                }

                HTML.Section {
                    _id: 'file-structure'
                    Bootstrap.PageHeader {
                        HTML.H1 { text: '2. File structure' }
                    }
                    Bootstrap.Lead {
                        text: 'Within the download you\'ll find the following file structure and contents, logically grouping common assets and providing both compiled and minified variations.'
                    }
                    HTML.P {
                        text: 'Once downloaded, unzip the compressed folder to see the structure of (the compiled) Bootstrap. You\'ll see something like this:'
                    }
                    PrePrettyPrint {
                        text: '  bootstrap/
  ├── css/
  │   ├── bootstrap.css
  │   ├── bootstrap.min.css
  ├── js/
  │   ├── bootstrap.js
  │   ├── bootstrap.min.js
  └── img/
      ├── glyphicons-halflings.png
      └── glyphicons-halflings-white.png
'
                    }
                    HTML.P { text: 'This is the most basic form of Bootstrap: compiled files for quick drop-in usage in nearly any web project. We provide compiled CSS and JS (<code>bootstrap.*</code>), as well as compiled and minified CSS and JS (<code>bootstrap.min.*</code>). The image files are compressed using <a href="http://imageoptim.com/">ImageOptim</a>, a Mac app for compressing PNGs.' }
                    HTML.P { text: 'Please note that all JavaScript plugins require jQuery to be included.' }
                }

                HTML.Section {
                    _id: 'contents'
                    Bootstrap.PageHeader {
                        HTML.H1 { text: '3. What\'s included' }
                    }
                    Bootstrap.Lead {
                        text: 'Bootstrap comes equipped with HTML, CSS, and JS for all sorts of things, but they can be summarized with a handful of categories visible at the top of the <a href="http://getbootstrap.com">Bootstrap documentation</a>.'
                    }

                    HTML.H2 { text: 'Docs sections' }
                    HTML.H4 { HTML.A { href: '/scaffolding.qml'; text: 'Scaffolding' } }
                    HTML.P { text: 'Global styles for the body to reset type and background, link styles, grid system, and two simple layouts.' }
                    HTML.H4 { HTML.A { href: '/base-css.qml'; text: 'Base CSS' } }
                    HTML.P { text: 'Styles for common HTML elements like typography, code, tables, forms, and buttons. Also includes <a href="http://glyphicons.com">Glyphicons</a>, a great little icon set.' }
                    HTML.H4 { HTML.A { href: '/components.qml'; text: 'Components' } }
                    HTML.P { text: 'Basic styles for common interface components like tabs and pills, navbar, alerts, page headers, and more.' }
                    HTML.H4 { HTML.A { href: '/javascript.qml'; text: 'JavaScript plugins' } }
                    HTML.P { text: 'Similar to Components, these JavaScript plugins are interactive components for things like tooltips, popovers, modals, and more.' }

                    HTML.H2 { text: 'List of components' }
                    HTML.P { text: 'Together, the <strong>Components</strong> and <strong>JavaScript plugins</strong> sections provide the following interface elements:' }
                    HTML.Ul {
                        Utils.Repeater {
                            model: [
                                'Button groups'
                                , 'Button dropdowns'
                                , 'Navigational tabs, pills, and lists'
                                , 'Navbar'
                                , 'Labels'
                                , 'Badges'
                                , 'Page headers and hero unit'
                                , 'Thumbnails'
                                , 'Alerts'
                                , 'Progress bars'
                                , 'Modals'
                                , 'Dropdowns'
                                , 'Tooltips'
                                , 'Popovers'
                                , 'Accordion'
                                , 'Carousel'
                                , 'Typeahead'
                            ]

                            QML.Component {
                                HTML.Li { text: model.modelData }
                            }
                        }
                    }
                    HTML.P { text: 'In future guides, we may walk through these components individually in more detail. Until then, look for each of these in the documentation for information on how to utilize and customize them.' }
                }

                HTML.Section {
                    _id: 'html-template'
                    Bootstrap.PageHeader {
                        HTML.H1 { text: '4. Basic HTML template' }
                    }
                    Bootstrap.Lead { text: 'With a brief intro into the contents out of the way, we can focus on putting Bootstrap to use. To do that, we\'ll utilize a basic HTML template that includes everything we mentioned in the <a href="./getting-started.qml#file-structure">File structure</a>.' }
                    HTML.P { text: 'Now, here\'s a look at a <strong>typical HTML file</strong>:' }
                    PrePrettyPrintLineNums {
                        text: '&lt;!DOCTYPE html&gt;
&lt;html&gt;
  &lt;head&gt;
    &lt;title&gt;Bootstrap 101 Template&lt;/title&gt;
    &lt;meta name="viewport" content="width=device-width, initial-scale=1.0"&gt;
  &lt;/head&gt;
  &lt;body&gt;
    &lt;h1&gt;Hello, world!&lt;/h1&gt;
    &lt;script src="http://code.jquery.com/jquery.js"&gt;&lt;/script&gt;
  &lt;/body&gt;
&lt;/html&gt;
'
                    }
                    HTML.P { text: 'To make this <strong>a Bootstrapped template</strong>, just include the appropriate CSS and JS files:' }
                    PrePrettyPrintLineNums {
                        text: '&lt;!DOCTYPE html&gt;
&lt;html&gt;
  &lt;head&gt;
    &lt;title&gt;Bootstrap 101 Template&lt;/title&gt;
    &lt;meta name="viewport" content="width=device-width, initial-scale=1.0"&gt;
    &lt;!-- Bootstrap --&gt;
    &lt;link href="css/bootstrap.min.css" rel="stylesheet" media="screen"&gt;
  &lt;/head&gt;
  &lt;body&gt;
    &lt;h1&gt;Hello, world!&lt;/h1&gt;
    &lt;script src="http://code.jquery.com/jquery.js"&gt;&lt;/script&gt;
    &lt;script src="js/bootstrap.min.js"&gt;&lt;/script&gt;
  &lt;/body&gt;
&lt;/html&gt;
'
                    }
                    HTML.P { text: '<strong>And you\'re set!</strong> With those two files added, you can begin to develop any site or application with Bootstrap.' }
                }

                HTML.Section {
                    _id: 'examples'
                    Bootstrap.PageHeader {
                        HTML.H1 { text: '5. Examples' }
                    }
                    Bootstrap.Lead {
                        text: 'Move beyond the base template with a few example layouts. We encourage folks to iterate on these examples and not simply use them as an end result.'
                    }
                    Bootstrap.Thumbnails {
                        _class: 'bootstrap-examples'
                        Utils.Repeater {
                            model: Models.ListModel {
                                Models.ListElement {
                                    href: '/examples/starter-template.qml'
                                    src: 'http://twitter.github.io/bootstrap/assets/img/examples/bootstrap-example-starter.png'
                                    title: 'Starter template'
                                    description: 'A barebones HTML document with all the Bootstrap CSS and JavaScript included.'
                                }
                                Models.ListElement {
                                    href: '/examples/hero.qml'
                                    src: 'http://twitter.github.io/bootstrap/assets/img/examples/bootstrap-example-marketing.png'
                                    title: 'Basic marketing site'
                                    description: 'Featuring a hero unit for a primary message and three supporting elements.'
                                }
                                Models.ListElement {
                                    href: '/examples/fluid.qml'
                                    src: 'http://twitter.github.io/bootstrap/assets/img/examples/bootstrap-example-fluid.png'
                                    title: 'Fluid layout'
                                    description: 'Uses our new responsive, fluid grid system to create a seamless liquid layout.'
                                }
                                Models.ListElement {
                                    href: '/examples/marketing-narrow.qml'
                                    src: 'http://twitter.github.io/bootstrap/assets/img/examples/bootstrap-example-marketing-narrow.png'
                                    title: 'Narrow marketing'
                                    description: 'Slim, lightweight marketing template for small projects or teams.'
                                }
                                Models.ListElement {
                                    href: '/examples/justified-nav.qml'
                                    src: 'http://twitter.github.io/bootstrap/assets/img/examples/bootstrap-example-justified-nav.png'
                                    title: 'Justified nav'
                                    description: 'Marketing page with equal-width navigation links in a modified navbar.'
                                }
                                Models.ListElement {
                                    href: '/examples/signin.qml'
                                    src: 'http://twitter.github.io/bootstrap/assets/img/examples/bootstrap-example-signin.png'
                                    title: 'Sign in'
                                    description: 'Barebones sign in form with custom, larger form controls and a flexible layout.'
                                }
                                Models.ListElement {
                                    href: '/examples/sticky-footer.qml'
                                    src: 'http://twitter.github.io/bootstrap/assets/img/examples/bootstrap-example-sticky-footer.png'
                                    title: 'Sticky footer'
                                    description: 'Pin a fixed-height footer to the bottom of the user\'s viewport.'
                                }
                                Models.ListElement {
                                    href: '/examples/carousel.qml'
                                    src: 'http://twitter.github.io/bootstrap/assets/img/examples/bootstrap-example-carousel.png'
                                    title: 'Carousel jumbotron'
                                    description: 'A more interactive riff on the basic marketing site featuring a prominent carousel.'
                                }
                            }

                            QML.Component {
                                HTML.Li {
                                    _class: 'span3'
                                    HTML.A {
                                        _class: 'thumbnail'
                                        href: model.href
                                        HTML.Img {
                                            src: model.src
                                        }
                                    }
                                    HTML.H4 { text: model.title }
                                    HTML.P { text: model.description }
                                }
                            }
                        }
                    }
                }

                HTML.Section {
                    _id: 'what-next'
                    Bootstrap.PageHeader {
                        HTML.H1 { text: 'What next?' }
                    }
                    Bootstrap.Lead {
                        text: 'Head to the docs for information, examples, and code snippets, or take the next leap and customize Bootstrap for any upcoming project.'
                    }
                    Bootstrap.ABtnPrimary {
                        _class: 'btn-large'
                        href: '/scaffolding.qml'
                        text: 'Visit the Bootstrap docs'
                    }
                    HTML.Text { text: ' ' }
                    Bootstrap.ABtn {
                        _class: 'btn-large'
                        style: 'margin-left: 5px;'
                        href: '/customize.qml'
                        text: 'Customize Bootstrap'
                    }
                }
            }
        }
    }
}
