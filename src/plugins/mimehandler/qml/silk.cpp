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

#include "silk.h"

#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QUuid>
#include <QtCore/QRegularExpression>

#include <QtQml/QQmlEngine>
#include <QtQml/QQmlComponent>
#include <QtQml/QQmlContext>

static QHash<QString, unsigned int> colorNameMap;

class Silk::Color
{
public:
    Color();
    Color(const Color &other);
    Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 0xFF);
    Color(const QString &str);

    Color &operator=(const Color &other);

    QString toString() const;
    bool isValid() const;

    Color lighter(qreal factor) const;
    Color darker(qreal factor) const;

private:
    Color changeValue(qreal factor) const;

private:
    bool ok;
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
};

Silk::Color::Color()
    : ok(false)
    , r(0)
    , g(0)
    , b(0)
    , a(0)
{
}

Silk::Color::Color(const Color &other)
    : ok(other.ok)
    , r(other.r)
    , g(other.g)
    , b(other.b)
    , a(other.a)
{
}

Silk::Color::Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
    : ok(true)
    , r(r)
    , g(g)
    , b(b)
    , a(a)
{
}

Silk::Color::Color(const QString &str)
    : ok(false)
    , r(0)
    , g(0)
    , b(0)
    , a(0xFF)
{
    if (colorNameMap.isEmpty()) {
        colorNameMap.insert(QStringLiteral("aliceblue"), ((0xFF << 24) | (240 << 16) | (248 << 8) | 255));
        colorNameMap.insert(QStringLiteral("antiquewhite"), ((0xFF << 24) | (250 << 16) | (235 << 8) | 215));
        colorNameMap.insert(QStringLiteral("aqua"), ((0xFF << 24) | (0 << 16) | (255 << 8) | 255));
        colorNameMap.insert(QStringLiteral("aquamarine"), ((0xFF << 24) | (127 << 16) | (255 << 8) | 212));
        colorNameMap.insert(QStringLiteral("azure"), ((0xFF << 24) | (240 << 16) | (255 << 8) | 255));
        colorNameMap.insert(QStringLiteral("beige"), ((0xFF << 24) | (245 << 16) | (245 << 8) | 220));
        colorNameMap.insert(QStringLiteral("bisque"), ((0xFF << 24) | (255 << 16) | (228 << 8) | 196));
        colorNameMap.insert(QStringLiteral("black"), ((0xFF << 24) | (0 << 16) | (0 << 8) | 0));
        colorNameMap.insert(QStringLiteral("blanchedalmond"), ((0xFF << 24) | (255 << 16) | (235 << 8) | 205));
        colorNameMap.insert(QStringLiteral("blue"), ((0xFF << 24) | (0 << 16) | (0 << 8) | 255));
        colorNameMap.insert(QStringLiteral("blueviolet"), ((0xFF << 24) | (138 << 16) | (43 << 8) | 226));
        colorNameMap.insert(QStringLiteral("brown"), ((0xFF << 24) | (165 << 16) | (42 << 8) | 42));
        colorNameMap.insert(QStringLiteral("burlywood"), ((0xFF << 24) | (222 << 16) | (184 << 8) | 135));
        colorNameMap.insert(QStringLiteral("cadetblue"), ((0xFF << 24) | (95 << 16) | (158 << 8) | 160));
        colorNameMap.insert(QStringLiteral("chartreuse"), ((0xFF << 24) | (127 << 16) | (255 << 8) | 0));
        colorNameMap.insert(QStringLiteral("chocolate"), ((0xFF << 24) | (210 << 16) | (105 << 8) | 30));
        colorNameMap.insert(QStringLiteral("coral"), ((0xFF << 24) | (255 << 16) | (127 << 8) | 80));
        colorNameMap.insert(QStringLiteral("cornflowerblue"), ((0xFF << 24) | (100 << 16) | (149 << 8) | 237));
        colorNameMap.insert(QStringLiteral("cornsilk"), ((0xFF << 24) | (255 << 16) | (248 << 8) | 220));
        colorNameMap.insert(QStringLiteral("crimson"), ((0xFF << 24) | (220 << 16) | (20 << 8) | 60));
        colorNameMap.insert(QStringLiteral("cyan"), ((0xFF << 24) | (0 << 16) | (255 << 8) | 255));
        colorNameMap.insert(QStringLiteral("darkblue"), ((0xFF << 24) | (0 << 16) | (0 << 8) | 139));
        colorNameMap.insert(QStringLiteral("darkcyan"), ((0xFF << 24) | (0 << 16) | (139 << 8) | 139));
        colorNameMap.insert(QStringLiteral("darkgoldenrod"), ((0xFF << 24) | (184 << 16) | (134 << 8) | 11));
        colorNameMap.insert(QStringLiteral("darkgray"), ((0xFF << 24) | (169 << 16) | (169 << 8) | 169));
        colorNameMap.insert(QStringLiteral("darkgreen"), ((0xFF << 24) | (0 << 16) | (100 << 8) | 0));
        colorNameMap.insert(QStringLiteral("darkgrey"), ((0xFF << 24) | (169 << 16) | (169 << 8) | 169));
        colorNameMap.insert(QStringLiteral("darkkhaki"), ((0xFF << 24) | (189 << 16) | (183 << 8) | 107));
        colorNameMap.insert(QStringLiteral("darkmagenta"), ((0xFF << 24) | (139 << 16) | (0 << 8) | 139));
        colorNameMap.insert(QStringLiteral("darkolivegreen"), ((0xFF << 24) | (85 << 16) | (107 << 8) | 47));
        colorNameMap.insert(QStringLiteral("darkorange"), ((0xFF << 24) | (255 << 16) | (140 << 8) | 0));
        colorNameMap.insert(QStringLiteral("darkorchid"), ((0xFF << 24) | (153 << 16) | (50 << 8) | 204));
        colorNameMap.insert(QStringLiteral("darkred"), ((0xFF << 24) | (139 << 16) | (0 << 8) | 0));
        colorNameMap.insert(QStringLiteral("darksalmon"), ((0xFF << 24) | (233 << 16) | (150 << 8) | 122));
        colorNameMap.insert(QStringLiteral("darkseagreen"), ((0xFF << 24) | (143 << 16) | (188 << 8) | 143));
        colorNameMap.insert(QStringLiteral("darkslateblue"), ((0xFF << 24) | (72 << 16) | (61 << 8) | 139));
        colorNameMap.insert(QStringLiteral("darkslategray"), ((0xFF << 24) | (47 << 16) | (79 << 8) | 79));
        colorNameMap.insert(QStringLiteral("darkslategrey"), ((0xFF << 24) | (47 << 16) | (79 << 8) | 79));
        colorNameMap.insert(QStringLiteral("darkturquoise"), ((0xFF << 24) | (0 << 16) | (206 << 8) | 209));
        colorNameMap.insert(QStringLiteral("darkviolet"), ((0xFF << 24) | (148 << 16) | (0 << 8) | 211));
        colorNameMap.insert(QStringLiteral("deeppink"), ((0xFF << 24) | (255 << 16) | (20 << 8) | 147));
        colorNameMap.insert(QStringLiteral("deepskyblue"), ((0xFF << 24) | (0 << 16) | (191 << 8) | 255));
        colorNameMap.insert(QStringLiteral("dimgray"), ((0xFF << 24) | (105 << 16) | (105 << 8) | 105));
        colorNameMap.insert(QStringLiteral("dimgrey"), ((0xFF << 24) | (105 << 16) | (105 << 8) | 105));
        colorNameMap.insert(QStringLiteral("dodgerblue"), ((0xFF << 24) | (30 << 16) | (144 << 8) | 255));
        colorNameMap.insert(QStringLiteral("firebrick"), ((0xFF << 24) | (178 << 16) | (34 << 8) | 34));
        colorNameMap.insert(QStringLiteral("floralwhite"), ((0xFF << 24) | (255 << 16) | (250 << 8) | 240));
        colorNameMap.insert(QStringLiteral("forestgreen"), ((0xFF << 24) | (34 << 16) | (139 << 8) | 34));
        colorNameMap.insert(QStringLiteral("fuchsia"), ((0xFF << 24) | (255 << 16) | (0 << 8) | 255));
        colorNameMap.insert(QStringLiteral("gainsboro"), ((0xFF << 24) | (220 << 16) | (220 << 8) | 220));
        colorNameMap.insert(QStringLiteral("ghostwhite"), ((0xFF << 24) | (248 << 16) | (248 << 8) | 255));
        colorNameMap.insert(QStringLiteral("gold"), ((0xFF << 24) | (255 << 16) | (215 << 8) | 0));
        colorNameMap.insert(QStringLiteral("goldenrod"), ((0xFF << 24) | (218 << 16) | (165 << 8) | 32));
        colorNameMap.insert(QStringLiteral("gray"), ((0xFF << 24) | (128 << 16) | (128 << 8) | 128));
        colorNameMap.insert(QStringLiteral("green"), ((0xFF << 24) | (0 << 16) | (128 << 8) | 0));
        colorNameMap.insert(QStringLiteral("greenyellow"), ((0xFF << 24) | (173 << 16) | (255 << 8) | 47));
        colorNameMap.insert(QStringLiteral("grey"), ((0xFF << 24) | (128 << 16) | (128 << 8) | 128));
        colorNameMap.insert(QStringLiteral("honeydew"), ((0xFF << 24) | (240 << 16) | (255 << 8) | 240));
        colorNameMap.insert(QStringLiteral("hotpink"), ((0xFF << 24) | (255 << 16) | (105 << 8) | 180));
        colorNameMap.insert(QStringLiteral("indianred"), ((0xFF << 24) | (205 << 16) | (92 << 8) | 92));
        colorNameMap.insert(QStringLiteral("indigo"), ((0xFF << 24) | (75 << 16) | (0 << 8) | 130));
        colorNameMap.insert(QStringLiteral("ivory"), ((0xFF << 24) | (255 << 16) | (255 << 8) | 240));
        colorNameMap.insert(QStringLiteral("khaki"), ((0xFF << 24) | (240 << 16) | (230 << 8) | 140));
        colorNameMap.insert(QStringLiteral("lavender"), ((0xFF << 24) | (230 << 16) | (230 << 8) | 250));
        colorNameMap.insert(QStringLiteral("lavenderblush"), ((0xFF << 24) | (255 << 16) | (240 << 8) | 245));
        colorNameMap.insert(QStringLiteral("lawngreen"), ((0xFF << 24) | (124 << 16) | (252 << 8) | 0));
        colorNameMap.insert(QStringLiteral("lemonchiffon"), ((0xFF << 24) | (255 << 16) | (250 << 8) | 205));
        colorNameMap.insert(QStringLiteral("lightblue"), ((0xFF << 24) | (173 << 16) | (216 << 8) | 230));
        colorNameMap.insert(QStringLiteral("lightcoral"), ((0xFF << 24) | (240 << 16) | (128 << 8) | 128));
        colorNameMap.insert(QStringLiteral("lightcyan"), ((0xFF << 24) | (224 << 16) | (255 << 8) | 255));
        colorNameMap.insert(QStringLiteral("lightgoldenrodyellow"), ((0xFF << 24) | (250 << 16) | (250 << 8) | 210));
        colorNameMap.insert(QStringLiteral("lightgray"), ((0xFF << 24) | (211 << 16) | (211 << 8) | 211));
        colorNameMap.insert(QStringLiteral("lightgreen"), ((0xFF << 24) | (144 << 16) | (238 << 8) | 144));
        colorNameMap.insert(QStringLiteral("lightgrey"), ((0xFF << 24) | (211 << 16) | (211 << 8) | 211));
        colorNameMap.insert(QStringLiteral("lightpink"), ((0xFF << 24) | (255 << 16) | (182 << 8) | 193));
        colorNameMap.insert(QStringLiteral("lightsalmon"), ((0xFF << 24) | (255 << 16) | (160 << 8) | 122));
        colorNameMap.insert(QStringLiteral("lightseagreen"), ((0xFF << 24) | (32 << 16) | (178 << 8) | 170));
        colorNameMap.insert(QStringLiteral("lightskyblue"), ((0xFF << 24) | (135 << 16) | (206 << 8) | 250));
        colorNameMap.insert(QStringLiteral("lightslategray"), ((0xFF << 24) | (119 << 16) | (136 << 8) | 153));
        colorNameMap.insert(QStringLiteral("lightslategrey"), ((0xFF << 24) | (119 << 16) | (136 << 8) | 153));
        colorNameMap.insert(QStringLiteral("lightsteelblue"), ((0xFF << 24) | (176 << 16) | (196 << 8) | 222));
        colorNameMap.insert(QStringLiteral("lightyellow"), ((0xFF << 24) | (255 << 16) | (255 << 8) | 224));
        colorNameMap.insert(QStringLiteral("lime"), ((0xFF << 24) | (0 << 16) | (255 << 8) | 0));
        colorNameMap.insert(QStringLiteral("limegreen"), ((0xFF << 24) | (50 << 16) | (205 << 8) | 50));
        colorNameMap.insert(QStringLiteral("linen"), ((0xFF << 24) | (250 << 16) | (240 << 8) | 230));
        colorNameMap.insert(QStringLiteral("magenta"), ((0xFF << 24) | (255 << 16) | (0 << 8) | 255));
        colorNameMap.insert(QStringLiteral("maroon"), ((0xFF << 24) | (128 << 16) | (0 << 8) | 0));
        colorNameMap.insert(QStringLiteral("mediumaquamarine"), ((0xFF << 24) | (102 << 16) | (205 << 8) | 170));
        colorNameMap.insert(QStringLiteral("mediumblue"), ((0xFF << 24) | (0 << 16) | (0 << 8) | 205));
        colorNameMap.insert(QStringLiteral("mediumorchid"), ((0xFF << 24) | (186 << 16) | (85 << 8) | 211));
        colorNameMap.insert(QStringLiteral("mediumpurple"), ((0xFF << 24) | (147 << 16) | (112 << 8) | 219));
        colorNameMap.insert(QStringLiteral("mediumseagreen"), ((0xFF << 24) | (60 << 16) | (179 << 8) | 113));
        colorNameMap.insert(QStringLiteral("mediumslateblue"), ((0xFF << 24) | (123 << 16) | (104 << 8) | 238));
        colorNameMap.insert(QStringLiteral("mediumspringgreen"), ((0xFF << 24) | (0 << 16) | (250 << 8) | 154));
        colorNameMap.insert(QStringLiteral("mediumturquoise"), ((0xFF << 24) | (72 << 16) | (209 << 8) | 204));
        colorNameMap.insert(QStringLiteral("mediumvioletred"), ((0xFF << 24) | (199 << 16) | (21 << 8) | 133));
        colorNameMap.insert(QStringLiteral("midnightblue"), ((0xFF << 24) | (25 << 16) | (25 << 8) | 112));
        colorNameMap.insert(QStringLiteral("mintcream"), ((0xFF << 24) | (245 << 16) | (255 << 8) | 250));
        colorNameMap.insert(QStringLiteral("mistyrose"), ((0xFF << 24) | (255 << 16) | (228 << 8) | 225));
        colorNameMap.insert(QStringLiteral("moccasin"), ((0xFF << 24) | (255 << 16) | (228 << 8) | 181));
        colorNameMap.insert(QStringLiteral("navajowhite"), ((0xFF << 24) | (255 << 16) | (222 << 8) | 173));
        colorNameMap.insert(QStringLiteral("navy"), ((0xFF << 24) | (0 << 16) | (0 << 8) | 128));
        colorNameMap.insert(QStringLiteral("oldlace"), ((0xFF << 24) | (253 << 16) | (245 << 8) | 230));
        colorNameMap.insert(QStringLiteral("olive"), ((0xFF << 24) | (128 << 16) | (128 << 8) | 0));
        colorNameMap.insert(QStringLiteral("olivedrab"), ((0xFF << 24) | (107 << 16) | (142 << 8) | 35));
        colorNameMap.insert(QStringLiteral("orange"), ((0xFF << 24) | (255 << 16) | (165 << 8) | 0));
        colorNameMap.insert(QStringLiteral("orangered"), ((0xFF << 24) | (255 << 16) | (69 << 8) | 0));
        colorNameMap.insert(QStringLiteral("orchid"), ((0xFF << 24) | (218 << 16) | (112 << 8) | 214));
        colorNameMap.insert(QStringLiteral("palegoldenrod"), ((0xFF << 24) | (238 << 16) | (232 << 8) | 170));
        colorNameMap.insert(QStringLiteral("palegreen"), ((0xFF << 24) | (152 << 16) | (251 << 8) | 152));
        colorNameMap.insert(QStringLiteral("paleturquoise"), ((0xFF << 24) | (175 << 16) | (238 << 8) | 238));
        colorNameMap.insert(QStringLiteral("palevioletred"), ((0xFF << 24) | (219 << 16) | (112 << 8) | 147));
        colorNameMap.insert(QStringLiteral("papayawhip"), ((0xFF << 24) | (255 << 16) | (239 << 8) | 213));
        colorNameMap.insert(QStringLiteral("peachpuff"), ((0xFF << 24) | (255 << 16) | (218 << 8) | 185));
        colorNameMap.insert(QStringLiteral("peru"), ((0xFF << 24) | (205 << 16) | (133 << 8) | 63));
        colorNameMap.insert(QStringLiteral("pink"), ((0xFF << 24) | (255 << 16) | (192 << 8) | 203));
        colorNameMap.insert(QStringLiteral("plum"), ((0xFF << 24) | (221 << 16) | (160 << 8) | 221));
        colorNameMap.insert(QStringLiteral("powderblue"), ((0xFF << 24) | (176 << 16) | (224 << 8) | 230));
        colorNameMap.insert(QStringLiteral("purple"), ((0xFF << 24) | (128 << 16) | (0 << 8) | 128));
        colorNameMap.insert(QStringLiteral("red"), ((0xFF << 24) | (255 << 16) | (0 << 8) | 0));
        colorNameMap.insert(QStringLiteral("rosybrown"), ((0xFF << 24) | (188 << 16) | (143 << 8) | 143));
        colorNameMap.insert(QStringLiteral("royalblue"), ((0xFF << 24) | (65 << 16) | (105 << 8) | 225));
        colorNameMap.insert(QStringLiteral("saddlebrown"), ((0xFF << 24) | (139 << 16) | (69 << 8) | 19));
        colorNameMap.insert(QStringLiteral("salmon"), ((0xFF << 24) | (250 << 16) | (128 << 8) | 114));
        colorNameMap.insert(QStringLiteral("sandybrown"), ((0xFF << 24) | (244 << 16) | (164 << 8) | 96));
        colorNameMap.insert(QStringLiteral("seagreen"), ((0xFF << 24) | (46 << 16) | (139 << 8) | 87));
        colorNameMap.insert(QStringLiteral("seashell"), ((0xFF << 24) | (255 << 16) | (245 << 8) | 238));
        colorNameMap.insert(QStringLiteral("sienna"), ((0xFF << 24) | (160 << 16) | (82 << 8) | 45));
        colorNameMap.insert(QStringLiteral("silver"), ((0xFF << 24) | (192 << 16) | (192 << 8) | 192));
        colorNameMap.insert(QStringLiteral("skyblue"), ((0xFF << 24) | (135 << 16) | (206 << 8) | 235));
        colorNameMap.insert(QStringLiteral("slateblue"), ((0xFF << 24) | (106 << 16) | (90 << 8) | 205));
        colorNameMap.insert(QStringLiteral("slategray"), ((0xFF << 24) | (112 << 16) | (128 << 8) | 144));
        colorNameMap.insert(QStringLiteral("slategrey"), ((0xFF << 24) | (112 << 16) | (128 << 8) | 144));
        colorNameMap.insert(QStringLiteral("snow"), ((0xFF << 24) | (255 << 16) | (250 << 8) | 250));
        colorNameMap.insert(QStringLiteral("springgreen"), ((0xFF << 24) | (0 << 16) | (255 << 8) | 127));
        colorNameMap.insert(QStringLiteral("steelblue"), ((0xFF << 24) | (70 << 16) | (130 << 8) | 180));
        colorNameMap.insert(QStringLiteral("tan"), ((0xFF << 24) | (210 << 16) | (180 << 8) | 140));
        colorNameMap.insert(QStringLiteral("teal"), ((0xFF << 24) | (0 << 16) | (128 << 8) | 128));
        colorNameMap.insert(QStringLiteral("thistle"), ((0xFF << 24) | (216 << 16) | (191 << 8) | 216));
        colorNameMap.insert(QStringLiteral("tomato"), ((0xFF << 24) | (255 << 16) | (99 << 8) | 71));
        colorNameMap.insert(QStringLiteral("turquoise"), ((0xFF << 24) | (64 << 16) | (224 << 8) | 208));
        colorNameMap.insert(QStringLiteral("violet"), ((0xFF << 24) | (238 << 16) | (130 << 8) | 238));
        colorNameMap.insert(QStringLiteral("wheat"), ((0xFF << 24) | (245 << 16) | (222 << 8) | 179));
        colorNameMap.insert(QStringLiteral("white"), ((0xFF << 24) | (255 << 16) | (255 << 8) | 255));
        colorNameMap.insert(QStringLiteral("whitesmoke"), ((0xFF << 24) | (245 << 16) | (245 << 8) | 245));
        colorNameMap.insert(QStringLiteral("yellow"), ((0xFF << 24) | (255 << 16) | (255 << 8) | 0));
        colorNameMap.insert(QStringLiteral("yellowgreen"), ((0xFF << 24) | (154 << 16) | (205 << 8) | 50));
    }

    if (colorNameMap.contains(str)) {
        unsigned int color = colorNameMap.value(str);
        r = ((color >> 16) & 0xFF);
        g = ((color >>  8) & 0xFF);
        b = ((color >>  0) & 0xFF);
        a = ((color >> 24) & 0xFF);
        ok = true;
        return;
    }

    QRegularExpression hex(QStringLiteral("^#([0-9A-Fa-f]{2})([0-9A-Fa-f]{2})([0-9A-Fa-f]{2})$"));
    QRegularExpressionMatch match = hex.match(str);
    if (match.hasMatch()) {
        r = match.captured(1).toInt(0, 16) & 0xFF;
        g = match.captured(2).toInt(0, 16) & 0xFF;
        b = match.captured(3).toInt(0, 16) & 0xFF;
        ok = true;
        return;
    }

    QRegularExpression rgba(QStringLiteral("^rgba\\(([0-9]+),\\s*([0-9]+),\\s*([0-9]+),\\s*([0-9\\.]+)\\s*\\)$"));
    match = rgba.match(str);
    if (match.hasMatch()) {
        r = match.captured(1).toInt();
        g = match.captured(2).toInt();
        b = match.captured(3).toInt();
        a = match.captured(4).toFloat() * 0xFF;
        ok = true;
        return;
    }
}

Silk::Color &Silk::Color::operator=(const Color &other)
{
    ok = other.ok;
    r = other.r;
    g = other.g;
    b = other.b;
    a = other.a;
    return *this;
}

Silk::Color Silk::Color::changeValue(qreal factor) const
{
    int h = 0;
    unsigned char max = qMax<unsigned char>(r, qMax(g, b));
    unsigned char min = qMin<unsigned char>(r, qMin(g, b));
    if (r == g && g == b) {
        h = 0;
    } else if (max == r) {
        h = 60 * (g - b) / (max - min);
    } if (max == g) {
        h = 60 * (b - r) / (max - min) + 120;
    } else {
        h = 60 * (r - g) / (max - min) + 240;
    }
    if (h < 0)
        h += 360;
    int s = 255 * (max - min) / max;
    int v = max;

    v *= factor;
    if (v > 255) {
        s -= v - 255;
        if (s < 0)
            s = 0;
        v = 255;
    }

    max = v;
    min = max - (max * s / 255);

    Color ret;
    if (h < 60) {
        ret = Color(max, (h / 60) * (max - min) + min, min);
    } else if (h < 120) {
        ret = Color(((120 - h) / 60) * (max - min) + min, max, min);
    } else if (h < 180) {
        ret = Color(min, max, ((120 - h) / 60) * (max - min) + min);
    } else if (h < 240) {
        ret = Color(min, ((240 - h) / 60) * (max - min) + min, max);
    } else if (h < 300) {
        ret = Color(((h - 240) / 60) * (max - min) + min, min, max);
    } else {
        ret = Color(max, min, ((360 - h) / 60) * (max - min) + min);
    }
    return ret;
}

Silk::Color Silk::Color::lighter(qreal factor) const
{
    return changeValue(factor);
}

Silk::Color Silk::Color::darker(qreal factor) const
{
    return changeValue(1.0 / factor);
}

QString Silk::Color::toString() const
{
    return QStringLiteral("rgba(%1, %2, %3, %4)").arg(r).arg(g).arg(b).arg(a / 255.0);
}

bool Silk::Color::isValid() const
{
    return ok;
}

Silk::Silk(QObject *parent)
    : QObject(parent)
{
}

QString Silk::uuid()
{
    QString ret = QUuid::createUuid().toString().mid(1);
    ret.chop(1);
    return ret;
}

QString Silk::readFile(const QString &filePath) const
{
    QString ret;
    QFile file(filePath);
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        ret = QString::fromUtf8(file.readAll());
        file.close();
    }
    return ret;
}

QVariantList Silk::readDir(const QString &path) const
{
    QVariantList ret;
    QDir dir(path);
    foreach (const QString &file, dir.entryList(QDir::Files)) {
        ret.append(file);
    }
    return ret;
}

bool Silk::isAvailable(const QString &uri, int major, int minor, const QString &element) const
{
    bool ret = false;
    QQmlEngine *engine = qobject_cast<QQmlEngine*>(parent());
    QQmlContext *context = new QQmlContext(engine->rootContext());
    QQmlComponent component(engine);
    QByteArray data = QString("import %1 %2.%3\n%4 {}").arg(uri).arg(major).arg(minor).arg(element).toUtf8();
    component.setData(data, QUrl());
    QObject *object = component.create(context);
    object->deleteLater();
    ret = !component.isError();

    return ret;
}

QString Silk::escapeHTML(const QString &source) const
{
    QString ret = source;
    return ret
            .replace(QLatin1Char('&'), QStringLiteral("&amp;"))
            .replace(QLatin1Char('<'), QStringLiteral("&lt;"))
            .replace(QLatin1Char('>'), QStringLiteral("&gt;"))
            .replace(QLatin1Char('"'), QStringLiteral("&quot;"))
            .replace(QLatin1Char('\''), QStringLiteral("&#39;"))
            ;
}

QString Silk::lighter(const QString &str, qreal factor) const
{
    Color color(str);
    if (color.isValid()) {
        return color.lighter(factor).toString();
    }
    return QString();
}

QString Silk::darker(const QString &str, qreal factor) const
{
    Color color(str);
    if (color.isValid()) {
        return color.darker(factor).toString();
    }
    return QString();
}
