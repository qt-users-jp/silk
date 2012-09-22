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

import Silk.CSS 2.1

Rule {
    position: "relative"
    height: "495px"
    width: "250px"
    property string border_radius: "35px"
    margin: "50px auto"

    background: "-webkit-linear-gradient(-165deg, rgba(255,255,255,0.4), rgba(255,255,255,0.15) 35%, transparent 35%),
                -webkit-linear-gradient(top, transparent 85px, #222222 85px, #151515 410px, transparent 410px),
                -webkit-linear-gradient(top, #000, #0a0a0a)"

    background_repeat: "no-repeat"
    property string background_size: "100% 100%, 220px 100%, 100% 100%"
    background_position: "0 0, 15px 0, 0 0"

    property string box_shadow: "0 0 0 3px black,
                            -40px -128px 0 -123px black,
                            0 0 0 5px #a09f9d,
                            49px -130px 3px -123px #777,
                            46px -130px 2px -123px #ddd,
                            62px -111px 0 -105px #8e8d8b,
                            62px -112px 0 -105px #b4b3b1,
                            62px -113px 0 -105px #666"

    Rule {
        selector: ":after"

        content: "'▢'"
        line_height: "46px"
        text_align: "center"
        font_size: "28px"
        color: "#666"
        position: "absolute"
        width: line_height
        height: line_height
        property string border_radius: "50%"
        background: "white"
        bottom: "18px"
        left: "100px"
        border: "2px solid #0a0a0a"

        background_image: "-webkit-linear-gradient(left, rgba(0, 0, 0, 0.85), black)"

        property string box_shadow: "-39px -410px 0 -23px #000f31,
                                    -39px -410px 0 -22px #0a1c5a,
                                    -40px -410px 0 -21px #0d1216,
                                    -40px -410px 0 -19px #1b191a,

                                    -12px -410px 0 -22px #333,
                                    -8px -410px 0 -22px #333,
                                    -4px -410px 0 -22px #333,
                                    -0px -410px 0 -22px #333,
                                    4px -410px 0 -22px #333,
                                    8px -410px 0 -22px #333,
                                    12px -410px 0 -22px #333,
                                    16px -410px 0 -22px #333,
                                    20px -410px 0 -22px #333,
                                    24px -410px 0 -22px #333,

                                    -12px -410px 0 -18px #0a0a0a,
                                    -9px -410px 0 -18px #0a0a0a,
                                    -6px -410px 0 -18px #0a0a0a,
                                    -3px -410px 0 -18px #0a0a0a,
                                    0px -410px 0 -18px #0a0a0a,
                                    3px -410px 0 -18px #0a0a0a,
                                    6px -410px 0 -18px #0a0a0a,
                                    9px -410px 0 -18px #0a0a0a,
                                    12px -410px 0 -18px #0a0a0a,
                                    15px -410px 0 -18px #0a0a0a,
                                    18px -410px 0 -18px #0a0a0a,
                                    21px -410px 0 -18px #0a0a0a,
                                    24px -410px 0 -18px #0a0a0a"
    }

    Rule {
        selector: ":before"

        position: "absolute"
        content: "''"
        width: "2px"
        height: "117px"
        background: "transparent"
        top: "40px"
        left: "-7px"

        background_image: "-webkit-linear-gradient(left, transparent 0px, transparent 1px, #7a7879 2px),
                          -webkit-linear-gradient(top, #383838 1px, #b9b9b9 3px, #b9b9b9 10px, #383838 19px, #b9b9b9 23px, transparent 23px, transparent 53px, #383838 53px, #b9b9b9 54px, #dadada 58px, #383838 62px, black 62px, black 66px, #383838 66px, #b9b9b9 67px, #dadada 68px, #383838 70px, transparent 70px, transparent 100px, #383838 100px, #b9b9b9 101px, #dadada 105px, #383838 109px, black 109px, black 113px, #383838 113px, #b9b9b9 114px, #dadada 115px, #383838 117px)"
    }
}
