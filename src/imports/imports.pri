INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD
QML_IMPORT_PATH += $$PWD

HEADERS += \
    $$PWD/qtquickhttpabstractobject.h \
    $$PWD/qtquickhttpobject.h \
    $$PWD/qtquickhttptextobject.h \
    $$PWD/qtquickhttprepeater.h
SOURCES += \
    $$PWD/qtquickhttpabstractobject.cpp \
    $$PWD/qtquickhttpobject.cpp \
    $$PWD/qtquickhttptextobject.cpp \
    $$PWD/qtquickhttprepeater.cpp

include(./html/html.pri)
include(./css/css.pri)
include(./json/json.pri)

OTHER_FILES += \
    $$PWD/QtQuick/HTML/qmldir \
    $$PWD/QtQuick/HTML/AbstractI18nElement.qml \
    $$PWD/QtQuick/HTML/AbstractI18nCoreattrsEventsElement.qml \
    $$PWD/QtQuick/HTML/AbstractI18nCoreattrsElement.qml \
    $$PWD/QtQuick/HTML/AbstractElement.qml \
    $$PWD/QtQuick/HTML/AbstractCoreattrsElement.qml \
    $$PWD/QtQuick/HTML/Html.qml \
    $$PWD/QtQuick/HTML/Abbr.qml \
    $$PWD/QtQuick/HTML/Acronym.qml \
    $$PWD/QtQuick/HTML/Address.qml \
    $$PWD/QtQuick/HTML/A.qml \
    $$PWD/QtQuick/HTML/Area.qml \
    $$PWD/QtQuick/HTML/Base.qml \
    $$PWD/QtQuick/HTML/Body.qml \
    $$PWD/QtQuick/HTML/Br.qml \
    $$PWD/QtQuick/HTML/Button.qml \
    $$PWD/QtQuick/HTML/Caption.qml \
    $$PWD/QtQuick/HTML/Cite.qml \
    $$PWD/QtQuick/HTML/Code.qml \
    $$PWD/QtQuick/HTML/Col.qml \
    $$PWD/QtQuick/HTML/ColGroup.qml \
    $$PWD/QtQuick/HTML/Dd.qml \
    $$PWD/QtQuick/HTML/Del.qml \
    $$PWD/QtQuick/HTML/Dfn.qml \
    $$PWD/QtQuick/HTML/Div.qml \
    $$PWD/QtQuick/HTML/Dl.qml \
    $$PWD/QtQuick/HTML/Dt.qml \
    $$PWD/QtQuick/HTML/Em.qml \
    $$PWD/QtQuick/HTML/Form.qml \
    $$PWD/QtQuick/HTML/Frame.qml \
    $$PWD/QtQuick/HTML/H1.qml \
    $$PWD/QtQuick/HTML/H2.qml \
    $$PWD/QtQuick/HTML/H3.qml \
    $$PWD/QtQuick/HTML/H4.qml \
    $$PWD/QtQuick/HTML/H5.qml \
    $$PWD/QtQuick/HTML/H6.qml \
    $$PWD/QtQuick/HTML/Head.qml \
    $$PWD/QtQuick/HTML/Hr.qml \
    $$PWD/QtQuick/HTML/IFrame.qml \
    $$PWD/QtQuick/HTML/Img.qml \
    $$PWD/QtQuick/HTML/Input.qml \
    $$PWD/QtQuick/HTML/Ins.qml \
    $$PWD/QtQuick/HTML/Kbd.qml \
    $$PWD/QtQuick/HTML/Label.qml \
    $$PWD/QtQuick/HTML/Legend.qml \
    $$PWD/QtQuick/HTML/Li.qml \
    $$PWD/QtQuick/HTML/Link.qml \
    $$PWD/QtQuick/HTML/Map.qml \
    $$PWD/QtQuick/HTML/Meta.qml \
    $$PWD/QtQuick/HTML/NoFrames.qml \
    $$PWD/QtQuick/HTML/NoScript.qml \
    $$PWD/QtQuick/HTML/Object.qml \
    $$PWD/QtQuick/HTML/Ol.qml \
    $$PWD/QtQuick/HTML/OptGroup.qml \
    $$PWD/QtQuick/HTML/Option.qml \
    $$PWD/QtQuick/HTML/Param.qml \
    $$PWD/QtQuick/HTML/P.qml \
    $$PWD/QtQuick/HTML/Pre.qml \
    $$PWD/QtQuick/HTML/Q.qml \
    $$PWD/QtQuick/HTML/Samp.qml \
    $$PWD/QtQuick/HTML/Script.qml \
    $$PWD/QtQuick/HTML/Select.qml \
    $$PWD/QtQuick/HTML/Span.qml \
    $$PWD/QtQuick/HTML/Strong.qml \
    $$PWD/QtQuick/HTML/Style.qml \
    $$PWD/QtQuick/HTML/Sub.qml \
    $$PWD/QtQuick/HTML/Sup.qml \
    $$PWD/QtQuick/HTML/Table.qml \
    $$PWD/QtQuick/HTML/TBody.qml \
    $$PWD/QtQuick/HTML/Td.qml \
    $$PWD/QtQuick/HTML/TextArea.qml \
    $$PWD/QtQuick/HTML/TFoot.qml \
    $$PWD/QtQuick/HTML/THead.qml \
    $$PWD/QtQuick/HTML/Th.qml \
    $$PWD/QtQuick/HTML/Title.qml \
    $$PWD/QtQuick/HTML/Tr.qml \
    $$PWD/QtQuick/HTML/Ul.qml \
    $$PWD/QtQuick/HTML/Var.qml \
    $$PWD/QtQuick/HTML/BlockQuote.qml \
    $$PWD/QtQuick/HTML/FrameSet.qml \
    $$PWD/QtQuick/HTML/FieldSet.qml \
    $$PWD/QtQuick/CSS/Rule.qml

RESOURCES += \
    $$PWD/imports.qrc
