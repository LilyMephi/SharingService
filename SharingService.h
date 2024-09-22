#ifndef SHARING_SERVICE_H
#define SHARING_SERVICE_H

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

class SharingService : public QOBject {
	Q_OBJECT
	Q_CLASSINFO("D-Bus Interface", "com.system.sharing");
        public:
		SharingService();
	public Q_SLOTS:
		void RegisterService(string name,vector<string> supportedFormats);
                void OpenFile(string path);
	        void OpenFileUsingService(string path,string service);
};		

#include "main.moc"

#endif // SHARING_SERVICE_H
