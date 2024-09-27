# SharingService

# README.md

# Динамическая библиотека для создания DBus-сервиса

Эта динамическая библиотека предоставляет средства для создания DBus-сервиса, который открывает указанный файл. Для компиляции и установки библиотеки вам потребуется использовать `qmake` и `make`. 

## Установка

1. Клонируйте репозиторий или скачайте исходный код библиотеки:

   ```bash
   git clone https://github.com/LilyMephi/SharingService.git
   cd SharingService
   ```
   

2. Создайте проект с помощью `qmake`:

  ```bush
   qmake MyLib.pro`
   ```

3. Соберите библиотеку:

  ```bash
   make
   ```
   

4. Перед запуском приложения, экспортируйте переменную окружения `LD_LIBRARY_PATH`, указывая путь к вашей библиотеке:

   ```bash
   export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/path/to/lib
   ```
   

   Замените `/path/to/lib` на фактический путь к директории, где находится ваша динамическая библиотека.

## Использование

После того как вы собрали библиотеку и установили переменную окружения, вы можете использовать ее для создания DBus-сервиса. Подробности о том, как это сделать, можно найти в документации к библиотеке или в примерах, приведенных в репозитории.

## Примеры

Библиотека содержит несколько примеров использования. Чтобы их запустить, ознакомьтесь с соответствующими файлами:
   ```bash
    git clone https://github.com/LilyMephi/DBusSharing.git
    cd DBusSharing
    qmake project.pro
    make
    qmake client.pro
    make
   ```
в разных терминалах запустите:
   ```bash
    ./service
    ./client /path/to/file
   ```

## Зависимости

Для компиляции и работы библиотеки вам потребуются следующие зависимости:

- Qt (необходимые модули для работы с DBus)
- dbus

Убедитесь, что эти зависимости установлены на вашей системе.

---
###Использование библиотеки 
```c++
   SharingService(name_serviec:string, supportedFormats: vector<string>,openFunction:function<void(const string path)>)`
   start(path:string)
```
Название интерфейса `com.sharing.service`

