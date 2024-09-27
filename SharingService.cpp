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
            qDebug() << "Service registered successfully";
        } else {
            qDebug() << "Failed to register service:" << QDBusConnection::sessionBus().lastError().message();
            exit(1);
        }

        // Регистрация интерфейса  корневым путем "/"
        if (dbusConnection.registerObject("/", this, QDBusConnection::ExportAllSlots | QDBusConnection::ExportAllSignals)) {
            qDebug() << "Object registered successfully";
        } else {
            qDebug() << "Failed to register object:" << QDBusConnection::sessionBus().lastError().message();
            exit(1);
        }
}

SharingService::~SharingService() {
        // Очистка сервиса
        QDBusConnection::sessionBus().unregisterService(service);
        qDebug() << "Service unregistered.";
    }

void SharingService::OpenFile(const QString &path){
	qDebug() << "Try to open File";
	std::string path_s = path.toStdString();
	this->OpenFile_s(path_s);
	emit fileOpened(path);
}

void SharingService::start(const string path_s){
	QString path = QString::fromStdString(path_s);
	qDebug() << path;
	//Подключаемся к интерфесу для регистрации нашего сервиса
	iface = new QDBusInterface("com.system.sharing", "/", "com.system.sharing", QDBusConnection::sessionBus(),this);
        if (!iface->isValid()) {
        	qDebug() << "Invalid interface:" << iface->lastError().message();
        	exit(-1);
        }
        
	connect(this, &SharingService::fileOpened,this, &SharingService::OpenFile);
        QDBusMessage reply = iface->call("RegisterService", service, supportedFormats);
        if (reply.type() == QDBusMessage::ErrorMessage) {
        	qDebug() << "Failed to connect to interface:" << reply.errorMessage();
		exit(-1);
        } else {
        	qDebug() << "service registered successfully:" << service;
        }

	QDBusPendingCall pcall  = iface->asyncCall("OpenFile", path);
	auto watcher = new QDBusPendingCallWatcher(pcall, this);
	QObject::connect(watcher, &QDBusPendingCallWatcher::finished, this,
                     [&](QDBusPendingCallWatcher *w) {
        	     	QDBusPendingReply<void> reply(*w);
			if(reply.isError()){
				qDebug() << "Error to read the file " << reply.error().message();
			}else{
				qDebug() << "Readin the  file was successfule";
			}
        });
	emit fileOpened(path);
}

