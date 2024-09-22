#ifndef SHARING_SERVICE_H
#define SHARING_SERVICE_H

#include "guilib_global.h"

#include <QCoreApplication>
#include <QDBusConnection>
#include <QDBusError>
#include <QDBusInterface>
#include <QDBusMetaType>
#include <QDebug>
#include <QFile>
#include <QtWidgets> 
#include <QVector>
#include <QDBusReply>

#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

using namespace std;

class QUILIBSHARED_EXPORT SharingService : public QObject {
	Q_OBJECT
	Q_CLASSINFO("D-Bus Interface", "com.system.sharing");
        public:
		explicit SharingService(QObject *parent = nullptr);
	public Q_SLOTS:
		void RegisterService(string e,vector<string> supportedFormats);
                void OpenFile(string path);
	        void OpenFileUsingService(string path,string service);
};		

//include "SharingService.moc"
#endif // SHARING_SERVICE_H
