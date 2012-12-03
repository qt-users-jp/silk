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

function QtConstructor() {}
QtConstructor.prototype = {
    formatDateTime: function (value, format) {
        var ret = format;
        function nn(n) {
            return n < 10 ? '0' + n : n
        }
        function nnn(n) {
            return n < 100 ? '0' + nn(n) : n
        }

        var mmmm = ['January', 'February', 'March', 'April', 'May', 'June', 'July', 'August', 'September', 'October', 'November', 'December'];
        var dddd = ['Sunday', 'Monday', 'Tuesday', 'Wednesday', 'Thursday', 'Friday', 'Saturday'];
        ret = ret.replace(/yyyy/g, value.getFullYear());
        ret = ret.replace(/yy/g, value.getYear());
        ret = ret.replace(/MMMM/g, mmmm[value.getMonth()]);
        ret = ret.replace(/MMM/g, mmmm[value.getMonth()].substr(0, 3));
        ret = ret.replace(/MM/g, nn(value.getMonth() + 1));
        ret = ret.replace(/M/g, value.getMonth() + 1);
        ret = ret.replace(/dddd/g, dddd[value.getDay()]);
        ret = ret.replace(/ddd/g, dddd[value.getDay()].substr(0, 3));
        ret = ret.replace(/dd/g, nn(value.getDate()));
        ret = ret.replace(/d/g, value.getDate());

        ret = ret.replace(/hh/g, nn(value.getHours()));
        ret = ret.replace(/h/g, value.getHours());
        ret = ret.replace(/mm/g, nn(value.getMinutes()));
        ret = ret.replace(/m/g, value.getMinutes());
        ret = ret.replace(/ss/g, nn(value.getSeconds()));
        ret = ret.replace(/s/g, value.getSeconds());
        ret = ret.replace(/zzz/g, nnn(value.getMilliseconds()));
        ret = ret.replace(/z/g, value.getMilliseconds());

        ret = ret.replace(/AP/g, value.getHours() < 12 ? 'AM' : 'PM');
        ret = ret.replace(/ap/g, value.getHours() < 12 ? 'am' : 'pm');

        return ret;
    }
}

var Qt = new QtConstructor();

(function($) { $(function() {

    function vboxLayout($vbox, animate) {
        // lay out children
        var $contents = $vbox.contents();
        var height_remains = $vbox.height();
        var hint_sum = 0;

        $contents.each(
                    function() {
                        switch ($(this).attr('data-policy-v')) {
                        case '0':
                            height_remains -= parseInt($(this).attr('data-h'));
                            break;
                        case '1':
                            hint_sum += parseInt($(this).attr('data-h')) * parseInt($(this).attr('data-hint-v'));
                            break;
                        }
                    }
                    );

        var top = 0;
        $contents.each(
                    function(i, e) {
                        var h = parseInt($(this).attr('data-h'));
                        switch ($(this).attr('data-policy-v')) {
                        case '0': // fixed height
                            break;
                        case '1': // expand height
                            h = Math.round(h * parseInt($(this).attr('data-hint-v')) * height_remains / hint_sum);
                            break;
                        }
                        var ih = h - Math.round(parseFloat($(this).css('margin-top'))) - Math.round(parseFloat($(this).css('border-top-width')));
                        ih = ih - Math.round(parseFloat($(this).css('margin-bottom'))) - Math.round(parseFloat($(this).css('border-bottom-width')));
                        if (animate) {
                            $(this).animate({'top': top, 'left': 0, 'width': '100%', 'height': ih}, 250);
                        } else {
                            $(this).css({'top': top, 'left': 0, 'width': '100%', 'height': ih});
                        }
                        $(this).attr('data-h', h / parseInt($(this).attr('data-hint-v')));
                        top += h;
                    }
                    )
    }

    function hboxLayout($vbox, animate) {

        // lay out children
        var $contents = $vbox.contents();
        var width_remains = $vbox.width();
        var hint_sum = 0;

        $contents.each(
                    function() {
                        switch ($(this).attr('data-policy-h')) {
                        case '0':
                            width_remains -= parseInt($(this).attr('data-w'));
                            break;
                        case '1':
                            hint_sum += parseInt($(this).attr('data-w')) * parseInt($(this).attr('data-hint-h'));
                            break;
                        }
                    }
                    );

        var left = 0;
        $contents.each(
                    function() {
                        var w = parseInt($(this).attr('data-w'));
                        switch ($(this).attr('data-policy-h')) {
                        case '0': // fixed width
                            break;
                        case '1': // expand width
                            w = Math.round(w * parseInt($(this).attr('data-hint-h')) * width_remains / hint_sum);
                            break;
                        }
                        if (animate) {
                            $(this).animate({'top': 0, 'left': left, 'width': w, 'height': '100%'}, 250);
                        } else {
                            $(this).css({'top': 0, 'left': left, 'width': w, 'height': '100%'});
                        }
                        $(this).attr('data-w', w / parseInt($(this).attr('data-hint-h')));
                        left += w;
                    }
                    )
    }

    function stackedLayout($stacked, animate) {
        var width = $stacked.width()
        var height = $stacked.height()
        // lay out children
        $stacked.contents().each(
                    function() {
                        var w = parseInt($(this).attr('data-w'))
                        switch ($(this).attr('data-policy-h')) {
                        case '0': // fixed width
                            $(this).css({'left': (width - w) / 2, 'width': w})
                            break
                        case '':
                        case '1': // expand width
                            $(this).css('width', '100%')
                            break
                        }

                        var h = parseInt($(this).attr('data-h'))
                        switch ($(this).attr('data-policy-v')) {
                        case '0': // fixed height
                            $(this).css({'top': (height - h) / 2, 'height': h})
                            break
                        case '':
                        case '1': // expand height
                            $(this).css('height', '100%')
                            break
                        }
                    }
                    )
    }

    $('.window').resize(function() { $(this).contents().css({'width': '100%', 'height': '100%'}) })
    $('.hbox').resize(function() { hboxLayout($(this)) })
    $('.hsplitter').resize(function() { hboxLayout($(this)) })
    $('.vbox').resize(function() { vboxLayout($(this)) })
    $('.vsplitter').resize(function() { vboxLayout($(this)) })
    $('.stacked').resize(function() { stackedLayout($(this)) })

    $('.window').resize();
    $('.hbox').resize();
    $('.hsplitter').resize();
    $('.window > .vbox').resize();
    $('.vsplitter').resize();
    $('.stacked').resize();

    $('.hsplitter_handler').mousedown(
                function(e) {
                    var $splitter = $(this);
                    var x_down = e.pageX;
                    $(document).on('mousemove.splitter',
                                   function(e) {
                                       var left = Math.round($splitter.position().left);
                                       var $prev = $splitter.prev();
                                       var $next = $splitter.next();
                                       var min = Math.round($prev.position().left);
                                       var max = Math.round($next.position().left) + $next.width() - $splitter.width();
                                       var x1 = left - x_down + e.pageX;
                                       var x2 = Math.max(min, Math.min(x1, max));
                                       $splitter.css('left', x2);
                                       $prev.css('width', x2 - min);
                                       $prev.attr('data-w', $prev.width() / parseInt($prev.attr('data-hint-h')));
                                       $next.css('left', x2 + $splitter.width());
                                       var w;
                                       if ($next.next().length > 0) {
                                           w = Math.round($next.next().position().left) - Math.round($next.position().left);
                                       } else {
                                           w = $next.parent().width() - Math.round($next.position().left);
                                       }
                                       $next.css('width', w);
                                       $next.attr('data-w', w / parseInt($next.attr('data-hint-h')));
                                       x_down = e.pageX - x1 + x2;
                                       return false;
                                   }
                                   ).one('mouseup', function(e) {
                                       $(document).off('mousemove.splitter');
                                   });
                    return false;
                });
    $('.vsplitter_handler').mousedown(
                function(e) {
                    var $splitter = $(this);
                    var y_down = e.pageY;
                    $(document).on('mousemove.splitter',
                                   function(e) {
                                       var top = Math.round($splitter.position().top);
                                       var $prev = $splitter.prev();
                                       var $next = $splitter.next();
                                       var min = Math.round($prev.position().top);
                                       var max = Math.round($next.position().top) + $next.height() - $splitter.height();
                                       var y1 = top - y_down + e.pageY;
                                       var y2 = Math.max(min, Math.min(y1, max));

                                       $splitter.css('top', y2);
                                       $prev.css('height', y2 - min);
                                       $prev.attr('data-h', $prev.height() / parseInt($prev.attr('data-hint-w')));
                                       $next.css('top', y2 + $splitter.height());
                                       var h;
                                       if ($next.next().length > 0) {
                                           h = Math.round($next.next().position().top) - Math.round($next.position().top);
                                       } else {
                                           h = $next.parent().height() - Math.round($next.position().top);
                                       }
                                       $next.css('height', h);
                                       $next.attr('data-h', h / parseInt($next.attr('data-hint-v')));
                                       y_down = e.pageY - y1 + y2;
                                       return false;
                                   }
                                   ).one('mouseup', function(e) {
                                       $(document).off('mousemove.splitter');
                                   });
                    return false;
                });
    $('.tree-toggle', $('.treeview')).click(function(){
        switch($(this).attr('src')) {
        case '/icons/tree-empty.png':
            break;
        case '/icons/tree-open.png':
            $(this).attr('src', '/icons/tree-closed.png');
            $(this).siblings('ul').slideUp('fast');
            break;
        case '/icons/tree-closed.png':
            $(this).attr('src', '/icons/tree-open.png');
            $(this).siblings('ul').slideDown('fast');
            break;
        }
    })

}); })(jQuery);
