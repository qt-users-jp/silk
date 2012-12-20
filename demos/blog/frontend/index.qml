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

import QtQml 2.0
import Silk.HTML 5.0
import Silk.Cache 1.0
import Silk.Database 1.0
import Silk.Utils 1.0
import './components/'

Theme {
    id: root

    Cache { id: cache }
    SilkConfig {
        id: config
        property variant blog: {author: 'task_jp'; database: ':memory:'}
    }

    UserInput {
        id: input
        property string action
        property string subaction

        // for list
        property string label: ''
        property int page: 0

        // for details
        property int no
        property string slug

        // for edit
        property string title
        property string head
        property string body
        property string body2
        property string yymmdd: ''
        property string hhmm: ''

        // for login
        property string oauth_token
        property string oauth_verifier

        onSubmit: {
            // account
            switch(input.action) {
            case 'login':
                account.login()
                return
            case 'logout':
                account.logout()
                return
            default:
                if (input.oauth_verifier.length > 0) {
                    account.verified()
                    return
                } else if (typeof http.requestCookies.session_id !== 'undefined') {
                    var session_data = cache.fetch(http.requestCookies.session_id.value)
                    if (typeof session_data !== 'undefined') {
                        account.user_id = session_data.user_id
                        account.screen_name = session_data.screen_name
                    }
                }
                break
            }

            // edit
            if (account.loggedIn) {
                switch(input.action) {
                case 'edit':
                    switch(input.subaction) {
                    case 'save':
                        editor.save()
                        break
                    default:
                        editor.load()
                        break
                    }
                    break
                case 'remove':
                    switch(input.subaction) {
                    case 'remove':
                        editor.remove()
                        break
                    default:
                        articleModel.condition = 'id=%1'.arg(input.no)
                        articleModel.select = true
                        editor.confirmRemove = true
                        break
                    }
                    break
                default:
                    break
                }
            }

            // view
            if (input.action.length === 0) {
                if (input.page > 0)
                    articleModel.offset = articleModel.limit * input.page
                if (input.no > 0)
                    articleModel.condition = 'id=%1'.arg(input.no)
                articleModel.select = true
            }
        }
    }

    QtObject {
        id: account
        property bool loggedIn: account.user_id.length > 0 && account.screen_name.length > 0
        property string user_id
        property string screen_name

        function login() {
            http.loading = true
            twitter.callbackUrl = '%1://%2:%3/'.arg(http.scheme).arg(http.host).arg(http.port)
            twitter.requestToken()
        }

        function logout() {
            twitter.token = '';
            twitter.tokenSecret = '';
            twitter.user_id = '';
            twitter.screen_name = '';
            var cookies = http.responseCookies
            cookies.session_id = { value: undefined, expires: new Date(0) }
            http.responseCookies = cookies
            cache.remove(http.requestCookies.session_id)
            http.status = 302
            http.responseHeader = {'Content-Type': 'text/plain; charset=utf-8;', 'Location': http.path}
        }

        function authorize(url) {
            cache.add(twitter.token, twitter.tokenSecret)
            http.status = 302
            http.responseHeader = {'Content-Type': 'text/plain; charset=utf-8;', 'Location': url};
            http.loading = false
        }

        function verified() {
            http.loading = true
            twitter.token = input.oauth_token
            twitter.tokenSecret = cache.fetch(input.oauth_token)
            cache.remove(input.oauth_token)
            twitter.accessToken(input.oauth_verifier)
        }

        function authorized() {
            if (twitter.authorized) {
                if (twitter.screen_name === config.blog.author) {
                    var session_id = Silk.uuid()
                    var session_data = {}
                    session_data.token = twitter.token
                    session_data.tokenSecret = twitter.tokenSecret
                    session_data.user_id = twitter.user_id
                    session_data.screen_name = twitter.screen_name
                    cache.add(session_id, session_data)

                    var cookies = http.responseCookies
                    cookies.session_id = { 'value': session_id }
                    http.responseCookies = cookies
                }

            }
            http.status = 302
            http.responseHeader = {'Content-Type': 'text/plain; charset=utf-8;', 'Location': http.path}
            http.loading = false
        }
    }

    Twitter {
        id: twitter
        onAuthorize: account.authorize(url)
        onAuthorizedChanged: account.authorized()
    }

    // Editor
    QtObject {
        id: editor
        property bool isNew: !(input.no > 1)
        property bool confirmRemove: false
        property variant errors: []

        function checkError() {
            var ret = false
            var messages = []
            if (input.title.length === 0) {
                messages.push(qsTr('Title is empty'))
                ret = true
            }

            if (input.slug.length === 0) {
                if (input.title.length !== 0) {
                    input.slug = input.title
                    ret = true
                }
            }

            if (input.body.length === 0) {
                messages.push(qsTr('Body is empty'))
                ret = true
            }

            errors = messages
            return ret
        }

        function load() {
            if (input.no > 0) {
                articleModel.condition = 'id=%1'.arg(input.no)
                articleModel.select = true
                var article = articleModel.get(0)
                input.title = article.title
                input.slug = article.slug
                input.body = article.body
                input.body2 = article.body2
                input.head = article.head
                input.yymmdd = Qt.formatDate(article.published, 'yyyy-MM-dd')
                input.hhmm = Qt.formatTime(article.published, 'hh:mm')
            }
        }

        function save() {
            if (checkError()) return
            if (db.transaction()) {
                var article = {}
                article.user_id = 1
                article.title = input.title
                article.slug = input.slug
                article.body = input.body
                article.body2 = input.body2
                article.head = input.head
                article.published = new Date(input.yymmdd + input.hhmm)

                if (editor.isNew) {
                    input.no = articleModel.insert(article)
                } else {
                    article.id = input.no
                    articleModel.update(article)
                }

                if (db.commit()) {
                    http.status = 302
                    http.responseHeader = {'Content-Type': 'text/plain; charset=utf-8;', 'Location': '%1?no=%2'.arg(http.path).arg(input.no)}
                } else {
                    db.rollback()
                    errors = [qsTr('Save failed. try again')]
                }
            } else {
                errors = [qsTr('Save failed. try again')]
            }
        }

        function remove() {
            if (db.transaction()) {
                articleModel.remove({id: input.no})
                if (db.commit()) {
                    http.status = 302
                    http.responseHeader = {'Content-Type': 'text/plain; charset=utf-8;', 'Location': '%1'.arg(http.path)}
                } else {
                    db.rollback()
                    errors = [qsTr('Delete failed. try again')]
                }
            } else {
                errors = [qsTr('Delete failed. try again')]
            }
        }
    }

    // Viewer
    QtObject {
        id: viewer
        property bool detail: input.no > 0
    }

    Database {
        id: db
        connectionName: 'blog'
        type: "QSQLITE"
        databaseName: config.blog.database
    }

    UserModel { id: userModel; database: db }
    TagModel { id: tagModel; database: db }
    ArticleModel { id: articleModel; database: db; limit: 10 }

    sideBar: [
        Ul {
            Li {
                A {
                    href: 'http://twitter.com/%1'.arg(config.blog.author)
                    target: '_blank'
                    Img { width: '22'; height: '22'; src: 'http://api.twitter.com/1/users/profile_image?screen_name=%1'.arg(config.blog.author) }
                    Text { text: config.blog.author }
                }
                Ul {
                    enabled: account.loggedIn
                    Li {
                        A {
                            href: "/?action=edit"
                            Img { width: '22'; height: '22'; src: '/icons/document-new.png' }
                            Text { text: "New" }
                        }
                    }
                    Li {
                        enabled: account.loggedIn && input.no > 0
                        A {
                            href: "/?action=edit&no=%1".arg(input.no)
                            Img { width: '22'; height: '22'; src: '/icons/document-properties.png' }
                            Text { text: "Edit" }
                        }
                    }
                    Li {
                        enabled: account.loggedIn && input.no > 0
                        A {
                            href: "/?action=remove&no=%1".arg(input.no)
                            Img { width: '22'; height: '22'; src: '/icons/document-close.png' }
                            Text { text: "Delete..." }
                        }
                    }
                    Li {
                        A {
                            href: "/?action=logout"
                            Img { width: '22'; height: '22'; src: '/icons/system-log-out.png' }
                            Text { text: "Logout" }
                        }
                    }
                }
            }
        }
    ]

    Article {
        enabled: input.action === 'edit'
        Header {
            H2 { text: editor.isNew ? qsTr('New') : qsTr('Edit') }
            Repeater {
                model: editor.errors
                Component {
                    H3 {
                        text: model.modelData
                    }
                }
            }
        }
        Form {
            action: http.path
            method: 'POST'
            Input {
                type: 'text'
                name: 'title'
                placeholder: qsTr('Title')
                value: input.title
                property string style: "width: 100%"
            }
            Input {
                type: 'text'
                name: 'slug'
                placeholder: qsTr('Slug')
                value: input.slug
                property string style: "width: 100%"
            }
            TextArea {
                name: 'body'
                placeholder: qsTr('Body')
                rows: '5'
                property string style: "width: 100%"
                Text { text: input.body }
            }
            TextArea {
                name: 'body2'
                placeholder: qsTr('Continue')
                rows: '20'
                property string style: "width: 100%"
                Text { text: input.body2 }
            }
            Input { type: 'date'; name: 'yymmdd'; value: input.yymmdd }
            Input { type: 'time'; name: 'hhmm'; value: input.hhmm }
            Input { type: 'submit'; value: qsTr('Save') }
            Input { type: 'hidden'; name: 'action'; value: input.action }
            Input { type: 'hidden'; name: 'subaction'; value: 'save' }
            Input { type: 'hidden'; name: 'no'; value: input.no }
            TextArea {
                name: 'head'
                placeholder: qsTr('Head')
                property string style: "width: 100%"
                Text { text: input.head }
            }
        }
    }

    // Viewer
    Repeater {
        enabled: input.action !== 'edit'
        model: articleModel
        Component {
            Article {
                Header {
                    H2 {
                        A {
                            href: '%1?no=%2'.arg(http.path).arg(model.id)
                            text: model.title
                        }
                    }
                    P { text: Qt.formatDateTime(model.published, 'yyyy年MM月dd日 hh時mm分') }
                }

                Section {
                    id: body
                    Text { text: model.body }
                    Text {
                        enabled: viewer.detail && model.body2.length > 0
                        text: model.body2
                    }
                    P {
                        A {
                            enabled: !viewer.detail && model.body2.length > 0
                            href: '%1?no=%2'.arg(http.path).arg(model.id)
                            text: qsTr('Continue reading...')
                        }
                    }
                }

                Footer {
                    enabled: editor.confirmRemove
                    H2 { text: qsTr('Are you sure to delete?') }
                    Button {
                        __text: qsTr('Delete')
                        href: '%1?action=remove&subaction=remove&no=%2'.arg(http.path).arg(input.no)
                    }
                    Button {
                        __text: qsTr('Cancel')
                        href: '%1?no=%2'.arg(http.path).arg(input.no)
                    }
                }
            }
        }
    }
}
