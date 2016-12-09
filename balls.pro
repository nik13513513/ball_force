QT += widgets

CONFIG+= qt c++11
DEFINES *= _USE_MATH_DEFINES

HEADERS += \
    view.h \
    ball_item.h \
    force_item.h \
    widget.h

SOURCES += \
        main.cpp \
    view.cpp \
    ball_item.cpp \
    force_item.cpp \
    widget.cpp \

FORMS    += widget.ui

