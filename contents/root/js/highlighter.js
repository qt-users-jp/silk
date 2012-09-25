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

function highlight() {
    var files = document.getElementsByClassName("file");
    for (var i = 0; i < files.length; i++) {
        var classes = files[i].className.split(/ /);
        if (classes.indexOf('json') > -1) {
            files[i].innerHTML = highlightJson(files[i].innerHTML);
        } else if (classes.indexOf('qml') > -1) {
            files[i].innerHTML = highlightQml(files[i].innerHTML);
        }
    }
}

function addClass(str, name)
{
    return '<span class="' + name + '">' + str + '</span>';
}

function qmlToString2(object, indent)
{
    var ret = '';
    if (typeof indent === 'undefined') indent = 0;

    var action = object.shift();
    var args = object.shift();
    switch (action) {
    case 'import':
        for (var i = 0; i < args.length; i++) {
            if (i > 0) ret += ' ';
            ret += addClass(args[i][1], args[i][0]);
        }
        ret += '\n';
        break;
    case 'element': {
        ret += spaces(indent);
        ret += addClass(args.shift(), 'element') + ' {';
        args.shift(); // {
        var contents = args.shift();
        if (contents.length > 0) {
            ret += '\n';
            for (var i = 0; i < contents.length; i++) {
                ret += qmlToString2(contents[i], indent + 1);
            }
            ret += spaces(indent) + '}\n';
        } else {
            ret += '}\n';
        }

        break; }
    case 'property': {
        ret += spaces(indent);
        ret += addClass(args.shift(), 'property');
        ret += args.shift() + ' ';
        ret += qmlToString2(args.shift(), indent + 1);
        ret += '\n';
        break; }
    case 'reference': {
        ret += addClass(args, 'reference');
        break; }
    case 'string': {
        ret += addClass(args, 'string');
        break; }
    case 'number': {
        ret += addClass(args, 'number');
        break; }
    case 'comment': {
        ret += spaces(indent);
        ret += addClass(args.shift(), 'comment');
        ret += '\n';
        break; }
    case 'blah3': {
        ret += spaces(indent);
        ret += addClass(args.shift(), 'blah3');
        ret += '\n';
        break; }
    case 'json': {
        ret += '{\n';
        for (var i in args) {
            ret += spaces(indent + 1);
            ret += addClass(i, 'property');
            ret += ': ';
            ret += qmlToString2(args[i], indent + 1);
            ret += '\n';
        }

        ret += spaces(indent);
        ret += '}\n';
        break; }


    default:
        console.debug(action, args);
        break;
    }

    return ret;
}

function qmlToString(object, indent)
{
    var ret = '';
    if (typeof indent === 'undefined') indent = 0;
    var type = typeof object;
    if (type === 'object') {
        if (typeof object.isTuple === 'boolean') {
            type = 'tuple';
        } else if (typeof object.pop === 'function') {
            type = 'array';
        }
    }

    switch (type) {
    case 'tuple': {
        var imports = false;
        for (var i in object) {
            if (i === 'isTuple' || i === 'ruleName') continue;
            if (object[i][0] === 'import') {
                imports = true;
            } else {
                if (imports) {
                    imports = false;
                    ret += '\n';
                }
            }
            ret += qmlToString2(object[i]);
        }
        break; }
    case 'object':
        for (var i in object) {
            ret += i + " " + typeof object[i] + " " + object[i] + '\n';
        }
        break;
    case 'array':
        for (var i = 0; i < object.length; i++) {
            ret += qmlToString(object[i], indent + 1);
        }
        break;
    default:
        console.debug(type);
        break;
    }

    return ret;
}

function QmlHandler() { this._s = []; }
QmlHandler.prototype = {
    import: function(v) { return ['import', v]; }
    , module: function(v) { return ['module', v]; }
    , version: function(v) { return ['version', v]; }
    , namespace: function(v) { return ['namespace', v]; }
    , element: function(v) { return ['element', v]; }
    , property: function(v) { return ['property', v]; }
    , number: function(v) { return ['number', v]; }
    , string: function(v) { return ['string', '"' + v + '"']; }
    , keyword: function(v) { return ['keyword', v]; }
    , comment: function(v) { return ['comment', v]; }
    , reference: function(v) { return ['reference', v]; }
    , json: function(v) { return ['json', v]; }
    , blah3: function(v) { return ['blah3', v]; }
    , join: function(v) { return v.join(''); }
    , first: function(v) { return v[0]; }
    , object: function(v) { this._s.push({}); }
    , array: function(v) { this._s.push([]); }
    , member: function(v) { this._s[this._s.length - 1][v[0]] = v[2]; }
    , elem: function(v) { this._s[this._s.length - 1].push(v); }
    , trueV: function(v) { return true; }
    , falseV: function(v) { return false; }
    , nullV: function(v) { return null; }
    , pop: function(v) { return this._s.pop(); }
}

function highlightQml(text) {
    Gin.ELEMENT = new Gin.Parser.RegExp(/[A-Z][A-Za-z0-9_]*/);
    Gin.STRING = new Gin.Parser.RegExp(/[A-Za-z][A-Za-z0-9_]*/);
    Gin.COMMENT = new Gin.Parser.RegExp(/([^\*]|\*[^\/])*/);
    Gin.COMMENT2 = new Gin.Parser.RegExp(/[^\n]*/);

    var qml = new Gin.Grammar({
                                  QML: / Comment* Import* Comment* Element* Comment* /
                                  , Import: / 'import':keyword (($STRING ('\.' $STRING)*):join:module | $JS_STRING:string ) ($INT '\.' $INT):join:version ? ('as':keyword $ELEMENT:namespace)* ::import/
                                  , Element: / $ELEMENT '\{' Children '\}' ::element /
                                  , Children: / (Property | Element | Blah3 | Comment)* /
                                  , Blah3: / ('\.'+):join ::blah3 /
                                  , Property: / $STRING ':' Value ';'? ::property /
                                  , Value: / JSON:json | $REAL:number | $JS_STRING:string | ($STRING ( '.' $STRING )*):join:reference ::first /
                                  , Comment: / (('\/\*' $COMMENT '\*\/') | ('\/\/' $COMMENT2)):join ::comment /
                                  , JSON: / Object | Array ::first /
                                  , Object: / '\{':object Member* (',' Member)* '\}' ::pop /
                                  , Member: / ($JS_STRING ':' Value):member /
                                  , Array: / '\[':array Elem* (',' Elem)* '\]' ::pop /
                                  , Elem: / Val:elem /
                                  , Val: / Keyword | $REAL | $JS_STRING | Object | Array ::first /
                                  , Keyword: / 'true':trueV | 'false':falseV | 'null':nullV ::first /
                              }, "QML", Gin.SPACE);
    var handler = new QmlHandler();
    var match = qml.parse(text, handler);

    if (!match || !match.full) {
        return text;
    }
    var target = match.value;

    return qmlToString(target);
}

function spaces(size) {
    var ret = ''
    for (var i = 0; i < size * 4; i++) ret += ' ';
    return ret;
}

function highlightJsonSub(val, indent)
{
    if (typeof indent === 'undefined') indent = 0;
    var ret = ''
    switch (typeof val) {
    case 'object':
        var array = (typeof val.push == 'function');
        if (array)
            ret += '[';
        else
            ret += '{';
        var first = true;
        for (var i in val) {
            if (first)
                first = false;
            else
                ret += ',';
            ret += '\n';
            ret += spaces(indent + 1) + '<span class="key">"' + i + '"</span>: ';
            ret += highlightJsonSub(val[i], indent + 1);
        }
        if (!first) {
            ret += '\n' + spaces(indent);
        }

        if (array)
            ret += ']';
        else
            ret += '}';
        break;
    case 'string':
        ret += '<span class="string">"' + val + '"</span>';
        break;
    case 'number':
        ret += '<span class="number">' + val + '</span>';
        break;
    }
    return ret;
}

function JsonHandler() { this._s = []; }
JsonHandler.prototype = {
    object: function(v) { this._s.push({}); },
    array: function(v) { this._s.push([]); },
    member: function(v) { this._s[this._s.length - 1][v[0]] = v[2]; },
    element: function(v) { this._s[this._s.length - 1].push(v); },
    trueV: function(v) { return true; },
    falseV: function(v) { return false; },
    nullV: function(v) { return null; },
    pop: function(v) { return this._s.pop(); },
    first: function(v) { return v[0]; }
}

function highlightJson(text) {
    var json = new Gin.Grammar({
                                   JSON: / Object | Array ::first /,
                                   Object: / '\{':object Member* (',' Member)* '\}' ::pop /,
                                   Member: / ($JS_STRING ':' Value):member /,
                                   Array: / '\[':array Element* (',' Element)* '\]' ::pop /,
                                   Element: / Value:element /,
                                   Value: / Keyword | $REAL | $JS_STRING | Object | Array ::first /,
                                   Keyword: / 'true':trueV | 'false':falseV | 'null':nullV ::first /
                               }, "JSON", Gin.SPACE);
    var handler = new JsonHandler();
    var match = json.parse(text, handler);

    if (!match || !match.full) {
        return text;
    }
    var target = match.value;
    console.debug(typeof target);
    for (var i in target.listen) {
        console.debug(i, target.listen[i], typeof target.listen[i]);
    }

    return highlightJsonSub(match.value);
}
