var mimeTypeDatabase = {}
mimeTypeDatabase['inode/directory'] = 'inode-directory'

function iconNameForMimeType(mime) {
    var ret = mimeTypeDatabase[mime]
    if (!ret)
        ret = mime.replace('/', '-')
    return '/icons/mime/' + ret + '.png'
}
