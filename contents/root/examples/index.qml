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
import Silk.HTML 5.0
import '../'

SilkPageTemplate {
    id: http

    subtitle: 'Examples'

    Article {
        Header {
            H2 { text: "silk qml examples" }
        }

        Section {
            H3 { text: "Basic HTML" }
            Ul {
                Li {
                    A { href: "%1.qml".arg(text); text: "class" }
                    Text { text: " / " }
                    A { href: "http://git.qtquick.me/?p=silk.git;a=blob;f=contents/root/examples/%1".arg(text); text: "class.qml" }
                    Ul {
                        Li {
                            A { href: "http://git.qtquick.me/?p=silk.git;a=blob;f=contents/root/examples/components/%1".arg(text); text: "Rectangle.qml" }
                        }
                    }
                }
                Li {
                    A { href: "%1.qml".arg(text); text: "repeater" }
                    Text { text: " / " }
                    A { href: "http://git.qtquick.me/?p=silk.git;a=blob;f=contents/root/examples/%1".arg(text); text: "repeater.qml" }
                }
                Li {
                    A { href: "%1.qml".arg(text); text: "json" }
                    Text { text: " / " }
                    A { href: "http://git.qtquick.me/?p=silk.git;a=blob;f=contents/root/examples/%1".arg(text); text: "json.qml" }
                    Ul {
                        Li {
                            A { href: "http://git.qtquick.me/?p=silk.git;a=blob;f=contents/root/examples/%1".arg(text); text: "json.js" }
                        }
                        Li {
                            A { href: "http://git.qtquick.me/?p=silk.git;a=blob;f=contents/root/examples/%1".arg(text); text: "jsondata.qml" }
                        }
                    }
                }
            }
        }

        Section {
            H3 { text: "Basic CSS" }
            Ul {
                Li {
                    A { href: "%1.qml".arg(text); text: "stylesheet1" }
                    Text { text: " / " }
                    A { href: "http://git.qtquick.me/?p=silk.git;a=blob;f=contents/root/examples/%1".arg(text); text: "stylesheet1.qml" }
                }
                Li {
                    A { href: "%1.qml".arg(text); text: "stylesheet2" }
                    Text { text: " / " }
                    A { href: "http://git.qtquick.me/?p=silk.git;a=blob;f=contents/root/examples/%1".arg(text); text: "stylesheet2.qml" }
                    Ul {
                        Li {
                            A { href: "http://git.qtquick.me/?p=silk.git;a=blob;f=contents/root/examples/%1".arg(text); text: "css.qml" }
                        }
                    }
                }
            }
        }

        Section {
            H3 { text: "Basic CGI" }
            Ul {
                Li {
                    A { href: "%1.qml".arg(text); text: "header" }
                    Text { text: " / " }
                    A { href: "http://git.qtquick.me/?p=silk.git;a=blob;f=contents/root/examples/%1".arg(text); text: "header.qml" }
                }
                Li {
                    A { href: "%1.qml".arg(text); text: "get" }
                    Text { text: " / " }
                    A { href: "http://git.qtquick.me/?p=silk.git;a=blob;f=contents/root/examples/%1".arg(text); text: "get.qml" }
                }
                Li {
                    A { href: "%1.qml".arg(text); text: "post" }
                    Text { text: " / " }
                    A { href: "http://git.qtquick.me/?p=silk.git;a=blob;f=contents/root/examples/%1".arg(text); text: "post.qml" }
                }
                Li {
                    A { href: "%1.qml".arg(text); text: "enabled" }
                    Text { text: " / " }
                    A { href: "http://git.qtquick.me/?p=silk.git;a=blob;f=contents/root/examples/%1".arg(text); text: "enabled.qml" }
                }
                Li {
                    A { href: "%1.qml".arg(text); text: "cookies" }
                    Text { text: " / " }
                    A { href: "http://git.qtquick.me/?p=silk.git;a=blob;f=contents/root/examples/%1".arg(text); text: "cookies.qml" }
                    Ul {
                        Li {
                            A { href: "http://git.qtquick.me/?p=silk.git;a=blob;f=contents/root/examples/components/%1".arg(text); text: "UserInput.qml" }
                        }
                    }
                }
                Li {
                    A { href: "%1.qml".arg(text); text: "session" }
                    Text { text: " / " }
                    A { href: "http://git.qtquick.me/?p=silk.git;a=blob;f=contents/root/examples/%1".arg(text); text: "session.qml" }
                }
                Li {
                    A { href: "%1.qml".arg(text); text: "upload" }
                    Text { text: " / " }
                    A { href: "http://git.qtquick.me/?p=silk.git;a=blob;f=contents/root/examples/%1".arg(text); text: "upload.qml" }
                }
            }
        }

        Section {
            H3 { text: "Basic Database" }
            Ul {
                Li {
                    A { href: "%1.qml".arg(text); text: "database" }
                    Text { text: " / " }
                    A { href: "http://git.qtquick.me/?p=silk.git;a=blob;f=contents/root/examples/%1".arg(text); text: "database.qml" }
                }
            }
        }

        Section {
            H3 { text: "Communication" }
            Ul {
                Li {
                    A { href: "%1.qml".arg(text); text: "websocket" }
                    Text { text: " / " }
                    A { href: "http://git.qtquick.me/?p=silk.git;a=blob;f=contents/root/examples/%1".arg(text); text: "websocket.qml" }
                    Ul {
                        Li {
                            A { href: "http://git.qtquick.me/?p=silk.git;a=blob;f=contents/root/examples/%1".arg(text); text: "websocket.js" }
                        }
                        Li {
                            A { href: "http://git.qtquick.me/?p=silk.git;a=blob;f=contents/root/examples/%1".arg(text); text: "echo.qml" }
                        }
                    }
                }
            }
            Ul {
                Li {
                    A { href: "%1.qml".arg(text); text: "smtp" }
                    Text { text: " / " }
                    A { href: "http://git.qtquick.me/?p=silk.git;a=blob;f=contents/root/examples/%1".arg(text); text: "smtp.qml" }
                }
            }
            Ul {
                Li {
                    A { href: "%1.qml".arg(text); text: "oauth" }
                    Text { text: " / " }
                    A { href: "http://git.qtquick.me/?p=silk.git;a=blob;f=contents/root/examples/%1".arg(text); text: "oauth.qml" }
                    Ul {
                        Li {
                            A { href: "http://git.qtquick.me/?p=silk.git;a=blob;f=contents/root/examples/components/%1".arg(text); text: "Twitter.qml" }
                        }
                    }
                }
            }
        }

        Section {
            H3 { text: "Advanced" }
            Ul {
                Li {
                    A { href: "%1.qml".arg(text); text: "timer" }
                    Text { text: " / " }
                    A { href: "http://git.qtquick.me/?p=silk.git;a=blob;f=contents/root/examples/%1".arg(text); text: "timer.qml" }
                }
                Li {
                    A { href: "%1.qml".arg(text); text: "file" }
                    Text { text: " / " }
                    A { href: "http://git.qtquick.me/?p=silk.git;a=blob;f=contents/root/examples/%1".arg(text); text: "file.qml" }
                }
                Li {
                    A { href: "%1.qml".arg(text); text: "process" }
                    Text { text: " / " }
                    A { href: "http://git.qtquick.me/?p=silk.git;a=blob;f=contents/root/examples/%1".arg(text); text: "process.qml" }
                }
                Li {
                    A { href: "%1.qml".arg(text); text: "config" }
                    Text { text: " / " }
                    A { href: "http://git.qtquick.me/?p=silk.git;a=blob;f=contents/root/examples/%1".arg(text); text: "config.qml" }
                }
                Li {
                    A { href: "%1.qml".arg(text); text: "remote" }
                    Text { text: " / " }
                    A { href: "http://git.qtquick.me/?p=silk.git;a=blob;f=contents/root/examples/%1".arg(text); text: "remote.qml" }
                }
                Li {
                    A { href: "%1.qml".arg(text); text: "javascript" }
                    Text { text: " / " }
                    A { href: "http://git.qtquick.me/?p=silk.git;a=blob;f=contents/root/examples/%1".arg(text); text: "javascript.qml" }
                    Ul {
                        Li {
                            A { href: "http://git.qtquick.me/?p=silk.git;a=blob;f=contents/root/examples/%1".arg(text); text: "reverse.js" }
                        }
                    }
                }
                Li {
                    A { href: "%1.qml".arg(text); text: "chat" }
                    Text { text: " / " }
                    A { href: "http://git.qtquick.me/?p=silk.git;a=blob;f=contents/root/examples/%1".arg(text); text: "chat.qml" }
                    Ul {
                        Li {
                            A { href: "http://git.qtquick.me/?p=silk.git;a=blob;f=contents/root/examples/%1".arg(text); text: "chat.js" }
                        }
                        Li {
                            A { href: "http://git.qtquick.me/?p=silk.git;a=blob;f=contents/root/examples/%1".arg(text); text: "chatserver.qml" }
                        }
                        Li {
                            A { href: "http://git.qtquick.me/?p=silk.git;a=blob;f=contents/tasks/%1".arg(text); text: "chatdaemon.qml" }
                        }
                    }
                }
                Li {
                    A { href: "%1.qml".arg(text); text: "iphone" }
                    Text { text: " / " }
                    A { href: "http://git.qtquick.me/?p=silk.git;a=blob;f=contents/root/examples/%1".arg(text); text: "iphone.qml" }
                    Ul {
                        Li {
                            A { href: "http://git.qtquick.me/?p=silk.git;a=blob;f=contents/root/examples/%1".arg(text); text: "IPhoneCSS.qml" }
                        }
                    }
                }
            }
        }
    }
}
