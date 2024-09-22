#include "SharingService.h"


SharingService::SharingService(QObject *parent) : QObject(parent) {
        // Регистрация сервиса на потоке D-Bus
        if (QDBusConnection::sessionBus().registerService("com.system.sharing")) {
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
 
void SharingService::RegisterService(string nam, vector<string> supFormats){
	    // Преобразование типов 
            const  QString name = QString::fromStdString(nam);
	    QStringList supportedFormats;
	    for(int i = 0; i < (int) supFormats.size(); ++i){
		    supportedFormats << QString::fromStdString(supFormats.at(i));
	    }
	    //В config.txt  будем сохранят информацию о сервисах
	    QFile configFile("config.txt");

	    //  Открываем файл чтобы считать  отуда информацию
	    //  если файл не открылся выводим информацию об ошибке 
            if (!configFile.open(QIODevice::ReadOnly | QIODevice::Text)){
		    qDebug() <<  "Cannot open the file \"config.txt\""; 
		    exit(-1);
            }
	    
	    //Считываем сервисы чтобы проверить на наличие дубликатов
	    QStringList existingServices;
	    QString line;
	    QTextStream in(&configFile);
	    while(!in.atEnd()){
		    line = in.readLine().trimmed();		  
		    // Проверяем, что строка не пустая и начинается с   "Service: "
                    if (line.startsWith("Service: ", Qt::CaseInsensitive)) {
            	    	existingServices << line.mid(9).trimmed();   
              	    }
            }
	
	    // Проверяем на наличие дубликатов
            if (existingServices.contains(name)) {
            	qDebug() <<  "Service with name: " << name << " already exist";
                configFile.close();
		exit(-1);
            }
	    configFile.close();

 	    //  Открываем файл чтобы записать туда информацию
	    //  если файл не открылся выводим информацию об ошибке 
            if (!configFile.open(QIODevice::Append | QIODevice::Text)){
		    qDebug() <<  "Cannot open the file \"config.txt\""; 
                    configFile.close();
		    exit(-1);
            }

	    //Записываем информацию о сервисе
	    QTextStream out(&configFile);
	    out << "Service: "<< name <<"\n";
	    out << "Formats: ";
	    out << supportedFormats.join(", ");
	    out << "\n";
		
           configFile.close();
   }
void SharingService:: OpenFile(string p){
	  //приведение типов 
	  QString path = QString::fromStdString(p);
	  // Получаем информациу о файле
	  QFileInfo fileInf(path);

	  //Проверяем на существование файла
	  if(!fileInf.exists()){
		  qDebug() <<  "File does not exist";
		  exit(-1);
          }

	  QString formatFile = fileInf.suffix(); // Получаем  формат файлa
	 
	  QFile configFile("config.txt");
          //  Открываем файл чтобы считать  отуда информацию
	  //  если файл не открылся выводим информацию об ошибке 
	  if (!configFile.open(QIODevice::ReadOnly | QIODevice::Text)){
              qDebug() << "Cannot open the file \"config.txt\""; 
              exit(-1);
          }
	  
	  //Проверяем содержит ли данный сервис нужный формат
	   QString service;
	   bool isFormat = false;
	   QTextStream in(&configFile);
	   QString line;

	   //Считываем данные из файла и проверяем есть ли у сервиса нужное расшиерние файла
	    while(!in.atEnd()){
		    QStringList formats;
		    line = in.readLine();		    
		    // Считываем сервис
                    if (line.startsWith("Service: ", Qt::CaseInsensitive)) {
		        service =  line.mid(9).trimmed(); 
		    }
		    // Считываем форматы данного сервиса
                    if (line.startsWith("Formats: ", Qt::CaseInsensitive)) {
			formats << line.mid(9).trimmed().split(", "); 
		    }

		    // если нужное расширение есть добовляем его в список
		    if(formats.contains(formatFile)){
			    isFormat = true;
			    break;
	            }

            }

    	   configFile.close();

	   //Проверяем нашли ли мы сервер для открытия файла
	   if(!isFormat){
		   qDebug() << "No D-Bus services available to open the file";
		   exit(-1);
           }
	   //Вывод найденого сервиса
	   qDebug() << service << "can open " << formatFile;

	   std::string service_s = service.toStdString();
	   std::string path_s = path.toStdString();
	   //Открываем файл с помощью найденного сервиса
	   OpenFileUsingService(path_s, service_s);
   }

void SharingService::OpenFileUsingService(string p,string serv){	
	   QString path = QString::fromStdString(p);
	   QString service = QString::fromStdString(serv);
           //Подклюячаемся к сервису
	   QDBusInterface iface(service,"/", service, QDBusConnection::sessionBus());
	   if(!iface.isValid()){
		   qDebug() << "Invalid interface: "  << iface.lastError().message();
		   exit(-1);
           }

	   //Предпологаем что у сервиса есть функция  RunFile(name: string)
	   //Которая запускает файл 	

	   // Отправляем D-Bus сообщение
           QDBusMessage reply = iface.call("RunFile",path);
	   // Проверяем на наличие ошибок
   	   if (reply.type() == QDBusMessage::ErrorMessage) {
                qDebug() << "Failed to open file:" << reply.errorMessage();
		exit(-1);
           } else {
        	qDebug() << "The file is open by" << service;
           }
}

