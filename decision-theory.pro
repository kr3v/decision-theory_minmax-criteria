QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/main.cpp \
    src/math/convex_hull.cpp \
    src/math/double_cmp.cpp \
    src/math/geometry.cpp \
    src/math/minmaxrandomized.cpp \
    src/math/polygon.cpp \
    src/ui/mainwindow.cpp \
    src/ui/practicewidget.cpp \
    src/ui/solverwidget.cpp \
    src/ui/theorywidget.cpp \
    src/ui/utils.cpp

HEADERS += \
    src/math/convex_hull.h \
    src/math/double_cmp.h \
    src/math/geometry.h \
    src/math/minmaxrandomized.h \
    src/math/polygon.h \
    src/ui/mainwindow.h \
    src/ui/practicewidget.h \
    src/ui/solverwidget.h \
    src/ui/theorywidget.h \
    src/ui/utils.h

FORMS += \
    src/ui/mainwindow.ui \
    src/ui/practicewidget.ui \
    src/ui/solverwidget.ui \
    src/ui/theorywidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources/theory.qrc
