#include "SharingService.h"


SharingService::SharingService(string service_s,vector <string> supportedFormats_s,function<void(const string path)> openFile) {
	//Преобразование типов
	 this->service = QString::fromStdString(service_s);
	  for(int i = 0; i < (int) supportedFormats_s.size(); ++i){
		   this-> supportedFormats << QString::fromStdString(supportedFormats_s.at(i));
	 }
	this->OpenFile = openFile;
        // Регистрация сервиса на потоке D-Bus
        if (QDBusConnection::sessionBus().registerService(service)) {
            qDebug() << "Service registered successfully";
        } else {
            qDebug() << "Failed to register service:" << QDBusConnection::sessionBus().lastError().message();
            exit(1);
        }

        // Регистрация интерфейса  корневым путем "/"
        if (QDBusConnection::sessionBus().registerObject("/", this, QDBusConnection::ExportAllSlots | QDBusConnection::ExportAllSignals)) {
            qDebug() << "Object registered successfully";
        } else {
            qDebug() << "Failed to register object:" << QDBusConnection::sessionBus().lastError().message();
            exit(1);
        }
	
}

void SharingService::start(){

	//Подключаемся к интерфесу для регистрации нашего сервиса
	QDBusInterface iface("com.system.sharing", "/", "com.system.sharing", QDBusConnection::sessionBus());
        if (!iface.isValid()) {
        	qDebug() << "Invalid interface:" << iface.lastError().message();
        	exit(-1);
        }

        //Регистрируем сервис
        QDBusMessage reply = iface.call("RegisterService", this->service, this->supportedFormats);
        if (reply.type() == QDBusMessage::ErrorMessage) {
        	qDebug() << "Failed to register service:" << reply.errorMessage();
		exit(-1);
        } else {
        	qDebug() << "service registered successfully:" << this->service;
        }
	QString path;
	QTextStream in(stdin);

        // Чтение ввода пользователя
       	qDebug() << "Insert path: (or 'exit' for ending)";
       	in >> path ;

	reply = iface.call("OpenFile",path);
	if (reply.type() == QDBusMessage::ErrorMessage) {
       		qDebug() << "Can not open file: " << reply.errorMessage();
		exit(-1);
        } else {
        	qDebug() << "File opened successfully";
        }
	
	
}
