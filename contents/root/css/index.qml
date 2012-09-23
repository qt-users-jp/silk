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

import Silk.HTTP 1.1
import Silk.CSS 3.0

Http {
    id: root
    status: 200
    responseHeader: {'Content-Type': 'text/css; charset=utf-8;'}

    property string whiteColor: '#f0ffe1'
    property string lightColor: '#80C342'
    property string darkColor: '#006225'
    property string blackColor: '#003212'

    /* ==========================================================================
       HTML5 Boilerplate styles - h5bp.com (generated via initializr.com)
       ========================================================================== */

    Rule {
        selector: 'html,button,input,select,textarea'
        color: '#222'
    }

    Rule {
        selector: 'body'
        font_size: '1em'
        property string line_height: '1.4'
        background: root.whiteColor
    }

    Rule {
        selector: '::-moz-selection'
        background: root.lightColor
        property string text_shadow: 'none'
    }

    Rule {
        selector: '::selection'
        background: root.lightColor
        property string text_shadow: 'none'
    }

    Rule {
        selector: 'hr'
        display: 'block'
        property string height: '1px'
        border: '0'
        border_top: '1px solid #ccc'
        property string margin: '1em 0'
        property string padding: '0'
    }

    Rule {
        selector: 'img'
        property string vertical_align: 'middle'
    }

    Rule {
        selector: 'fieldset'
        border: '0'
        property string margin: '0'
        property string padding: '0'
    }

    Rule {
        selector: 'textarea'
        resize: 'vertical'
    }

    Rule {
        selector: '.chromeframe'
        property string margin: '0.2em 0'
        background: '#ccc'
        color: '#000'
        property string padding: '0.2em 0'
    }

    /* ===== Initializr Styles ==================================================
       Author: Jonathan Verrecchia - verekia.com/initializr/responsive-template
       ========================================================================== */

    Rule {
        selector: 'body'
        font: '16px/26px Helvetica, Helvetica Neue, Arial'
    }

    Rule {
        selector: '.wrapper'
        property string width: '90%'
        property string margin: '0 5%'
    }

    /* ===================
        ALL: Orange Theme
       =================== */

    Rule {
        id: header_container
        selector: '.header-container'
        border_bottom: '20px solid %1'.arg(root.lightColor)
        background: root.darkColor
    }

    Rule {
        selector: '.main aside'
        border_bottom: '10px solid %1'.arg(root.darkColor)
        background: root.lightColor
    }

    Rule {
        selector: '.footer-container'
        border_top: '20px solid %1'.arg(root.lightColor)
        background: root.darkColor
    }

    Rule {
        selector: '.title'
        color: root.whiteColor
        font_size: '2.25em'

        Rule {
            selector: 'a'
            color: root.whiteColor
            text_decoration: 'none'

            Rule {
                selector: ':hover,:visit'
                color: root.whiteColor
            }

            Rule {
                selector: ':hover'
                text_decoration: 'underline'
            }
        }
    }

    /* ==============
        MOBILE: Menu
       ============== */

    Rule {
        selector: '.header-container nav'
        Rule {
            selector: 'ul'
            property string margin: '0'
            property string padding: '0'
        }

        Rule {
            selector: 'a'
            display: 'block'
            property string margin_bottom: '10px'
            property string padding: '15px 0'

            text_align: 'center'
            text_decoration: 'none'
            font_weight: 'bold'

            color: root.whiteColor
            background: root.lightColor

            Rule {
                selector: ':hover,:visit'
                color: root.whiteColor
            }

            Rule {
                selector: ':hover'
                text_decoration: 'underline'
            }
        }
    }

    /* ==============
        MOBILE: Main
       ============== */

    Rule {
        selector: '.main'

        Rule {
            selector: 'article'
//            property string padding: '30px 0'
            color: root.blackColor

            Rule {
                selector: 'h1'
                font_size: '2em'
                property string line_height: '2em'
            }

            Rule {
                selector: 'h2'
                font_size: '2em'
                property string line_height: '1.5em'
            }

            Rule {
                selector: 'h3'
                font_size: '1.5em'
                property string line_height: '1.25em'
            }

            Rule {
                selector: 'a'
                color: root.darkColor

                Rule {
                    selector: ':hover,:visit'
                    color: root.darkColor
                }
            }
        }

        Rule {
            selector: 'aside'
            color: root.whiteColor
            property string padding: '0px 5% 10px'
        }
    }

    Rule {
        selector: '.footer-container'
        Rule {
            selector: 'footer'
            property string padding: '20px 0'
            Rule {
                selector: 'a'
                color: root.whiteColor
                text_decoration: 'none'

                Rule {
                    selector: ':hover,:visit'
                    color: root.whiteColor
                }

                Rule {
                    selector: ':hover'
                    text_decoration: 'underline'
                }
            }
        }
    }

    /* ===============
        ALL: IE Fixes
       =============== */

    Rule {
        selector: '.ie7'
        Rule {
            selector: '.title'
            property string padding_top: '20px'
        }
    }

    /* ==========================================================================
       Author's custom styles
       ========================================================================== */

    PlainFile {}
    Button {}
    Terminal {}
    ArchitectureBlock {}

    Rule {
        selector: '.main'
        Rule {
            selector: 'article'
            Rule {
                selector: 'footer'
                text_align: 'right'
                border_top: '1px solid %1'.arg(root.darkColor)
            }
        }
    }

    /* ==========================================================================
       Media Queries
       ========================================================================== */

    Rule {
        selector: '@media only screen and (min-width: 480px)'

        /* ====================
            INTERMEDIATE: Menu
           ==================== */

        Rule {
            selector: '.header-container nav'

            Rule {
                selector: 'a'
                property string _float: 'left'
                property string width: '27%'
                property string margin: '10px 1.7%'
                property string padding: '25px 2%'
                property string margin_bottom: '0'
            }

            Rule {
                selector: 'li:first-child a'
                property string margin_left: '0'
            }

            Rule {
                selector: 'li:last-child a'
                property string margin_right: '0'
            }
        }

        /* ========================
            INTERMEDIATE: IE Fixes
           ======================== */

        Rule {
            selector: '.header-container nav ul li'
            display: 'inline'
        }

        Rule {
            selector: '.oldie .header-container nav a'
            property string margin: '0 0.7%'
        }
    }

    Rule {
        selector: '@media only screen and (min-width: 768px)'

        /* ====================
            WIDE: CSS3 Effects
           ==================== */

        Rule {
            selector: '.header-container, .main aside'
            property string _webkit_box_shadow: '0 5px 10px #aaa'
            property string _moz_box_shadow: _webkit_box_shadow
            property string box_shadow: _webkit_box_shadow
        }

        /* ============
            WIDE: Menu
           ============ */

        Rule {
            selector: '.title'
            property string _float: 'left'
            property string padding_top: '10px'
        }

        Rule {
            selector: '.header-container nav'
            property string _float: 'right'
            property string width: '38%'
        }

        /* ============
            WIDE: Main
           ============ */

        Rule {
            selector: '.main'
            Rule {
                selector: 'article'
                property string _float: 'left'
                property string width: '57%'
            }
            Rule {
                selector: 'aside'
                property string _float: 'right'
                property string width: '28%'
            }
        }
    }

    Rule {
        selector: '@media only screen and (min-width: 1140px)'

        /* ===============
            Maximal Width
           =============== */

        Rule {
            selector: '.wrapper'
            property string width: '1026px'  /* 1140px - 10% for margins */
            property string margin: '0 auto'
        }
    }

    /* ==========================================================================
       Helper classes
       ========================================================================== */

    Rule {
        selector: '.ir'
        background_color: 'transparent'
        border: '0'
        property string overflow: 'hidden'
        text_indent: '-9999%' /* *text-indent?? */

        Rule {
            selector: ':before'
            content: '""'
            display: 'block'
            property string width: '0'
            property string height: '100%'
        }
    }

    Rule {
        selector: '.hidden'
        display: 'none !important'
        property string visibility: 'hidden'
    }

    Rule {
        selector: '.visuallyhidden'
        border: '0'
        property string clip: 'rect(0 0 0 0)'
        property string height: '1px'
        property string margin: '-1px'
        property string overflow: 'hidden'
        property string padding: '0'
        property string position: 'absolute'
        property string width: '1px'

        Rule {
            selector: '.focusible'
            Rule {
                selector: ':active,:focus'
                property string clip: 'auto'
                property string height: 'auto'
                property string margin: '0'
                property string overflow: 'visible'
                property string position: 'static'
                property string width: 'auto'
            }
        }
    }

    Rule {
        selector: '.invisible'
        property string visibility: 'hidden'
    }

    Rule {
        selector: '.clearfix'
        property string zoom: '1'

        Rule {
            selector: ':before'
            content: '" "'
            display: 'table'
        }

        Rule {
            selector: ':after'
            content: '" "'
            display: 'table'
            property string clear: 'both'
        }
    }

    /* ==========================================================================
       Print styles
       ========================================================================== */

    Rule {
        selector: '@media print'

        Rule {
            selector: '*'
            background: 'transparent !important'
            color: '#000 !important'
            property string box_shadow: 'none !important'
            property string text_shadow: 'none !important'
        }

        Rule {
            selector: 'a'
            color: 'blue'
            text_decoration: 'underline'

            Rule {
                selector: ':visited'
                text_decoration: 'underline'
            }

            Rule {
                selector: '[href]:after'
                content: '" (" attr(href) ")"'
            }
        }

        Rule {
            selector: 'abbr[title]:after'
            content: '" (" attr(title) ")"'
        }

        /*
         * Don't show links for images, or javascript/internal links
         */

        Rule {
            selector: '.ir a:after, a[href^="javascript:"]:after, a[href^="#"]:after, a[href^="/"]:after'
            content: '""'
        }

        Rule {
            selector: 'pre,blockquote'
            border: '1px solid #999'
            property string page_break_inside: "avoid"
        }

        Rule {
            selector: 'thead'
            display: 'table-header-group'
        }

        Rule {
            selector: 'tr,img'
            property string page_break_inside: "avoid"
        }

        Rule {
            selector: 'img'
            property string max_width: '100% !important'
        }

        Rule {
            selector: '@page'
            property string margin: '0.5cm'
        }

        Rule {
            selector: 'p,h2,h3'
            orphans: '3'
            widows: '3'
        }

        Rule {
            selector: 'h2,h3'
            property string page_break_after: "avoid"
        }

        Rule {
            selector: '.header-container'
            border_bottom: '2px solid #333'
        }

        Rule {
            selector: '.main'
            property string padding: '0'
        }

        Rule {
            selector: 'aside,.header-container nav'
            display: 'none'
        }

        Rule {
            selector: '.footer-container'
            border_top: '2px solid #333'
            Rule {
                selector: 'footer'
                property string padding: '0'
            }
        }
    }
}
