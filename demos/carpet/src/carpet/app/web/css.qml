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

Css {
    Rule {
        selector: '*'
        property string margin: '0px'
    }

    Rule {
        selector: '.layout,.widget'
//        property string border: '1px solid black'
        property string position: 'absolute'
        property string overflow: 'hidden'
    }
    Rule {
        selector: 'html'
    }
    Rule {
        selector: 'body'
        property string width: '100%'
        property string height: '100%'
        background_color: '#eee'
    }

    Rule {
        selector: 'a'
        text_decoration: 'none'
    }

    Rule {
        selector: '.widget'
        property string padding: '0px'
        background_color: '#eee'
    }

    Rule {
        selector: '.layout'
        property string padding: '0px'
        property string width: '100%'
        property string height: '100%'
//        background_color: '#ffffff'
    }

    Rule {
        selector: 'form'
        property string width: '100%'
        property string height: '100%'
        property string position: 'absolute'
        property string overflow: 'hidden'
    }

    Rule {
        selector: '.vsplitter_handler'
        cursor: 'row-resize'
//        property string z_index: '10'
        Rule {
            selector: ':hover'
            background_color: '#999999'
        }
    }

    Rule {
        selector: '.hsplitter_handler'
        cursor: 'col-resize'
//        property string z_index: '10'
        Rule {
            selector: ':hover'
            background_color: '#999999'
        }
    }

    Rule {
        selector: '.menubar'
        property string overflow: 'visible'
        property string border_bottom: '#999 1px solid'
        Rule {
            selector: '> .menuitem'
            property string _float: 'left'
            property string list_style: 'none'
            property string height: '100%'
            cursor: 'pointer'
        }
    }

    Rule {
        selector: '.menu'
//        background_color: '#eee'
        property string position: 'absolute'
        property string z_index: '9999'
        display: 'none'
    }

    Rule {
        selector: '.menuitem'
        background_color: '#eee'
        property string padding_left: '5px'
        property string padding_right: '20px'
        property string line_height: '25px'
        Rule {
            selector: 'a'
            color: '#333333'
            property string display: 'block'
        }
    }

    Rule {
        selector: '#directories'
        property string padding: '0px'

        Rule {
            selector: 'li'
            display: 'inline'
            property string line_height: '30px'
            property string list_style: 'none'

            Rule {
                selector: 'a'
                color: '#3333cc'
            }
        }
    }

    Rule {
        selector: '#jobs'
        property string padding: '0px'

        Rule {
            selector: 'li'
            display: 'inline'
            property string line_height: '30px'
            property string list_style: 'none'
        }
    }
    Rule {
        selector: '.tableview'
        Rule {
            selector: 'table'
            property string position: 'auto'
//            property string overflow: 'hiddren'
//            property string width: '100%'
//            property string height: '100%'
            Rule {
                selector: 'td'
                white_space: 'nowrap'
            }
            Rule {
                selector: 'a'
                color: '#3333cc'
            }
        }
    }

    Rule {
        selector: '.frame'
//        border: '#333399 solid 1px'
//        background_color: '#333399'
    }

    Rule {
        selector: '.view'
        background_color: '#ffffff'
        property string overflow: 'auto'

        Rule {
            selector: 'li'
//            cursor: 'pointer'
            white_space: 'nowrap'
            property string list_style: 'none'

            Rule {
                selector: '.selected'
                background_color: '#3333cc'
                Rule {
                    selector: 'a'
                    color: '#ffffff'
                }
            }

            Rule {
                selector: 'a'
                color: '#3333cc'
                Rule {
                    selector: '.removed'
                    opacity: '0.5'
                }
            }

        }
    }

    Rule {
        selector: '.listview'
        background_color: '#ffffff'
        property string overflow: 'auto'

        Rule {
            selector: 'li'

            Rule {
                selector: '.selected'
                background_color: '#3333cc'

                Rule {
                    selector: ':hover'
                    background_color: '#3333ff'
                }
            }

            Rule {
                selector: ':hover'
                background_color: '#eeeeff'
            }
        }
    }

    Rule {
        selector: '.textedit'
        background_color: '#ffffff'
        property string overflow: 'auto'
    }

    Rule {
        selector: 'img'
        property string vertical_align: 'middle'
        property string padding: '2px'
    }

    Rule {
        selector: '#loading'
        display: 'none'
        background_color: '#000000'
        opacity: '0.0'
        property string z_index: '100'
    }

    Rule {
        selector: '#add-exclude-pattern'
        cursor: 'pointer'
    }

    Rule {
        selector: '.remove-exclude-pattern'
        cursor: 'pointer'
    }

    Rule {
        selector: '#add-exclude-pattern-dialog'
        display: 'none'
    }

    Rule {
        selector: '#remove-exclude-pattern-dialog'
        display: 'none'
    }

    Rule {
        selector: '#restore-confirm-dialog'
        display: 'none'
    }
}
