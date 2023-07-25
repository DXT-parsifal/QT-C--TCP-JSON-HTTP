QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT += multimedia
QT += multimediawidgets
CONFIG+=resources_big
QT += network

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    fan_switch.cpp \
    house_control.cpp \
    light_switch.cpp \
    main.cpp \
    mainwindow.cpp \
    media_playback.cpp \
    online_music.cpp \
    temperature_humidity.cpp \
    video_monitor.cpp

HEADERS += \
    fan_switch.h \
    house_control.h \
    light_switch.h \
    mainwindow.h \
    media_playback.h \
    online_music.h \
    temperature_humidity.h \
    video_monitor.h

FORMS += \
    fan_switch.ui \
    house_control.ui \
    light_switch.ui \
    mainwindow.ui \
    media_playback.ui \
    online_music.ui \
    temperature_humidity.ui \
    video_monitor.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
