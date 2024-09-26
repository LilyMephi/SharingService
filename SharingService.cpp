#include "SharingService.h"


SharingService::SharingService(string service_s,vector <string> supportedFormats_s,function<void(const string path)> openFile) {
	//Преобразование типов
	 this->service = QString::fromStdString(service_s);
	  for(int i = 0; i < (int) supportedFormats_s.size(); ++i){
		   this-> supportedFormats << QString::fromStdString(supportedFormats_s.at(i));
	 }
	this->onOpenFile = openFile;
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

void SharingService::OpenFile(QString path){
	QFile file(path);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        	qDebug() << "Unable to open file:" << path;
        	exit(-1);
       }    
       QTextStream in(&file);
      	while (!in.atEnd()) {
        	QString line = in.readLine();
        	qDebug() << line; // Выводим содержимое файла в консоль
      }

      file.close();
	/*
	qDebug() << "Try to open File";
	std::string path_s = path.toStdString();
	this->onOpenFile(path_s);
	qDebug()<< "Open successfully";*/
}

void SharingService::start(const string path_s){
	QString path = QString::fromStdString(path_s);
	qDebug() << path;
	//Подключаемся к интерфесу для регистрации нашего сервиса
	QDBusInterface iface("com.system.sharing", "/", "com.system.sharing", QDBusConnection::sessionBus());
        if (!iface.isValid()) {
        	qDebug() << "Invalid interface:" << iface.lastError().message();
        	exit(-1);
        }
        
        QDBusMessage reply = iface.call("RegisterService", this->service, this->supportedFormats);
        if (reply.type() == QDBusMessage::ErrorMessage) {
        	qDebug() << "Failed to register service:" << reply.errorMessage();
		exit(-1);
        } else {
        	qDebug() << "service registered successfully:" << this->service;
        }
	/*
	 QDBusReply<void> replyreg = iface.call("RegisterService",this->service,this->supportedFormats);
    	 	if (!replyreg.isValid()) {
         		qDebug() << "Error to register service:" << replyreg.error().message();
			exit(-1);
		}else{
			qDebug() << "Registered service successfully";
	}*/

	reply = iface.call("OpenFile",path);
	/*if (!replyOpen.isValid()) {
       		qDebug() << "Can not open file: " << replyOpen.error().message();
		exit(-1);
        } else {
        	qDebug() << "File opened successfully";
        }*/

	if (reply.type() == QDBusMessage::ErrorMessage) {
        	qDebug() << "Failed to register service:" << reply.errorMessage();
		exit(-1);
        } else {
        	qDebug() << "service registered successfully:" << this->service;
        }
	
	
}
