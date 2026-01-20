QT       += core gui widgets

#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
TEMPLATE = app
TARGET = NetMap3_frontend


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    ../NetMap_backend/app_manager/src/manager.cpp \
    ../NetMap_backend/communications/client/src/Client.cpp \
    ../NetMap_backend/communications/server/src/Server.cpp \
    ../NetMap_backend/motor_controls/src/motor_controller.cpp

HEADERS += \
    mainwindow.h \
  ../NetMap_backend/app_manager/include/manager.hpp \
  ../NetMap_backend/communications/client/include/Client.hpp \
  ../NetMap_backend/communications/server/include/Server.hpp \
  ../NetMap_backend/motor_controls/include/motor_controller.hpp

FORMS += \
    mainwindow.ui

INCLUDEPATH += \
    ../NetMap_backend/app_manager/include \
    ../NetMap_backend/communications/client/include \
    ../NetMap_backend/communications/server/include \
    ../NetMap_backend/motor_controls/include

LIBS +=  -L/usr/local/lib -lpigpiod_if2 -lpigpio


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
