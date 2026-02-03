QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
greaterThan(QT_MAJOR_VERSION, 5): QT += openglwidgets

CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../../model/model.cpp \
    QtGifImage/gifimage/qgifimage.cc \
    QtGifImage/giflib/dgif_lib.c \
    QtGifImage/giflib/egif_lib.c \
    QtGifImage/giflib/gif_err.c \
    QtGifImage/giflib/gif_font.c \
    QtGifImage/giflib/gif_hash.c \
    QtGifImage/giflib/gifalloc.c \
    QtGifImage/giflib/quantize.c \
    glwidget.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    ../../model/model.h \
    QtGifImage/gifimage/qgifglobal.h \
    QtGifImage/gifimage/qgifimage.h \
    QtGifImage/gifimage/qgifimage_p.h \
    QtGifImage/giflib/gif_hash.h \
    QtGifImage/giflib/gif_lib.h \
    QtGifImage/giflib/gif_lib_private.h \
    glwidget.h \
    mainwindow.h \
    projection_strategy.h \
    settings_manager.h \
    viewer_facade.h

FORMS += \
    mainwindow.ui

# Общие флаги для C++ (прибавляем, не перезаписываем)
QMAKE_CXXFLAGS += -Wall -Wextra -Wpedantic -Wno-deprecated-declarations -std=c++20 -pthread



# Оптимизации только для релиза
QMAKE_CXXFLAGS_RELEASE += -O3 -march=native 
# QMAKE_LFLAGS_RELEASE += -flto

# Добавляем pthread на линковке (если требуется)
QMAKE_LFLAGS += -pthread

# Определения
DEFINES += NDEBUG

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
