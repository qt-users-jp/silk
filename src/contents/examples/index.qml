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

import QtQuick.HTTP 1.1
import QtQuick.HTML 4.01

Http {
    id: root
    status: 200
    responseHeader: {'Content-Type': 'text/html; charset=utf-8;'}

    Html {
        Head {
            Title { id: title; text: "Examples" }
        }

        Body {
            H1 { text: title.text }
            Ul {
                Li {
                    Text { text: "HTML Basics" }
                    Ul {
                        Li { A { href: "%1.qml".arg(text); text: "class" } }
                        Li { A { href: "%1.qml".arg(text); text: "repeater" } }
                        Li { A { href: "%1.qml".arg(text); text: "comment" } }
                        Li {
                            A { href: "%1.qml".arg(text); text: "json" }
                            Ul {
                                Li { A { href: "%1.qml".arg(text); text: "jsondata" } }
                            }
                        }
                    }
                }
                Li {
                    Text { text: "CSS Basics" }
                    Ul {
                        Li { A { href: "%1.qml".arg(text); text: "stylesheet1" } }
                        Li {
                            A { href: "%1.qml".arg(text); text: "stylesheet2" }
                            Ul {
                                Li { A { href: "%1.qml".arg(text); text: "css" } }
                            }
                        }
                    }
                }
                Li {
                    Text { text: "CGI Basics" }
                    Ul {
                        Li { A { href: "%1.qml".arg(text); text: "header" } }
                        Li { A { href: "%1.qml".arg(text); text: "get" } }
                        Li { A { href: "%1.qml".arg(text); text: "post" } }
                        Li { A { href: "%1.qml".arg(text); text: "enabled" } }
                    }
                }

                Li {
                    Text { text: 'Advanced' }
                    Ul {
                        Li { A { href: "%1.qml".arg(text); text: "timer" } }
                        Li { A { href: "%1.qml".arg(text); text: "file" } }
                        Li { A { href: "%1.qml".arg(text); text: "remote" } }
                        Li {
                            A { href: "%1.qml".arg(text); text: "javascript" }
                            Ul {
                                Li { A { href: "%1.js".arg(text); text: "reverse" } }
                            }
                        }
                        Li {
                            A { href: "%1.qml".arg(text); text: "chat" }
                            Ul {
                                Li { A { href: "%1.js".arg(text); text: "chat" } }
                            }
                        }
                        Li {
                            A { href: "%1.qml".arg(text); text: "iphone" }
                            Ul {
                                Li { A { href: "%1.qml".arg(text); text: "IPhoneCSS" } }
                            }
                        }
                    }
                }

            }
        }
    }
}
