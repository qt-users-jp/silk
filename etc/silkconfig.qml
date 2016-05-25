import Silk.Config 0.1

Config {
    listen: Listen {
        address: 'localhost'
        port: 8080
    }

    contents: {
        '/': SILK_DATA_PATH + '/root/'
    }

    daemons: [
        SILK_DATA_PATH + '/daemons/chatdaemon.qml'
    ]

    offlineStoragePath: SILK_DATA_PATH

    imports: []

    cache: {
        'qml': true
    }

    deflate: {
        'video/*': false
        , 'image/*': false
    }
}
