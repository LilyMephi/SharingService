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
#include <QDBusMessage>
#include <QDBusPendingCall>

#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

using namespace std;

class QUILIBSHARED_EXPORT SharingService : public QObject {
	Q_OBJECT
	//Q_CLASSINFO("D-Bus Interface", "com.sharing.service")
        public:
		explicit SharingService(string service,vector<string> supportedFormats, function<void(const string path)> openFunc);
		function<void(const string path)>OpenFile_s;
		void start();
	signals:
		void fileOpened(const QString &path);
	public slots:
		void OpenFile(const QString &path);
	private:
		QString service;
		QStringList supportedFormats;
		QDBusInterface *iface;

};		



//#include "SharingService.moc"
#endif // SHARING_SERVICE_H
