QT       += core gui
QT += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    creatgamepage.cpp \
    customgamemaneger.cpp \
    custompage.cpp \
    deletegamepage.cpp \
    gamelogicone.cpp \
    gamepageone.cpp \
    gamepagetwo.cpp \
    gamepagewithai.cpp \
    gamesessiondata.cpp \
    leveldata.cpp \
    levelmaneger.cpp \
    leveloriginandautor.cpp \
    levelpage.cpp \
    loadgamepage.cpp \
    main.cpp \
    mainbutton.cpp \
    mainstackedwidget.cpp \
    poptips.cpp \
    rankpage.cpp \
    settingpage.cpp \
    slidingsidebar.cpp \
    startpage.cpp \
    tilebutton.cpp \
    widget.cpp

HEADERS += \
    creatgamepage.h \
    customgamemaneger.h \
    custompage.h \
    deletegamepage.h \
    gamelogicone.h \
    gamepageone.h \
    gamepagetwo.h \
    gamepagewithai.h \
    gamesessiondata.h \
    leveldata.h \
    levelmaneger.h \
    leveloriginandautor.h \
    levelpage.h \
    loadgamepage.h \
    mainbutton.h \
    mainstackedwidget.h \
    poptips.h \
    rankpage.h \
    settingpage.h \
    slidingsidebar.h \
    startpage.h \
    tilebutton.h \
    widget.h

FORMS += \
    creatgamepage.ui \
    custompage.ui \
    deletegamepage.ui \
    gamepageone.ui \
    gamepagetwo.ui \
    gamepagewithai.ui \
    levelpage.ui \
    loadgamepage.ui \
    rankpage.ui \
    settingpage.ui \
    startpage.ui \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
