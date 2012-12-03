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

(function($) { $(function() {

    var firstTime = true;
    var currentDirectory = '';
    var currentFile = '';
    var currentMimeType = 'inode/directory';

    $('input[type="checkbox"]', $('.treeview')).click(function(){
        var $ul = $(this).siblings('ul');
        if ($(this).attr('checked') === 'checked') {
            $('input[type="checkbox"]', $ul).removeAttr('disabled');
            if ($ul.css('display') !== 'none')
                $ul.fadeTo('fast', 1.0);
            else
                $ul.css('opacity', 1.0);
        } else {
            $('input[type="checkbox"]', $ul).attr('disabled', 'disabled');
            if ($ul.css('display') !== 'none')
                $ul.fadeTo('fast', 0.5);
            else
                $ul.css('opacity', 0.5);
        }
    })

    if (window.history && window.history.pushState) {
        $(window).on('popstate', function(e) {
            if (firstTime) {
                init();
                firstTime = false;
                return;
            }
            var state = e.originalEvent.state;
            pathChanged();
          });
    }

    function breadcrumbClicked() {
        if (window.history && window.history.pushState) {
            $('#breadcrumb a').click(function(e) {
                if (ws.readyState !== 1) return;
                var link = e.currentTarget;
                if (e.which > 1 || e.metaKey || e.ctrlKey || e.shiftKey || e.altKey)
                    return;
                if (location.protocol !== link.protocol || location.host !== link.host)
                    return;
                if (link.hash && link.href.replace(link.hash, '') === location.href.replace(location.hash, ''))
                  return;

                setDirectory(params('path', link.search), $(this));
                e.preventDefault();
            });
        }
    }

    function addEventHandlerForPath(selector) {
        if (window.history && window.history.pushState) {
            $(selector).click(function(e) {
                var link = e.currentTarget;
                if (e.which > 1 || e.metaKey || e.ctrlKey || e.shiftKey || e.altKey)
                    return;
                if (location.protocol !== link.protocol || location.host !== link.host)
                    return;
                if (link.hash && link.href.replace(link.hash, '') === location.href.replace(location.hash, ''))
                  return

                if (ws.readyState === 1) {
                    setPath(link.href, $(this));
//                    pathChanged();
                    e.preventDefault();
                }
            });
        }
    }

    breadcrumbClicked();
    addEventHandlerForPath('#entries a');
    addEventHandlerForPath('#histories a');

    var m_directory;
    var m_file;
    var m_backup;

    function getMimeType($item) {
        return $item.children('img').attr('title');
    }

    function params(key, str) {
        if (!str) str = location.search;
        if (str.length > 0) {
            var items = str.substr(1).split(/&/);
            for (var i = 0; i < items.length; i++) {
                var pair = items[i].split(/=/);
                if (pair[0] === key) return pair[1];
            }
        }
        return undefined;
    }

    function init() {
        m_directory = '';
        m_file = {};
        m_backup = -1;
        $('#breadcrumb a').each(function(i, e) {
            m_file.mimeType = getMimeType($(this));
            m_file.name = $(this).text();
            if (m_file.mimeType === 'inode/directory')
                m_directory += m_file.name + '/';
        })

        m_backup = params('backup');
    }

    function setDirectory(directory, $sender) {
        console.debug(m_directory, directory);
        if (m_directory === directory) return;
        var item;
        if (m_directory.length > directory.length) {
            item = m_directory.substr(1, directory.length - 1);
            item = item.substr(0, item.length - 1);
            console.debug(item);
        } else {
            item = directory.substr(m_directory.length);
            item = item.substr(0, item.length - 1);
            console.debug(item);
//            console.debug(item.split(/\//g));
        }

        m_directory = directory;
//        history.pushState(null, null, "/?path=" + directory);
    }

    function setPath(path, $sender) {
        history.pushState(null, null, path);
        var backup = params('backup');
        if (backup < 0) {
            var mimeType = getMimeType($sender);
        } else {
//            setBackup(backup);
        }
    }

    function pathChanged() {
//        $('#loading').show();
//        $('#loading').fadeTo('fast', 0.25);

        var path = '/';
        var backup = -1;
        if (location.search.length > 0) {
            var items = location.search.substr(1).split(/&/);
            for (var i = 0; i < items.length; i++) {
                var pair = items[i].split(/=/);
                switch (pair[0]) {
                case 'path':
                    path = pair[1];
                    break;
                case 'backup':
                    backup = pair[1];
                    break;
                }
            }
        }

        if (backup < 0) {
            ws.send(JSON.stringify({action: 'breadcrumb', path: path}));
        } else {
            ws.send(JSON.stringify({action: 'backup', path: path}));
        }
    }

    var ws;
    var url = 'ws://' + webSocketServer.host;
    if (webSocketServer.port !== 80)
        url += ':' + webSocketServer.port;
    url += '/server.qml';

//    if ("WebSocket" in window) {
//        ws = new WebSocket(url, ["carpet"]);
//    } else if ("MozWebSocket" in window) {
//        ws = new MozWebSocket(url, ["carpet"]);
//    }
//    ws.onopen = function() {
//        ws.onmessage = function(m) {
////            console.debug(m.data);
//            var message = JSON.parse(m.data);
////            console.debug(message.action);
//            switch (message.action) {
//            case 'breadcrumb':
//                setBreadCrumb(message.data);
//                switch (currentMimeType) {
//                case 'inode/directory':
//                    message.action = 'entries';
//                    break;
//                default:
//                    message.action = 'histories';
//                    break;
//                }
//                message.data = '';
//                ws.send(JSON.stringify(message));
//                break;
//            case 'entries':
//                setEntries(message.data);
//                $('#histories').empty();
//                message.action = 'histories';
//                message.data = '';
//                ws.send(JSON.stringify(message));
//                $('#loading').fadeTo('fast', 0.0, function() { $(this).hide(); })
//                break;
//            case 'histories':
//                setHistories(message.data);
//                $('#contents').empty();
//                message.action = 'contents';
//                message.data = '';
//                ws.send(JSON.stringify(message));
//                break;
//            case 'contents':
//                var $contents = $('#contents');
//                $contents.empty();
//                $contents.html(message.data.replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;').replace(/"/g, '&quot;'));
//                $('#loading').fadeTo('fast', 0.0, function() { $(this).hide(); })
//                break;
//            }
//        }
//    }

    function createElementForFile(item, suffix) {
        var ret = '<li'
        if (item.removed) ret += ' class="removed"';
        ret += '><a href="/?path=' + item.path;
        if (item.mime === 'inode/directory') ret += '/';
        ret += '"><img src="' + iconNameForMimeType(item.mime) + '" width="22px" height="22px" title="' + item.mime + '"/>' + item.name;
        ret += '</a>';
        if (item.mime === 'inode/directory') ret += suffix;
        ret += '</li>';
        return ret;
    }

    function createElementForHistory(item) {
        var ret = '<li><a href="/?path=' + item.path;
//        if (item.mime === 'inode/directory') ret += '/';
        var dt = new Date(item.datetime);
        var name = Qt.formatDateTime(dt, 'yyyy/MM/dd hh:mm');
        ret += '&backup=' + item.id + '"><img src="' + iconNameForMimeType(item.mime) + '" width="22px" height="22px" title="' + item.mime + '"/>' + name + '</a></li>';
        return ret;
    }

    function setBreadCrumb(items) {
        var $breadcrumb = $('#breadcrumb');
        $breadcrumb.empty();

        var directory = '/';
        currentMimeType = 'inode/directory';
        $breadcrumb.append('<li><a href="/"><img src="/icons/folder-blue.png" width="22px" height="22px" title="inode/directory" /></a>/</li>');

        for (var i = 0; i < items.length; i++) {
            var item = items[i];
            currentMimeType = item.mime;
            if (currentMimeType === 'inode/directory') {
                directory += item.name + '/';
                $breadcrumb.append(createElementForFile(item, '/'));
            } else {
                currentFile = item.name;
            }
        }
        if (currentDirectory != directory) {
            $entries = $('#entries');
            if (currentDirectory.length < directory.length) {
                $entries.css('overflow', 'hidden');
                $entries.animate({left: -$(this).width()}, 'fast', null, function() { $(this).css('left', $(this).width()); });
            } else {
                $entries.css('overflow', 'hidden');
                $entries.animate({left: $(this).width()}, 'fast', null, function() { $(this).css('left', -$(this).width()); });
            }

            currentDirectory = directory;
        }
        addEventHandlerForPath('#breadcrumb a');
    }

    function setEntries(items) {
        var $entries = $('#entries');
        $entries.empty();

        for (var i = 0; i < items.length; i++) {
            var item = items[i];
            $entries.append(createElementForFile(item, ''));
        }
        $entries.animate({left: 0}, 'fast', null, function() { $(this).css('overflow', 'auto'); });
        addEventHandlerForPath('#entries a');
    }

    function setHistories(items) {
        var $histories = $('#histories');
        $histories.empty();

        for (var i = 0; i < items.length; i++) {
            var item = items[i];
            $histories.append(createElementForHistory(item));
        }
        addEventHandlerForPath('#histories a');
    }

    $('#add-exclude-pattern').click(function() {
        $('#add-exclude-pattern-dialog').fadeTo('fast', 1.0)
        $('#pattern').focus()
    })

    $('.remove-exclude-pattern').click(function() {
        var $dialog = $('#remove-exclude-pattern-dialog')
        $('#remove-pattern', $dialog).text($(this).attr('data-pattern'))
        $('#remove-pattern-id', $dialog).val($(this).attr('data-pattern-id'))
        $dialog.fadeTo('fast', 1.0)
    })

    $('.dialog-background').click(function() {
        $(this).parent().parent().fadeTo('fast', 0.0, function() { $(this).hide() })
    })

}); })(jQuery);
