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

import Silk.HTML 5.0
import Silk.Cache 1.0
import "./components"

Html {
    Cache { id: cache }

    UserInput {
        id: input

        property string action
        property string oauth_token
        property string oauth_verifier

        onSubmit: {
            var responseCookies = http.responseCookies;
            switch(input.action) {
            case 'authorize':
                http.loading = true;
                twitter.requestToken();
                break;
            case 'unauthorize':
                twitter.token = '';
                twitter.tokenSecret = '';
                twitter.user_id = '';
                twitter.screen_name = '';
                responseCookies.session_id = {value: undefined, expires: new Date(0)};
                cache.remove(http.requestCookies.session_id);
                break;
            default:
                if (input.oauth_verifier.length > 0) {
                    http.loading = true;
                    twitter.token = input.oauth_token;
                    twitter.tokenSecret = cache.fetch(input.oauth_token);
                    cache.remove(input.oauth_token);
                    twitter.accessToken(input.oauth_verifier);
                } else {
                    if (typeof http.requestCookies.session_id !== 'undefined') {
                        var session_data = cache.fetch(http.requestCookies.session_id.value);
                        if (typeof session_data !== 'undefined') {
                            http.loading = true;
                            twitter.token = session_data.token;
                            twitter.tokenSecret = session_data.tokenSecret;
                            if (typeof session_data.user_id !== 'undefined' ) twitter.user_id = session_data.user_id;
                            if (typeof session_data.screen_name !== 'undefined' ) twitter.screen_name = session_data.screen_name;

                            var request = new XMLHttpRequest();
                            request.onreadystatechange = function() {
                                switch (request.readyState) {
                                case 4: { // Done
                                    var result = JSON.parse(request.responseText);
                                    profile_image.src = result.profile_image_url;
                                    name.text = result.name;
                                    http.loading = false;
                                    break }
                                }
                            }
                            var header = twitter.authHeader('GET', 'https://api.twitter.com/1.1/account/verify_credentials.json');
                            request.open('GET', 'https://api.twitter.com/1.1/account/verify_credentials.json');
                            request.setRequestHeader('Authorization', header);
                            request.send();
                        }
                    }
                }

                break;
            }
            http.responseCookies = responseCookies;
        }
    }

    Twitter {
        id: twitter
        onAuthorize: {
            cache.add(twitter.token, twitter.tokenSecret);
            http.status = 301;
            http.responseHeader = {'Content-Type': 'text/plain; charset=utf-8', 'Location': url};
            http.loading = false;
        }
        onAuthorizedChanged: {
            if (authorized) {
                var session_id = Silk.uuid();
                var session_data = {}
                session_data.token = twitter.token;
                session_data.tokenSecret = twitter.tokenSecret;
                session_data.user_id = twitter.user_id;
                session_data.screen_name = twitter.screen_name;
                cache.add(session_id, session_data);

                var cookies = http.responseCookies;
                cookies.session_id = { 'value': session_id, 'httponly': true };
                http.responseCookies = cookies;

                http.status = 301
                http.responseHeader = {'Content-Type': 'text/plain; charset=utf-8', 'Location': '/examples/oauth.qml'}
            }
            http.loading = false
        }
    }

    Head {
        enabled: http.status === 200
        Title { id: title; text: "OAuth" }
    }

    Body {
        enabled: http.status === 200
        H1 { text: title.text }
        P {
            enabled: twitter.consumerKey.length === 0
            Text {
                text: 'to test oauth.qml you have to add oauth config to <a href="/config.qml">silkrc</a> like below.'
            }
            Pre {
                text: '{
    "oauth:" {
        "consumerKey": "<em>consumerKey</em>"
        , "consumerSecret": "<em>consumerSecret</em>"
    }
}'
            }
        }
        P {
            enabled: profile_image.src.length > 0
            Img { id: profile_image; }
            Text { id: name }
        }

        Form {
            id: form
            enabled: twitter.consumerKey.length > 0
            method: 'POST'
            Input { type: 'submit'; value: twitter.user_id.length === 0 ? 'Sign in with twitter' : 'Sign out' }
            Input { type: 'hidden'; name: 'action'; value: twitter.user_id.length === 0 ? 'authorize' : 'unauthorize' }
        }
    }
}
