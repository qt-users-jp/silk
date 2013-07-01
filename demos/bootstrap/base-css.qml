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
        HTML.Title { text: 'Base · Bootstrap' }
    }

    JumboTron {
        title: 'Base CSS'
        description: 'Fundamental HTML elements styled and enhanced with extensible classes.'
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
                                href: '#typography'
                                text: ' Typography'
                            }
                            Models.ListElement {
                                href: '#code'
                                text: ' Code'
                            }
                            Models.ListElement {
                                href: '#tables'
                                text: ' Tables'
                            }
                            Models.ListElement {
                                href: '#forms'
                                text: ' Forms'
                            }
                            Models.ListElement {
                                href: '#buttons'
                                text: ' Buttons'
                            }
                            Models.ListElement {
                                href: '#images'
                                text: ' Images'
                            }
                            Models.ListElement {
                                href: '#icons'
                                text: ' Icons by Glyphicons'
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
                    _id: 'typography'
                    Bootstrap.PageHeader {
                        HTML.H1 { text: 'Typography' }
                    }

                    HTML.H2 { _id: 'headings'; text: 'Headings' }
                    HTML.P { text: 'All HTML headings, <code>&lt;h1&gt;</code> through <code>&lt;h6&gt;</code> are available.' }
                    HTML.Div {
                        _class: 'bs-docs-example'
                        Utils.Repeater {
                            model: 6

                            QML.Component {
                                HTML.H1 {
                                    tagName: 'H%1'.arg(model.index + 1)
                                    text: 'h%1. Heading %1'.arg(model.index + 1)
                                }
                            }
                        }
                    }

                    HTML.H2 { _id: 'body-copy'; text: 'Body copy' }
                    HTML.P { text: 'Bootstrap\'s global default <code>font-size</code> is <strong>14px</strong>, with a <code>line-height</code> of <strong>20px</strong>. This is applied to the <code>&lt;body&gt;</code> and all paragraphs. In addition, <code>&lt;p&gt;</code> (paragraphs) receive a bottom margin of half their line-height (10px by default).' }
                    BsDocsExample {
                        HTML.P { text: 'Nullam quis risus eget urna mollis ornare vel eu leo. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Nullam id dolor id nibh ultricies vehicula.' }
                        HTML.P { text: 'Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec ullamcorper nulla non metus auctor fringilla. Duis mollis, est non commodo luctus, nisi erat porttitor ligula, eget lacinia odio sem nec elit. Donec ullamcorper nulla non metus auctor fringilla.' }
                        HTML.P { text: 'Maecenas sed diam eget risus varius blandit sit amet non magna. Donec id elit non mi porta gravida at eget metus. Duis mollis, est non commodo luctus, nisi erat porttitor ligula, eget lacinia odio sem nec elit.' }
                    }
                    PrePrettyPrint {
                        HTML.P { text: '...'; property bool escapeHTML: true }
                    }

                    HTML.H3 { text: 'Lead body copy' }
                    HTML.P { text: 'Make a paragraph stand out by adding <code>.lead</code>.' }
                    BsDocsExample {
                        Bootstrap.Lead { text: 'Vivamus sagittis lacus vel augue laoreet rutrum faucibus dolor auctor. Duis mollis, est non commodo luctus.' }
                    }
                    PrePrettyPrint {
                        Bootstrap.Lead { text: '...'; property bool escapeHTML: true }
                    }

                    HTML.H3 { text: 'Built with Less' }
                    HTML.P { text: 'The typographic scale is based on two LESS variables in <strong>variables.less</strong>: <code>@baseFontSize</code> and <code>@baseLineHeight</code>. The first is the base font-size used throughout and the second is the base line-height. We use those variables and some simple math to create the margins, paddings, and line-heights of all our type and more. Customize them and Bootstrap adapts.' }

                    BsDocsSeparator {}

                    HTML.H2 { _id: 'emphasis'; text: 'Emphasis' }
                    HTML.P { text: 'Make use of HTML\'s default emphasis tags with lightweight styles.' }

                    HTML.H3 { text: '<code>&lt;small&gt;</code>' }
                    HTML.P { text: 'For de-emphasizing inline or blocks of text, <small>use the small tag.</small>' }
                    BsDocsExample {
                        HTML.P { HTML.Small { text: 'This line of text is meant to be treated as fine print.' } }
                    }
                    PrePrettyPrint {
                        HTML.P {
                            property bool escapeHTML: true
                            HTML.Text { text: '\n    ' }
                            HTML.Small { text: 'This line of text is meant to be treated as fine print.' }
                            HTML.Text { text: '\n' }
                        }
                    }

                    HTML.H3 { text: 'Bold' }
                    HTML.P { text: 'For emphasizing a snippet of text with a heavier font-weight.' }
                    BsDocsExample {
                        HTML.P {
                            HTML.Text { text: 'The following snippet of text is ' }
                            HTML.Strong { text: 'rendered as bold text' }
                            HTML.Text { text: '.' }
                        }
                    }
                    PrePrettyPrint {
                        HTML.Strong { text: 'rendered as bold text'; property bool escapeHTML: true }
                    }

                    HTML.H3 { text: 'Italics' }
                    HTML.P { text: 'For emphasizing a snippet of text with italics.' }
                    BsDocsExample {
                        HTML.P {
                            HTML.Text { text: 'The following snippet of text is ' }
                            HTML.Em { text: 'rendered as italicized text' }
                            HTML.Text { text: '.' }
                        }
                    }
                    PrePrettyPrint {
                        HTML.Em { text: 'rendered as italicized text'; property bool escapeHTML: true }
                    }
                    HTML.P {
                        Bootstrap.LabelInfo { text: 'Heads Up!' }
                        HTML.Text { text: ' Feel free to use <code>&lt;b&gt;</code> and <code>&lt;i&gt;</code> in HTML5. <code>&lt;b&gt;</code> is meant to highlight words or phrases without conveying additional importance while <code>&lt;i&gt;</code> is mostly for voice, technical terms, etc.' }
                    }

                    HTML.H3 { text: 'Alignment classes' }
                    HTML.P { text: 'Easily realign text to components with text alignment classes.' }
                    BsDocsExample {
                        HTML.P { _class: 'text-left'; text: 'Left aligned text.' }
                        HTML.P { _class: 'text-center'; text: 'Center aligned text.' }
                        HTML.P { _class: 'text-right'; text: 'Right aligned text.' }
                    }
                    PrePrettyPrintLineNums {
                        HTML.P { _class: 'text-left'; text: 'Left aligned text.'; property bool escapeHTML: true }
                        HTML.Text { text: '\n' }
                        HTML.P { _class: 'text-center'; text: 'Center aligned text.'; property bool escapeHTML: true }
                        HTML.Text { text: '\n' }
                        HTML.P { _class: 'text-right'; text: 'Right aligned text.'; property bool escapeHTML: true }
                    }

                    HTML.H3 { text: 'Emphasis classes' }
                    HTML.P { text: 'Convey meaning through color with a handful of emphasis utility classes.' }
                    BsDocsExample {
                        HTML.P { _class: 'muted'; text: 'Fusce dapibus, tellus ac cursus commodo, tortor mauris nibh.' }
                        HTML.P { _class: 'text-warning'; text: 'Etiam porta sem malesuada magna mollis euismod.' }
                        HTML.P { _class: 'text-error'; text: 'Donec ullamcorper nulla non metus auctor fringilla.' }
                        HTML.P { _class: 'text-info'; text: 'Aenean eu leo quam. Pellentesque ornare sem lacinia quam venenatis.' }
                        HTML.P { _class: 'text-success'; text: 'Duis mollis, est non commodo luctus, nisi erat porttitor ligula.' }
                    }
                    PrePrettyPrintLineNums {
                        HTML.P { _class: 'muted'; text: 'Fusce dapibus, tellus ac cursus commodo, tortor mauris nibh.'; property bool escapeHTML: true }
                        HTML.Text { text: '\n' }
                        HTML.P { _class: 'text-warning'; text: 'Etiam porta sem malesuada magna mollis euismod.'; property bool escapeHTML: true }
                        HTML.Text { text: '\n' }
                        HTML.P { _class: 'text-error'; text: 'Donec ullamcorper nulla non metus auctor fringilla.'; property bool escapeHTML: true }
                        HTML.Text { text: '\n' }
                        HTML.P { _class: 'text-info'; text: 'Aenean eu leo quam. Pellentesque ornare sem lacinia quam venenatis.'; property bool escapeHTML: true }
                        HTML.Text { text: '\n' }
                        HTML.P { _class: 'text-success'; text: 'Duis mollis, est non commodo luctus, nisi erat porttitor ligula.'; property bool escapeHTML: true }
                    }

                    BsDocsSeparator {}

                    HTML.H2 { _id: 'abbreviations'; text: 'Abbreviations' }
                    HTML.P { text: 'Stylized implementation of HTML\'s <code>&lt;abbr&gt;</code> element for abbreviations and acronyms to show the expanded version on hover. Abbreviations with a <code>title</code> attribute have a light dotted bottom border and a help cursor on hover, providing additional context on hover' }

                    HTML.H3 { text: '<code>&lt;abbr&gt;</code>' }
                    HTML.P { text: 'For expanded text on long hover of an abbreviation, include the <code>title</code> attribute.' }
                    BsDocsExample {
                        HTML.P {
                            HTML.Text { text: 'An abbreviation of the word attribute is ' }
                            HTML.Abbr { title: 'attribute'; text: 'attr' }
                            HTML.Text { text: '.' }
                        }
                    }
                    PrePrettyPrint {
                        HTML.Abbr { title: 'attribute'; text: 'attr'; property bool escapeHTML: true }
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

                }
            }
        }
    }
}
