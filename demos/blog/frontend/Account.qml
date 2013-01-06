import QtQml 2.0
import Silk.HTML 5.0
import Silk.Cache 1.0
import Silk.Database 1.0
import Silk.Utils 1.0
import './components/'

Twitter {
    id: root

    Cache { id: cache }

    property bool loggedIn: root.user_id.length > 0 && root.screen_name.length > 0
    property string user_id
    property string screen_name
    property string author
    
    onAuthorize: root.redirect(url)
    onAuthorizedChanged: root.storeSession()

    function login() {
        http.loading = true
        root.callbackUrl = '%1://%2:%3/'.arg(http.scheme).arg(http.host).arg(http.port)
        root.requestToken()
    }
    
    function logout() {
        root.token = '';
        root.tokenSecret = '';
        root.user_id = '';
        root.screen_name = '';
        var cookies = http.responseCookies
        cookies.session_id = { value: undefined, expires: new Date(0) }
        http.responseCookies = cookies
        cache.remove(http.requestCookies.session_id)
        http.status = 302
        http.responseHeader = {'Content-Type': 'text/plain; charset=utf-8;', 'Location': http.path}
    }
    
    function redirect(url) {
        cache.add(root.token, root.tokenSecret)
        http.status = 302
        http.responseHeader = {'Content-Type': 'text/plain; charset=utf-8;', 'Location': url};
        http.loading = false
    }
    
    function verified(token, verifier) {
        http.loading = true
        root.token = token
        root.tokenSecret = cache.fetch(token)
        cache.remove(token)
        root.accessToken(verifier)
    }
    
    function restoreSession() {
        var session_data = cache.fetch(http.requestCookies.session_id.value)
        if (typeof session_data !== 'undefined') {
            root.user_id = session_data.user_id
            root.screen_name = session_data.screen_name
        }
    }

    function storeSession() {
        if (root.authorized) {
            if (root.screen_name === root.author) {
                var session_id = Silk.uuid()
                var session_data = {}
                session_data.token = root.token
                session_data.tokenSecret = root.tokenSecret
                session_data.user_id = root.user_id
                session_data.screen_name = root.screen_name
                cache.add(session_id, session_data)
                
                var cookies = http.responseCookies
                cookies.session_id = { 'value': session_id, 'httponly': true }
                http.responseCookies = cookies
            }
            
        }
        http.status = 302
        http.responseHeader = {'Content-Type': 'text/plain; charset=utf-8;', 'Location': http.path}
        http.loading = false
    }
}
