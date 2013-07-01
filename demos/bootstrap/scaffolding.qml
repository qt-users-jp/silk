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
        HTML.Title { text: 'Scaffolding · Bootstrap' }
    }

    JumboTron {
        title: 'Scaffolding'
        description: 'Bootstrap is built on responsive 12-column grids, layouts, and components.'
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
                                href: '#global'
                                text: ' Global styles'
                            }
                            Models.ListElement {
                                href: '#gridSystem'
                                text: ' Grid system'
                            }
                            Models.ListElement {
                                href: '#fluidGridSystem'
                                text: ' Fluid grid system'
                            }
                            Models.ListElement {
                                href: '#layouts'
                                text: ' Layouts'
                            }
                            Models.ListElement {
                                href: '#responsive'
                                text: ' Responsive design'
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
                    _id: 'global'
                    Bootstrap.PageHeader {
                        HTML.H1 { text: 'Global settings' }
                    }
                    HTML.H3 { text: 'Requires HTML5 doctype' }
                    HTML.P { text: 'Bootstrap makes use of certain HTML elements and CSS properties that require the use of the HTML5 doctype. Include it at the beginning of all your projects.' }
                    PrePrettyPrintLineNums {
                        text: '&lt;!DOCTYPE html&gt;
&lt;html lang="en"&gt;
  ...
&lt;/html&gt;'
                    }
                    HTML.H3 { text: 'Typography and links' }
                    HTML.P { text: 'Bootstrap sets basic global display, typography, and link styles. Specifically, we:' }
                    HTML.Ul {
                        HTML.Li { text: 'Remove <code>margin</code> on the body' }
                        HTML.Li { text: 'Set <code>background-color: white;</code> on the <code>body</code>' }
                        HTML.Li { text: 'Use the <code>@baseFontFamily</code>, <code>@baseFontSize</code>, and <code>@baseLineHeight</code> attributes as our typographic base' }
                        HTML.Li { text: 'Set the global link color via <code>@linkColor</code> and apply link underlines only on <code>:hover</code>' }
                    }
                    HTML.P { text: 'These styles can be found within <strong>scaffolding.less</strong>.' }

                    HTML.H3 { text: 'Reset via Normalize' }
                    HTML.P { text: 'With Bootstrap 2, the old reset block has been dropped in favor of <a href="http://necolas.github.com/normalize.css/" target="_blank">Normalize.css</a>, a project by <a href="http://twitter.com/necolas" target="_blank">Nicolas Gallagher</a> and <a href="http://twitter.com/jon_neal" target="_blank">Jonathan Neal</a> that also powers the <a href="http://html5boilerplate.com" target="_blank">HTML5 Boilerplate</a>. While we use much of Normalize within our <strong>reset.less</strong>, we have removed some elements specifically for Bootstrap.' }
                }

                HTML.Section {
                    _id: 'gridSystem'
                    Bootstrap.PageHeader {
                        HTML.H1 { text: 'Default grid system' }
                    }
                    HTML.H2 { text: 'Live grid example' }
                    HTML.P { text: 'The default Bootstrap grid system utilizes <strong>12 columns</strong>, making for a 940px wide container without <a href="./scaffolding.html#responsive">responsive features</a> enabled. With the responsive CSS file added, the grid adapts to be 724px and 1170px wide depending on your viewport. Below 767px viewports, the columns become fluid and stack vertically.' }
                    HTML.Div {
                        _class: 'bs-docs-grid'
                        Bootstrap.Row {
                            _class: 'show-grid'
                            Utils.Repeater {
                                model: 9
                                QML.Component {
                                    Bootstrap.Span1 {
                                        text: '1'
                                    }
                                }
                            }
                        }
                        Bootstrap.Row {
                            _class: 'show-grid'
                            Utils.Repeater {
                                model: 3
                                QML.Component {
                                    HTML.Span {
                                        _class: 'span%1'.arg(text)
                                        text: model.index + 2
                                    }
                                }
                            }
                        }
                        Bootstrap.Row {
                            _class: 'show-grid'
                            Utils.Repeater {
                                model: 2
                                QML.Component {
                                    HTML.Span {
                                        _class: 'span%1'.arg(text)
                                        text: model.index + 4
                                    }
                                }
                            }
                        }
                        Bootstrap.Row {
                            _class: 'show-grid'
                            Bootstrap.Span9 {
                                text: '9'
                            }
                        }
                    }
                    HTML.H3 { text: 'Basic grid HTML' }
                    HTML.P { text: 'For a simple two column layout, create a <code>.row</code> and add the appropriate number of <code>.span*</code> columns. As this is a 12-column grid, each <code>.span*</code> spans a number of those 12 columns, and should always add up to 12 for each row (or the number of columns in the parent).' }
                    PrePrettyPrintLineNums {
                        text: '&lt;div class="row"&gt;
  &lt;div class="span4"&gt;...&lt;/div&gt;
  &lt;div class="span8"&gt;...&lt;/div&gt;
&lt;/div&gt;
'
                    }
                    HTML.P { text: 'Given this example, we have <code>.span4</code> and <code>.span8</code>, making for 12 total columns and a complete row.' }

                    HTML.H2 { text: 'Offsetting columns' }
                    HTML.P { text: 'Move columns to the right using <code>.offset*</code> classes. Each class increases the left margin of a column by a whole column. For example, <code>.offset4</code> moves <code>.span4</code> over four columns.' }
                    HTML.Div {
                        _class: 'bs-docs-grid'
                        Bootstrap.Row {
                            _class: 'show-grid'
                            Bootstrap.Span4 { text: '4' }
                            Bootstrap.Span3 { _class: 'offset2'; text: '3 offset 2' }
                        }
                        Bootstrap.Row {
                            _class: 'show-grid'
                            Bootstrap.Span3 { _class: 'offset1'; text: '3 offset 1' }
                            Bootstrap.Span3 { _class: 'offset2'; text: '3 offset 2' }
                        }
                        Bootstrap.Row {
                            _class: 'show-grid'
                            Bootstrap.Span6 { _class: 'offset3'; text: '6 offset 3' }
                        }
                    }
                    PrePrettyPrintLineNums {
                        text: '&lt;div class="row"&gt;
  &lt;div class="span4"&gt;...&lt;/div&gt;
  &lt;div class="span3 offset2"&gt;...&lt;/div&gt;
&lt;/div&gt;
'
                    }

                    HTML.H2 { text: 'Nesting columns' }
                    HTML.P { text: 'To nest your content with the default grid, add a new <code>.row</code> and set of <code>.span*</code> columns within an existing <code>.span*</code> column. Nested rows should include a set of columns that add up to the number of columns of its parent.' }
                    HTML.Div {
                        _class: 'bs-docs-grid'
                        Bootstrap.Row {
                            _class: 'show-grid'
                            Bootstrap.Span9 {
                                HTML.Text { text: 'Level 1 column' }
                                Bootstrap.Row {
                                    _class: 'show-grid'
                                    Bootstrap.Span6 { text: 'Level 2' }
                                    Bootstrap.Span3 { text: 'Level 2' }
                                }
                            }
                        }
                    }
                    PrePrettyPrintLineNums {
                        text: '&lt;div class="row"&gt;
  &lt;div class="span9"&gt;
    Level 1 column
    &lt;div class="row"&gt;
      &lt;div class="span6"&gt;Level 2&lt;/div&gt;
      &lt;div class="span3"&gt;Level 2&lt;/div&gt;
    &lt;/div&gt;
  &lt;/div&gt;
&lt;/div&gt;
'
                    }
                }

                HTML.Section {
                    _id: 'fluidGridSystem'
                    Bootstrap.PageHeader {
                        HTML.H1 { text: 'Fluid grid system' }
                    }
                    HTML.H2 { text: 'Live fluid grid example' }
                    HTML.P { text: 'The fluid grid system uses percents instead of pixels for column widths. It has the same responsive capabilities as our fixed grid system, ensuring proper proportions for key screen resolutions and devices.' }
                    HTML.Div {
                        _class: 'bs-docs-grid'
                        Bootstrap.RowFluid {
                            _class: 'show-grid'
                            Utils.Repeater {
                                model: 12
                                QML.Component {
                                    Bootstrap.Span1 {
                                        text: '1'
                                    }
                                }
                            }
                        }
                        Bootstrap.RowFluid {
                            _class: 'show-grid'
                            Utils.Repeater {
                                model: 3
                                QML.Component {
                                    Bootstrap.Span4 {
                                        text: '4'
                                    }
                                }
                            }
                        }
                        Bootstrap.RowFluid {
                            _class: 'show-grid'
                            Utils.Repeater {
                                model: 2
                                QML.Component {
                                    HTML.Span {
                                        _class: 'span%1'.arg(text)
                                        text: (model.index + 1) * 4
                                    }
                                }
                            }
                        }
                        Bootstrap.RowFluid {
                            _class: 'show-grid'
                            Utils.Repeater {
                                model: 2
                                QML.Component {
                                    Bootstrap.Span6 {
                                        text: '6'
                                    }
                                }
                            }
                        }
                        Bootstrap.RowFluid {
                            _class: 'show-grid'
                            Bootstrap.Span12 {
                                text: '12'
                            }
                        }
                    }

                    HTML.H3 { text: 'Basic fluid grid HTML' }
                    HTML.P { text: 'Make any row "fluid" by changing <code>.row</code> to <code>.row-fluid</code>. The column classes stay the exact same, making it easy to flip between fixed and fluid grids.' }
                    PrePrettyPrintLineNums {
                        text: '&lt;div class="row-fluid"&gt;
  &lt;div class="span4"&gt;...&lt;/div&gt;
  &lt;div class="span8"&gt;...&lt;/div&gt;
&lt;/div&gt;
'
                    }

                    HTML.H2 { text: 'Fluid offsetting' }
                    HTML.P { text: 'Operates the same way as the fixed grid system offsetting: add <code>.offset*</code> to any column to offset by that many columns.' }
                    HTML.Div {
                        _class: 'bs-docs-grid'
                        Bootstrap.RowFluid {
                            _class: 'show-grid'
                            Bootstrap.Span4 { text: '4' }
                            Bootstrap.Span4 { _class: 'offset4'; text: '4 offset 4' }
                        }
                        Bootstrap.RowFluid {
                            _class: 'show-grid'
                            Bootstrap.Span3 { _class: 'offset3'; text: '3 offset 3' }
                            Bootstrap.Span3 { _class: 'offset3'; text: '3 offset 3' }
                        }
                        Bootstrap.RowFluid {
                            _class: 'show-grid'
                            Bootstrap.Span6 { _class: 'offset6'; text: '6 offset 6' }
                        }
                    }
                    PrePrettyPrintLineNums {
                        text: '&lt;div class="row-fluid"&gt;
  &lt;div class="span4"&gt;...&lt;/div&gt;
  &lt;div class="span4 offset2"&gt;...&lt;/div&gt;
&lt;/div&gt;
'
                    }

                    HTML.H2 { text: 'Fluid nesting' }
                    HTML.P { text: 'Fluid grids utilize nesting differently: each nested level of columns should add up to 12 columns. This is because the fluid grid uses percentages, not pixels, for setting widths.' }
                    HTML.Div {
                        _class: 'bs-docs-grid'
                        Bootstrap.RowFluid {
                            _class: 'show-grid'
                            Bootstrap.Span12 {
                                HTML.Text { text: 'Fluid 12' }
                                Bootstrap.RowFluid {
                                    _class: 'show-grid'
                                    Bootstrap.Span6 {
                                        HTML.Text { text: 'Fluid 6' }
                                        Bootstrap.RowFluid {
                                            _class: 'show-grid'
                                            Bootstrap.Span6 {text: 'Fluid 6' }
                                            Bootstrap.Span6 {text: 'Fluid 6' }
                                        }
                                    }
                                    Bootstrap.Span6 {text: 'Fluid 6' }
                                }
                            }
                        }
                    }
                    PrePrettyPrintLineNums {
                        text: '&lt;div class="row-fluid"&gt;
  &lt;div class="span12"&gt;
    Fluid 12
    &lt;div class="row-fluid"&gt;
      &lt;div class="span6"&gt;
        Fluid 6
        &lt;div class="row-fluid"&gt;
          &lt;div class="span6"&gt;Fluid 6&lt;/div&gt;
          &lt;div class="span6"&gt;Fluid 6&lt;/div&gt;
        &lt;/div&gt;
      &lt;/div&gt;
      &lt;div class="span6"&gt;Fluid 6&lt;/div&gt;
    &lt;/div&gt;
  &lt;/div&gt;
&lt;/div&gt;
'
                    }
                }

                HTML.Section {
                    _id: 'layouts'
                    Bootstrap.PageHeader {
                        HTML.H1 { text: 'Layouts' }
                    }

                    HTML.H2 { text: 'Fixed layout' }
                    HTML.P { text: 'Provides a common fixed-width (and optionally responsive) layout with only <code>&lt;div class="container"&gt;</code> required.' }
                    HTML.Div {
                        _class: 'mini-layout'
                        HTML.Div {
                            _class: 'mini-layout-body'
                            text: ''
                        }
                    }
                    PrePrettyPrintLineNums {
                        text: '&lt;body&gt;
  &lt;div class="container"&gt;
    ...
  &lt;/div&gt;
&lt;/body&gt;
'
                    }

                    HTML.H2 { text: 'Fluid layout' }
                    HTML.P { text: 'Create a fluid, two-column page with <code>&lt;div class="container-fluid"&gt;</code>&mdash;great for applications and docs.' }
                    HTML.Div {
                        _class: 'mini-layout fluid'
                        HTML.Div {
                            _class: 'mini-layout-sidebar'
                            text: ''
                        }
                        HTML.Div {
                            _class: 'mini-layout-body'
                            text: ''
                        }
                    }
                    PrePrettyPrintLineNums {
                        text: '&lt;div class="container-fluid"&gt;
  &lt;div class="row-fluid"&gt;
    &lt;div class="span2"&gt;
      &lt;!--Sidebar content--&gt;
    &lt;/div&gt;
    &lt;div class="span10"&gt;
      &lt;!--Body content--&gt;
    &lt;/div&gt;
  &lt;/div&gt;
&lt;/div&gt;
'
                    }
                }

                HTML.Section {
                    _id: 'responsive'
                    Bootstrap.PageHeader {
                        HTML.H1 { text: 'Responsive design' }
                    }

                    HTML.H2 { text: 'Enabling responsive features' }
                    HTML.P { text: 'Turn on responsive CSS in your project by including the proper meta tag and additional stylesheet within the <code>&lt;head&gt;</code> of your document. If you\'ve compiled Bootstrap from the Customize page, you need only include the meta tag.' }
                    PrePrettyPrintLineNums {
                        text: '&lt;meta name="viewport" content="width=device-width, initial-scale=1.0"&gt;
&lt;link href="assets/css/bootstrap-responsive.css" rel="stylesheet"&gt;
'
                    }
                    HTML.P {
                        Bootstrap.LabelInfo { text: 'Heads up!' }
                        HTML.Text { text: '  Bootstrap doesn\'t include responsive features by default at this time as not everything needs to be responsive. Instead of encouraging developers to remove this feature, we figure it best to enable it as needed.' }
                    }

                    HTML.H2 { text: 'About responsive Bootstrap' }
                    HTML.Img {
                        src: 'http://twitter.github.io/bootstrap/assets/img/responsive-illustrations.png'
                        alt: 'Responsive devices'
                        style: 'float: right; margin: 0 0 20px 20px;'
                    }
                    HTML.P { text: 'Media queries allow for custom CSS based on a number of conditions&mdash;ratios, widths, display type, etc&mdash;but usually focuses around <code>min-width</code> and <code>max-width</code>.' }
                    HTML.Ul {
                        HTML.Li { text: 'Modify the width of column in our grid' }
                        HTML.Li { text: 'Stack elements instead of float wherever necessary' }
                        HTML.Li { text: 'Resize headings and text to be more appropriate for devices' }
                    }
                    HTML.P { text: 'Use media queries responsibly and only as a start to your mobile audiences. For larger projects, do consider dedicated code bases and not layers of media queries.' }

                    HTML.H2 { text: 'Supported devices' }
                    HTML.P { text: 'Bootstrap supports a handful of media queries in a single file to help make your projects more appropriate on different devices and screen resolutions. Here\'s what\'s included:' }
                    Bootstrap.Table {
                        bordered: true
                        striped: true

                        HTML.THead {
                            HTML.Tr {
                                HTML.Th { text: 'Label' }
                                HTML.Th { text: 'Layout width' }
                                HTML.Th { text: 'Column width' }
                                HTML.Th { text: 'Gutter width' }
                            }
                        }

                        HTML.TBody {
                            HTML.Tr {
                                HTML.Td { text: 'Large display' }
                                HTML.Td { text: '1200px and up' }
                                HTML.Td { text: '70px' }
                                HTML.Td { text: '30px' }
                            }
                            HTML.Tr {
                                HTML.Td { text: 'Default' }
                                HTML.Td { text: '980px and up' }
                                HTML.Td { text: '60px' }
                                HTML.Td { text: '20px' }
                            }
                            HTML.Tr {
                                HTML.Td { text: 'Portrait tablets' }
                                HTML.Td { text: '768px and above' }
                                HTML.Td { text: '42px' }
                                HTML.Td { text: '20px' }
                            }
                            HTML.Tr {
                                HTML.Td { text: 'Phones to tablets' }
                                HTML.Td { text: '767px and below' }
                                HTML.Td { _class: 'muted'; colspan: '2'; text: 'Fluid columns, no fixed widths' }
                            }
                            HTML.Tr {
                                HTML.Td { text: 'Phones' }
                                HTML.Td { text: '480px and below' }
                                HTML.Td { _class: 'muted'; colspan: '2'; text: 'Fluid columns, no fixed widths' }
                            }
                        }
                    }

                    PrePrettyPrintLineNums {
                        text: '/* Large desktop */
@media (min-width: 1200px) { ... }

/* Portrait tablet to landscape and desktop */
@media (min-width: 768px) and (max-width: 979px) { ... }

/* Landscape phone to portrait tablet */
@media (max-width: 767px) { ... }

/* Landscape phones and down */
@media (max-width: 480px) { ... }
'
                    }

                    HTML.H2 { text: 'Responsive utility classes' }
                    HTML.P { text: 'For faster mobile-friendly development, use these utility classes for showing and hiding content by device. Below is a table of the available classes and their effect on a given media query layout (labeled by device). They can be found in <code>responsive.less</code>.' }
                    Bootstrap.Table {
                        bordered: true
                        striped: true
                        _class: 'responsive-utilities'

                        HTML.THead {
                            HTML.Tr {
                                HTML.Th { text: 'Class' }
                                HTML.Th { text: 'Phones <small>767px and below</small>' }
                                HTML.Th { text: 'Tablets <small>979px to 768px</small>' }
                                HTML.Th { text: 'Desktops <small>Default</small>' }
                            }
                        }

                        HTML.TBody {
                            HTML.Tr {
                                HTML.Th { text: '<code>.visible-phone</code>' }
                                HTML.Td { _class: 'is-visible'; text: 'Visible' }
                                HTML.Td { _class: 'is-hidden'; text: 'Hidden' }
                                HTML.Td { _class: 'is-hidden'; text: 'Hidden' }
                            }
                            HTML.Tr {
                                HTML.Th { text: '<code>.visible-tablet</code>' }
                                HTML.Td { _class: 'is-hidden'; text: 'Hidden' }
                                HTML.Td { _class: 'is-visible'; text: 'Visible' }
                                HTML.Td { _class: 'is-hidden'; text: 'Hidden' }
                            }
                            HTML.Tr {
                                HTML.Th { text: '<code>.visible-desktop</code>' }
                                HTML.Td { _class: 'is-hidden'; text: 'Hidden' }
                                HTML.Td { _class: 'is-hidden'; text: 'Hidden' }
                                HTML.Td { _class: 'is-visible'; text: 'Visible' }
                            }
                            HTML.Tr {
                                HTML.Th { text: '<code>.hidden-phone</code>' }
                                HTML.Td { _class: 'is-hidden'; text: 'Hidden' }
                                HTML.Td { _class: 'is-visible'; text: 'Visible' }
                                HTML.Td { _class: 'is-visible'; text: 'Visible' }
                            }
                            HTML.Tr {
                                HTML.Th { text: '<code>.hidden-tablet</code>' }
                                HTML.Td { _class: 'is-visible'; text: 'Visible' }
                                HTML.Td { _class: 'is-hidden'; text: 'Hidden' }
                                HTML.Td { _class: 'is-visible'; text: 'Visible' }
                            }
                            HTML.Tr {
                                HTML.Th { text: '<code>.hidden-desktop</code>' }
                                HTML.Td { _class: 'is-visible'; text: 'Visible' }
                                HTML.Td { _class: 'is-visible'; text: 'Visible' }
                                HTML.Td { _class: 'is-hidden'; text: 'Hidden' }
                            }
                        }
                    }

                    HTML.H3 { text: 'When to use' }
                    HTML.P { text: 'Use on a limited basis and avoid creating entirely different versions of the same site. Instead, use them to complement each device\'s presentation. Responsive utilities should not be used with tables, and as such are not supported.' }

                    HTML.H3 { text: 'Responsive utilities test case' }
                    HTML.P { text: 'Resize your browser or load on different devices to test the above classes.' }
                    HTML.H4 { text: 'Visible on...' }
                    HTML.P { text: 'Green checkmarks indicate that class is visible in your current viewport.' }
                    HTML.Ul {
                        _class: 'responsive-utilities-test'
                        HTML.Li { text: 'Phone<span class="visible-phone">&#10004; Phone</span>' }
                        HTML.Li { text: 'Tablet<span class="visible-tablet">&#10004; Tablet</span>' }
                        HTML.Li { text: 'Desktop<span class="visible-desktop">&#10004; Desktop</span>' }
                    }
                    HTML.H4 { text: 'Hidden on...' }
                    HTML.P { text: 'Here, green checkmarks indicate that class is hidden in your current viewport.' }
                    HTML.Ul {
                        _class: 'responsive-utilities-test'
                        HTML.Li { text: 'Phone<span class="hidden-phone">&#10004; Phone</span>' }
                        HTML.Li { text: 'Tablet<span class="hidden-tablet">&#10004; Tablet</span>' }
                        HTML.Li { text: 'Desktop<span class="hidden-desktop">&#10004; Desktop</span>' }
                    }
                }
            }
        }
    }
}
