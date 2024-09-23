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
	 
        public:
		explicit SharingService(string service,vector<string> supportedFormats, function<void(const string path)> openFunc);
		function<void(const string & path)>OpenFile;
		
	public slots:
		void start();
	private:
		QString service;
		QStringList supportedFormats;

};		



//#include "SharingService.moc"
#endif // SHARING_SERVICE_H
