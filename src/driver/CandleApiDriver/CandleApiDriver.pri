QMAKE_CFLAGS += -std=gnu11

SOURCES += \
    $$PWD/CandleApiDriver.cpp \
    $$PWD/CandleApiInterface.cpp \
    $$PWD/api/candle.c \
    $$PWD/api/candle_ctrl_req.c \
    $$PWD/CandleApiTiming.cpp

#win32:LIBS += -L$$PWD/api/ -lcandle_api
win32: LIBS += -lsetupapi
win32: LIBS += -lole32
win32: LIBS += -lwinusb

HEADERS += \
    $$PWD/api/candle.h \
    $$PWD/CandleApiDriver.h \
    $$PWD/CandleApiInterface.h \
    $$PWD/api/candle_ctrl_req.h \
    $$PWD/api/candle_defs.h \
    $$PWD/api/ch_9.h \
    $$PWD/CandleApiTiming.h

FORMS +=
