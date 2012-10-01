TARGET = ssso
include(../../../../silkapp.pri)

SOURCES += main.cpp

RESOURCES += \
    ssso.qrc

OTHER_FILES += \
    sssorc \
    contents/index.qml \
    contents/signup.qml \
    contents/password.qml \
    contents/SSSOBase.qml \
    contents/UserInput.qml \
    contents/SSSODatabase.qml \
    contents/AccountModel.qml \
    contents/SessionModel.qml \
    contents/SignupModel.qml

