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

setInterval("post()", 10000);

function post(element) {
    var request = new XMLHttpRequest();

    request.onreadystatechange = function() {
        switch (request.readyState) {
        case 4: // Done
            readData(request.responseText);
            break
        }
    }

    var data = {}

    var ul = document.getElementById("ul");
    var lis = ul.childNodes;
    if (lis.length > 0) {
        data.since = lis[0].title;
    }

    if (typeof element !== 'undefined') {
        if (element.value.length > 0) {
            data.message = element.value;
        }
    }
    request.open("POST", "chat.qml");
    request.send(JSON.stringify(data));
    if (typeof element !== 'undefined') {
        element.value = ''
    }
}

function readData(data) {
    var ul = document.getElementById("ul");
    var json = JSON.parse(data);
    for (var i = 0; i < json.length; i++) {
        var item = json[i]
        var li = document.createElement("li");
        li.title = item.id
        li.innerHTML = item.message
        if (ul.children.length > 0) {
            ul.insertBefore(li, ul.children[0])
        } else {
            ul.appendChild(li)
        }
    }
}
