   TEMPLATE = lib
   TARGET = SharingService
   CONFIG += c++11
   CONFIG += shared
   QT += dbus
   QT += widgets
   QT +=core
   QT -=gui
   
   DEFINES += QUILIB_LIBRARY
   DEFINES += QUILIB_LIBRARY 

   SOURCES += SharingService.cpp
   HEADERS += SharingService.h
   HEADERS += guilib_global.h
