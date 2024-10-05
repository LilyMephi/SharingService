#include "SharingService.h"


SharingService::SharingService(string service_s,vector <string> supportedFormats_s,function<void(const string path)> openFile) {
	//Преобразование типов
	 this->service = QString::fromStdString(service_s);
	  for(int i = 0; i < (int) supportedFormats_s.size(); ++i){
		   this-> supportedFormats << QString::fromStdString(supportedFormats_s.at(i));
	}
	this->OpenFile_s = openFile;
        // Регистрация сервиса на потоке D-Bus	
	QDBusConnection dbusConnection = QDBusConnection::sessionBus();
        if (dbusConnection.registerService(service)){
            qInfo() << "Service registered successfully.";
        } else {
            qDebug() << QDBusConnection::sessionBus().lastError().message();
            qFatal("Failed to register service.\n");
        }

        // Регистрация интерфейса  корневым путем "/"
        if (dbusConnection.registerObject("/", this, QDBusConnection::ExportAllSlots | QDBusConnection::ExportAllSignals)) {
            qDebug() << "Object registered successfully";
        } else {
            qDebug() << QDBusConnection::sessionBus().lastError().message();
            qFatal("Failed to register object.\n");
	}
}

void SharingService::OpenFile(const QString &path){
	std::string path_s = path.toStdString();
	this->OpenFile_s(path_s);
	//emit fileOpened(path);
}

void SharingService::start(){
	//Подключаемся к интерфесу для регистрации нашего сервиса
	auto iface = new QDBusInterface("com.system.sharing", "/", "com.system.sharing", QDBusConnection::sessionBus(),this);
        if (!iface->isValid()) {
                qDebug() << iface->lastError().message();
        	qFatal("Invalid interface.\n");
        }
        
	//connect(this, &SharingService::fileOpened,this, &SharingService::OpenFile);
        QDBusMessage reply = iface->call("RegisterService", service, supportedFormats);
        if (reply.type() == QDBusMessage::ErrorMessage) {
                qDebug() << reply.errorMessage();
        	qFatal("Failed to connect to the service interface.\n");
        } else {
        	qInfo() << "The Service registered successfully:" << service;
        }
	//emit fileOpened(path);
}

