QT       += core gui network
QT       += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET		= tekControl
TEMPLATE	= app
QMAKE_CXXFLAGS += -std=c++1y

SOURCES += \
	src/observer/observer.cpp \
	src/ui/main.cpp \
	src/ui/mainwindow.cpp \
	src/ui/tekoutput.cpp \
	src/ui/teksettings.cpp \
    src/ui/plots.cpp \
    src/module/adc.cpp \
    src/module/client.cpp \
    src/module/host.cpp \
    src/module/master.cpp \
    src/module/queueofmessages.cpp \
    src/module/registers.cpp \
    src/module/station.cpp \
    src/tekVisa/genmodule.cpp \
    src/module/paamodule.cpp \
    src/calibration/calibration.cpp \
    src/calibration/processing.cpp \
    src/calibration/calculation.cpp

HEADERS  += \
	src/observer/observer.h \
	src/tekVisa/tektypes.h \
	src/ui/mainwindow.h \
	src/ui/tekoutput.h \
	src/ui/teksettings.h \
        include/visa/visa.h \
        include/visa/visatype.h \
    src/ui/plots.h  \
    src/module/adc.h \
    src/module/client.h \
    src/module/host.h \
    src/module/master.h \
    src/module/queueofmessages.h \
    src/module/registers.h \
    src/module/station.h \
    src/module/paamodule.h \
    src/tekVisa/genmodule.h \
    src/ui/types.h \
    src/calibration/calibration.h \
    src/calibration/processing.h \
    src/calibration/calculation.h

LIBS        +=	-L$$PWD/libs/ -lvisa
#LIBS         += -lvisa
LIBS        += -lqcustomplot
INCLUDEPATH +=	  $$PWD/lib/

release {
	QMAKE_CXXFLAGS_RELEASE	-= -O1
	QMAKE_CXXFLAGS_RELEASE	-= -O2
	QMAKE_CXXFLAGS_RELEASE	+= -O3
}
