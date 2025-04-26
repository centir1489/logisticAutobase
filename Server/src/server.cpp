#include"server.h"

Server::Server(QObject *parent) : QTcpServer(parent){
    qInfo() << "Инициализация сервера...";
    if(!db.openConnection()){
        qCritical() << "ОШИБКА: Не удалось подключиться к БД";
        exit(EXIT_FAILURE);
    }
}

Server::~Server(){
    qInfo() << "Сервер завершает работу";
    db.closeConnection();
}

void Server::startServer(){
    qInfo() << "Попытка запуска сервера на порту 12345";
    if(!this -> listen(QHostAddress::Any, 12345)){
        qCritical() << "ОШИБКА: Не удалось запустить сервер -" << this -> errorString();
        exit(EXIT_FAILURE);
    }
    qInfo() << "Сервер запущен";
}

void Server::incomingConnection(qintptr socketDescriptor){
    QTcpSocket *socket = new QTcpSocket(this);
    if(!socket -> setSocketDescriptor(socketDescriptor)){
        qWarning() << "ОШИБКА: Неверный дескриптор сокета -" << socket->errorString();
        delete socket;
        return;
    }
    qDebug() << "Новое подключение:" << socket->peerAddress().toString();

    connect(socket, &QTcpSocket::readyRead, this, &Server::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &Server::onDisconnected);
    clientBuffers[socket] = QByteArray();
}

void Server::onReadyRead(){
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if(!socket){
        return;
    }

    clientBuffers[socket].append(socket -> readAll());

    while(clientBuffers[socket].contains('\n')){
        QByteArray message = clientBuffers[socket].left(clientBuffers[socket].indexOf('\n')).trimmed();
        clientBuffers[socket].remove(0, clientBuffers[socket].indexOf('\n') + 1);

        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(message, &parseError);

        if(parseError.error != QJsonParseError::NoError){
            qWarning() << "Ошибка: Неверный JSON -" << parseError.errorString();
            sendErrorResponse(socket, "Неверный JSON формат");
            continue;
        }

        if(doc.isObject()){
            QJsonObject logObj = doc.object();
            if(logObj.contains("operation") && (logObj["operation"].toString() == "login" ||logObj["operation"].toString() == "register")){
                QJsonObject safeLogObj = logObj;
                if(safeLogObj["data"].isObject()){
                    QJsonObject dataObj = safeLogObj["data"].toObject();
                    if(dataObj.contains("password")){
                        dataObj["password"] = "*****";
                        safeLogObj["data"] = dataObj;
                    }
                    if(dataObj.contains("login")){
                        dataObj["login"] = "*****";
                        safeLogObj["data"] = dataObj;
                    }
                }
                qDebug() << "Получен запрос:" << QJsonDocument(safeLogObj).toJson(QJsonDocument::Compact);
            }
            else{
                qDebug() << "Получен запрос:" << doc.toJson(QJsonDocument::Compact);
            }
        }
        processRequest(socket, doc.object());
    }
}

void Server::processRequest(QTcpSocket* clientSocket, const QJsonObject& request) {
    if(!request.contains("operation")){
        qWarning() << "Запрос без операции от" << clientSocket -> peerAddress().toString();
        sendErrorResponse(clientSocket, "Не указана операция");
        return;
    }

    QString operationStr = request["operation"].toString();
    qDebug() << "Обработка операции:" << operationStr << "от" << clientSocket->peerAddress().toString();

    QJsonObject data = request.value("data").toObject();
    QJsonObject response;
    response["operation"] = operationStr;

    Operation operation = defineOperation(operationStr);
    Error error = processOperation(operation, data, response);

    if(error != Error::None) {
        if(!response.contains("status")){
            response["status"] = "error";
            qWarning() << "Ошибка при обработке" << operationStr << ":" << (response.contains("message") ? response["message"].toString() : "Неизвестная ошибка");
        }
    } else {
        qDebug() << "Операция" << operationStr << "успешно выполнена";
    }

    sendResponse(clientSocket, response);
}

void Server::onDisconnected(){
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if(socket){
        qDebug() << "Отключение:" << socket->peerAddress().toString();
        clientBuffers.remove(socket);
        socket -> deleteLater();
    }
}

void Server::sendResponse(QTcpSocket *clientSocket, const QJsonObject &response){
    if (!clientSocket || clientSocket->state() != QTcpSocket::ConnectedState) {
        qWarning() << "Попытка отправить ответ отключенному клиенту";
        return;
    }

    QByteArray responseData = QJsonDocument(response).toJson(QJsonDocument::Compact) + "\n";
    std::cout << "Отправка ответа: " << responseData.constData() << '\n';

    clientSocket->write(responseData);
    clientSocket->flush();
}

void Server::sendErrorResponse(QTcpSocket *clientSocket, const QString &message){
    QJsonObject response;
    response["status"] = "error";
    response["message"] = message;
    sendResponse(clientSocket, response);
}



/*  Функции для запросов к postgre    */

// authoriz
Server::Error Server::opCheckUser(const QJsonObject& data, QJsonObject& response) {
    if(!data.contains("username")) {
        response["message"] = "Отсутствует поле username";
        return Error::UnknownOperation;
    }

    QString username = data["username"].toString();
    bool exists = db.userExists(username);

    response["status"] = "success";
    response["exists"] = exists;
    return Error::None;
}

Server::Error Server::opLogin(const QJsonObject& data, QJsonObject& response){
    if(!data.contains("username") || !data.contains("password")){
        response["message"] = "Не указаны логин или пароль";
        return Error::UnknownOperation;
    }

    QString username = data["username"].toString();
    QString password = data["password"].toString();

    if(!db.openConnection()){
        response["message"] = "Ошибка подключения к базе данных";
        return Error::DatabaseError;
    }

    if(db.authenticUser(username, password)){
        int user_id = db.getuser_id(username);
        if(user_id <= 0) {
            response["status"] = "error";
            response["message"] = "Не удалось получить ID пользователя";
            return Error::DatabaseError;
        }

        response["status"] = "success";
        response["message"] = "Авторизация успешна";
        response["user_id"] = user_id;
        return Error::None;
    }

    response["status"] = "error";
    response["message"] = "Неверный пароль";
    return Error::InvalidCredentials;
}


Server::Error Server::opRegister(const QJsonObject& data, QJsonObject& response){
    if(!data.contains("username") || !data.contains("password")){
        response["message"] = "Не указаны логин или пароль";
        return Error::UnknownOperation;
    }

    QString username = data["username"].toString();
    QString passwd = data["password"].toString();

    if(!db.openConnection()){
        response["message"] = "Ошибка подключения к базе данных";
        return Error::UnknownOperation;
    }

    if(db.userExists(username)){
        response["status"] = "error";
        response["message"] = "Пользователь уже существует";
        return Error::UserExists;
    }

    if(!db.isCorrectPassword(passwd)){
        response["status"] = "error";
        response["message"] = "Некорректный пароль (должен быть 5-16 символов)";
        return Error::InvalidPassword;
    }

    if(db.addRegUser(username, passwd)){
        int user_id = db.getuser_id(username);
        if(user_id <= 0){
            response["status"] = "error";
            response["message"] = "Не удалось получить ID пользователя";
            return Error::DatabaseError;
        }
        response["status"] = "success";
        response["user_id"] = user_id;
        response["message"] = "Регистрация успешна";
        return Error::None;
    }

    response["status"] = "error";
    response["message"] = "Ошибка при регистрации пользователя";
    return Error::RegistrationError;
}

Server::Error Server::opGetUserId(const QJsonObject& data, QJsonObject& response){
    if(!data.contains("username")) {
        response["message"] = "Отсутствует поле username";
        return Error::UnknownOperation;
    }

    QString username = data["username"].toString();

    int user_id = db.getuser_id(username);
    if(user_id == -1){
        response["status"] = "error";
        response["message"] = "Пользователь не найден";
        return Error::InvalidCredentials;
    }

    response["status"] = "success";
    response["user_id"] = user_id;
    return Error::None;
}




/* Приложение таблица USER_TABLES */
Server::Error Server::opGetUserTables(const QJsonObject& data, QJsonObject& response) {
    if(!data.contains("user_id")){
        response["message"] = "Отсутствует поле user_id";
        return Error::UnknownOperation;
    }

    int user_id = data["user_id"].toInt();
    auto tables = db.getUserTbls(user_id);

    QJsonArray arrTransportations;
    for(const auto& table : tables) {
        auto tableInfo = db.getTransportation(table.first);
        if(tableInfo.contains("error")) {
            continue;
        }

        QJsonObject jsTransportation;
        jsTransportation["table_id"] = table.first;
        jsTransportation["table_name"] = table.second;
        jsTransportation["departure_location"] = tableInfo["departure_location"].toString();
        jsTransportation["destination_location"] = tableInfo["destination_location"].toString();
        arrTransportations.append(jsTransportation);
    }

    response["status"] = "success";
    response["tables"] = arrTransportations;
    return Error::None;
}

Server::Error Server::opOpenTable(const QJsonObject& data, QJsonObject& response){
    if(!data.contains("table_id")) {
        response["message"] = "Отсутствуют необходимые поле table_id";
        return Error::UnknownOperation;
    }

    int table_id = data["table_id"].toInt();

    QVector<QMap<QString, QVariant>> tableData = db.getTableParcels(table_id);

    QJsonArray parcels;
    for(const auto& row : tableData){
        QJsonObject parcel;

        parcel["number_parcel"] = row["number_parcel"].toString();
        parcel["sender"] = row["sender"].toString();
        parcel["recipient"] = row["recipient"].toString();
        parcel["date_dispatch"] = row["date_dispatch"].toDate().toString("dd-MM-yyyy");
        parcel["delivery_date"] = row["delivery_date"].toDate().toString("dd-MM-yyyy");
        parcel["status_parcel"] = row["status_parcel"].toString();

        if(row["weight"].canConvert<double>()) {
            parcel["weight"] = row["weight"].toDouble();
        }
        else{ // при проблемах преобразвоания
            bool corectConvert;
            double weight = row["weight"].toString().toDouble(&corectConvert);
            parcel["weight"] = corectConvert ? weight : 0.0;
        }

        parcels.append(parcel);
    }

    response["status"] = "success";
    response["parcels"] = parcels;
    return Error::None;
}



Server::Error Server::opViewTable(const QJsonObject& data, QJsonObject& response){
    if(!data.contains("table_id")){
        response["message"] = "Отсутствуют необходимые поля table_id";
        return Error::InvalidData;
    }

    int table_id = data["table_id"].toInt();
    QVector<QMap<QString, QVariant>> parcels = db.getTableParcels(table_id);

    QJsonArray jsParcel;
    for(const auto& parcel : parcels){
        QJsonObject rowObj;
        for(auto iter = parcel.begin(); iter != parcel.end(); ++iter) {
            rowObj[iter.key()] = QJsonValue::fromVariant(iter.value());
        }
        jsParcel.append(rowObj);
    }

    response["status"] = "success";
    response["parcels"] = jsParcel;
    return Error::None;
}

Server::Error Server::opCreateTable(const QJsonObject& data, QJsonObject& response){
    if(!data.contains("user_id") || !data.contains("table_name")) {
        response["message"] = "Отсутствуют необходимые поля (user_id или table_name)";
        return Error::DatabaseError;
    }

    int user_id = data["user_id"].toInt();
    QString table_name = data["table_name"].toString();
    QString destination_location = data["destination_location"].toString();
    QString departure_location = data["departure_location"].toString();

    if(db.tblExistsUser(user_id, table_name)){
        response["status"] = "error";
        response["message"] = "Перевозка с таким именем уже существует";
        return Error::UserExists;
    }

    if(db.AddTable(user_id, table_name, departure_location, destination_location)){
        response["status"] = "success";
        response["message"] = "Посылка успешно создана";
        return Error::None;
    }


    response["status"] = "error";
    response["message"] = "Ошибка при создании таблицы";
    return Error::DatabaseError;
}

Server::Error Server::opGetTransportation(const QJsonObject& data, QJsonObject& response){
    if(!data.contains("table_id")){
        response["message"] = "Отсутствует table_id";
        return Error::InvalidData;
    }
    int table_id = data["table_id"].toInt();
    QMap<QString, QVariant> transportData = db.getTransportation(table_id);
    if(transportData.contains("error")){
        response["message"] = transportData["error"].toString();
        return Error::DatabaseError;
    }
    response["table_id"] = transportData["table_id"].toString();
    response["table_name"] = transportData["table_name"].toString();
    response["departure_location"] = transportData["departure_location"].toString();
    response["destination_location"] = transportData["destination_location"].toString();

    response["status"] = "success";
    return Error::None;
}

Server::Error Server::opRenameTable(const QJsonObject& data, QJsonObject& response){
    if(!data.contains("table_id") || !data.contains("new_name")){
        response["message"] = "Отсутствуют необходимые поля (table_id или new_name)";
        return Error::InvalidData;
    }

    int table_id = data["table_id"].toInt();
    int user_id = data["user_id"].toInt();
    QString newNameTbl = data["new_name"].toString();


    if(db.renameTable(user_id, table_id, newNameTbl)){
        response["status"] = "success";
        response["message"] = "Таблица успешно переименована";
        return Error::None;
    }


    response["status"] = "error";
    response["message"] = "Ошибка при переименовании таблицы";
    return Error::DatabaseError;
}


Server::Error Server::opEditTransportation(const QJsonObject& data, QJsonObject& response){
    if(!data.contains("table_id") || !data.contains("user_id")){
        response["message"] = "Отсутствуют необходимое поле table_id";
        return Error::InvalidData;
    }

    int table_id = data["table_id"].toInt();
    int user_id = data["user_id"].toInt();
    QString destination_location = data["destination_location"].toString();
    QString departure_location = data["departure_location"].toString();

    if(db.updateTable(user_id, table_id, destination_location, departure_location)){
        response["status"] = "success";
        response["message"] = "Перевозка успешно обновлена";
        return Error::None;
    }
    response["status"] = "error";
    response["message"] = "Ошибка при обновлении перевозки";
    return Error::DatabaseError;
}

Server::Error Server::opDeleteTable(const QJsonObject& data, QJsonObject& response){
    if(!data.contains("table_id") || !data.contains("user_id")){
        response["message"] = "Отсутствует поле table_id или user_id";
        return Error::InvalidData;
    }

    int table_id = data["table_id"].toInt();
    int user_id = data["user_id"].toInt();

    if(db.delTableNParcels(user_id, table_id)){
        response["status"] = "success";
        response["message"] = "Перевозка и посылки этой перевозки успешно удалены";
        return Error::None;
    }

    response["status"] = "error";
    response["message"] = "Ошибка при удалении таблицы";
    return Error::DatabaseError;
}

Server::Error Server::opImportTable(const QJsonObject& data, QJsonObject& response) {
    if (!data.contains("user_id") || !data.contains("file_path") ||
        !data.contains("table_name") || !data.contains("departure_location") ||
        !data.contains("destination_location")) {
        response["message"] = "Недостаточно данных для импорта";
        return Error::InvalidData;
    }

    int userId = data["user_id"].toInt();
    QString filePath = data["file_path"].toString();
    QString tableName = data["table_name"].toString().trimmed();
    QString departure = data["departure_location"].toString().trimmed();
    QString destination = data["destination_location"].toString().trimmed();

    if (tableName.isEmpty()) {
        response["message"] = "Имя таблицы не может быть пустым";
        return Error::InvalidData;
    }

    if (departure.isEmpty() || destination.isEmpty()) {
        response["message"] = "Локации отправления и назначения не могут быть пустыми";
        return Error::InvalidData;
    }

    QVariantList importedParcels;
    if(db.importTablesCSV(userId, filePath, importedParcels, tableName, departure, destination)){
        response["parcels"] = QJsonArray::fromVariantList(importedParcels);
        return Error::None;
        response["message"] = "Ошибка при импорте таблицы";
        return Error::DatabaseError;
    }

    response["message"] = "Ошибка при импорте таблицы";
    return Error::DatabaseError;
}

Server::Error Server::opExportTable(const QJsonObject& data, QJsonObject& response) {
    if (!data.contains("table_id") || !data.contains("file_path")) {
        response["message"] = "Отсутствуют обязательные поля (table_id или file_path)";
        return Error::InvalidData;
    }

    int table_id = data["table_id"].toInt();
    QString filePath = data["file_path"].toString();

    QFile testFile(filePath);
    if (!testFile.open(QIODevice::WriteOnly)) {
        response["message"] = "Не удается открыть файл для записи: " + testFile.errorString();
        return Error::InvalidData;
    }
    testFile.close();

    if(db.exportTableCSV(table_id, filePath)){
        response["status"] = "success";
        response["message"] = "Таблица успешно экспортирована";
        response["file_path"] = filePath;
        return Error::None;
    }

    response["status"] = "error";
    response["message"] = "Ошибка при экспорте таблицы";
    return Error::DatabaseError;
}



/* Приложение таблица PARCELS */
Server::Error Server::opGetParcelData(const QJsonObject& data, QJsonObject& response){
    if(!data.contains("parcel_id")){
        response["message"] = "Отсутствует поле parcel_id";
        return Error::InvalidData;
    }

    int parcel_id = data["parcel_id"].toInt();
    auto parcelData = db.getParcelData(parcel_id);

    if(parcelData.isEmpty()){
        response["status"] = "error";
        response["message"] = "Посылка не найдена";
        return Error::InvalidData;
    }

    response["status"] = "success";
    for(auto iter = parcelData.constBegin(); iter != parcelData.constEnd(); ++iter){
        response[iter.key()] = QJsonValue::fromVariant(iter.value());
    }
    return Error::None;
}

bool checkCorrectParcel(const QJsonObject& data, QString& problemField){
    const QVector<QString> correctField = {"sender", "recipient", "date_dispatch", "delivery_date", "status_parcel", "weight"};
    for(const QString& attr : correctField){
        if(!data.contains(attr)){
            problemField = attr;
            return false;
        }
    }
    return true;
}

Server::Error Server::opUpdateParcel(const QJsonObject& data, QJsonObject& response){
    QString problemField;
    if(!checkCorrectParcel(data, problemField)){
        response["message"] = "Отсутствует данные в поле: " + problemField;
        return Error::InvalidData;
    }

    int parcel_id = db.getParcelId(data["number_parcel"].toString());
    if(parcel_id <= 0) {
        response["message"] = "Некорректный ID таблицы";
        return Error::InvalidData;
    }

    QMap<QString, QVariant> parcelData;
    parcelData["parcel_id"] = parcel_id;
    parcelData["number_parcel"] = data["number_parcel"].toString();
    parcelData["sender"] = data["sender"].toString();
    parcelData["recipient"] = data["recipient"].toString();
    parcelData["date_dispatch"] = data["date_dispatch"].toString();
    parcelData["delivery_date"] = data["delivery_date"].toString();
    parcelData["status_parcel"] = data["status_parcel"].toString();
    parcelData["weight"] = data["weight"].toDouble();

    if(db.updateParcel(parcelData)){
        response["status"] = "success";
        response["message"] = "Посылка успешно обновлена";
        return Error::None;
    }

    response["status"] = "error";
    response["message"] = "Ошибка при обновлении посылки";
    return Error::DatabaseError;
}

// удаление посылки по parcel_id
Server::Error Server::opDeleteParcel(const QJsonObject& data, QJsonObject& response) {
    if(!data.contains("number_parcel")){
        response["message"] = "Отсутствует поле number_parcel";
        return Error::InvalidData;
    }

    int parcel_id = db.getParcelId(data["number_parcel"].toString());
    if(parcel_id == -1){
        response["message"] = "Посылка не найдена";
        return Error::DatabaseError;
    }
    if(db.delParcel(parcel_id)){
        response["status"] = "success";
        response["message"] = "Посылка успешно удалена";
        return Error::None;
    }

    response["status"] = "error";
    response["message"] = "Ошибка при удалении посылки";
    return Error::DatabaseError;
}



Server::Error Server::opAddParcel(const QJsonObject& data, QJsonObject& response){
    QString problemField;
    if(!checkCorrectParcel(data, problemField)){
        response["message"] = "Отсутствует обязательный атрибут: " + problemField;
        return Error::InvalidData;
    }

    int table_id = data["table_id"].toInt();
    if(table_id <= 0) {
        response["message"] = "Некорректный ID таблицы";
        return Error::InvalidData;
    }

    QMap<QString, QVariant> parcelData;
    parcelData["number_parcel"] = data["number_parcel"].toString();
    parcelData["sender"] = data["sender"].toString();
    parcelData["recipient"] = data["recipient"].toString();
    parcelData["date_dispatch"] = QDate::fromString(data["date_dispatch"].toString(), Qt::ISODate);
    parcelData["delivery_date"] = QDate::fromString(data["delivery_date"].toString(), Qt::ISODate);
    parcelData["status_parcel"] = data["status_parcel"].toString();
    parcelData["weight"] = data["weight"].toDouble();
    parcelData["table_id"] = table_id;

    if(db.addParcel(parcelData)){
        response["status"] = "success";
        response["message"] = "Посылка успешно создана";
        return Error::None;
    }

    response["status"] = "error";
    response["message"] = "Ошибка при создании посылки";
    return Error::DatabaseError;
}

Server::Error Server::opMoveParcel(const QJsonObject& data, QJsonObject& response){
    if(!data.contains("number_parcel") || !data.contains("target_table") || !data.contains("user_id")) {
        response["message"] = "Отсутствуют необходимые поля (number_parcel, target_table или user_id)";
        return Error::InvalidData;
    }

    int parcel_id = db.getParcelId(data["number_parcel"].toString());
    if(parcel_id <= 0) {
        response["message"] = "Посылка не найдена";
        return Error::InvalidData;
    }

    int user_id = data["user_id"].toInt();
    if(user_id <= 0) {
        response["message"] = "Некорректный ID пользователя";
        return Error::InvalidData;
    }

    QString target_table = data["target_table"].toString();
    int new_table_id = db.gettable_id(target_table, user_id);
    if(new_table_id <= 0){
        response["message"] = "Целевая таблица/перевозка не найдена или вам не принадлежит";
        return Error::InvalidData;
    }

    if(db.moveParcel(parcel_id, new_table_id)) {
        response["status"] = "success";
        response["message"] = "Посылка успешно перемещена";
        return Error::None;
    }

    response["status"] = "error";
    response["message"] = "Ошибка при перемещении посылки";
    return Error::DatabaseError;
}


Server::Error Server::opGetAllParcels(const QJsonObject& data, QJsonObject& response){
    if(!data.contains("user_id")) {
        response["message"] = "Отсутствует поле user_id";
        return Error::InvalidData;
    }
    int user_id = data["user_id"].toInt();

    auto tables = db.getUserTbls(user_id);
    if(tables.isEmpty()) {
        response["status"] = "success";
        response["parcels"] = QJsonArray();
        return Error::None;
    }

    QJsonArray jsParcels;
    for(const auto& table : tables){
        int table_id = table.first;
        auto parcels = db.getTableParcels(table_id);
        for(const auto& parcel : parcels){
            QJsonObject parcelField;
            parcelField["parcel_id"] = parcel["parcel_id"].toInt();
            parcelField["number_parcel"] = parcel["number_parcel"].toString();
            parcelField["sender"] = parcel["sender"].toString();
            parcelField["recipient"] = parcel["recipient"].toString();
            parcelField["date_dispatch"] = parcel["date_dispatch"].toString();
            parcelField["delivery_date"] = parcel["delivery_date"].toString();
            parcelField["status_parcel"] = parcel["status_parcel"].toString();
            parcelField["weight"] = parcel["weight"].toDouble();
            parcelField["table_id"] = table_id;

            jsParcels.append(parcelField);
        }
    }

    response["status"] = "success";
    response["parcels"] = jsParcels;
    return Error::None;
}


/* Поиск */
Server::Error Server::opSearchInTable(const QJsonObject& data, QJsonObject& response){
    if(!data.contains("table_id") || !data.contains("search_text")){
        response["message"] = "Отсутствуют необходимые поля (table_id или search_text)";
        return Error::InvalidData;
    }

    int table_id = data["table_id"].toInt();
    QString searchText = data["search_text"].toString();

    QVector<QMap<QString, QVariant>> parcels;
    if(!db.searchInTable(table_id, searchText, parcels)){
        response["message"] = "Ошибка при поиске в таблице";
        return Error::DatabaseError;
    }

    QJsonArray jsParcels;
    for (const auto& parcel : parcels) {
        QJsonObject parcelObj;
        parcelObj["parcel_id"] = parcel["parcel_id"].toInt();
        parcelObj["number_parcel"] = parcel["number_parcel"].toString();
        parcelObj["sender"] = parcel["sender"].toString();
        parcelObj["recipient"] = parcel["recipient"].toString();
        parcelObj["date_dispatch"] = parcel["date_dispatch"].toString();
        parcelObj["delivery_date"] = parcel["delivery_date"].toString();
        parcelObj["status_parcel"] = parcel["status_parcel"].toString();
        parcelObj["weight"] = parcel["weight"].toDouble();
        parcelObj["table_id"] = parcel["table_id"].toInt();

        jsParcels.append(parcelObj);
    }

    response["status"] = "success";
    response["parcels"] = jsParcels;
    return Error::None;
}

Server::Error Server::opSearchInAllParcels(const QJsonObject& data, QJsonObject& response){
    if (!data.contains("user_id") || !data.contains("search_text")) {
        response["message"] = "Отсутствуют необходимые поля (user_id или search_text)";
        return Error::InvalidData;
    }

    int user_id = data["user_id"].toInt();
    QString searchText = data["search_text"].toString();

    QVector<QMap<QString, QVariant>> parcels;
    if (!db.searchInAllParcels(user_id, searchText, parcels)) {
        response["message"] = "Ошибка при поиске во всех посылках";
        return Error::DatabaseError;
    }

    QJsonArray jsParcels;
    for (const auto& parcel : parcels) {
        QJsonObject parcelObj;
        parcelObj["parcel_id"] = parcel["parcel_id"].toInt();
        parcelObj["number_parcel"] = parcel["number_parcel"].toString();
        parcelObj["sender"] = parcel["sender"].toString();
        parcelObj["recipient"] = parcel["recipient"].toString();
        parcelObj["date_dispatch"] = parcel["date_dispatch"].toString();
        parcelObj["delivery_date"] = parcel["delivery_date"].toString();
        parcelObj["status_parcel"] = parcel["status_parcel"].toString();
        parcelObj["weight"] = parcel["weight"].toDouble();
        parcelObj["table_id"] = parcel["table_id"].toInt();

        jsParcels.append(parcelObj);
    }

    response["status"] = "success";
    response["parcels"] = jsParcels;
    return Error::None;
}


Server::Operation Server::defineOperation(const QString& operationStr){
    static const QHash<QString, Operation> operations = {
        {"checkUser", Operation::CheckUser},
        {"login", Operation::Login},
        {"register", Operation::Register},
        {"searchInTable", Operation::SearchInTable},
        {"searchInAllParcels", Operation::SearchInAllParcels},
        {"getUserTables", Operation::GetUserTables},
        {"getAllParcels", Operation::GetAllParcels},
        {"addParcel", Operation::AddParcel},
        {"getParcelData", Operation::GetParcelData},
        {"updateParcel", Operation::UpdateParcel},
        {"deleteParcel", Operation::DeleteParcel},
        {"moveParcel", Operation::MoveParcel},
        {"openTable", Operation::OpenTable},
        {"renameTable", Operation::RenameTable},
        {"viewTable", Operation::ViewTable},
        {"getTransportion", Operation::GetTransportion},
        {"createTable", Operation::CreateTable},
        {"editTable", Operation::EditTable},
        {"deleteTable", Operation::DeleteTable},
        {"exportTable", Operation::ExportTable},
        {"importTable", Operation::ImportTable}
    };
    return operations.value(operationStr, Operation::Unknown);
}

Server::Error Server::processOperation(Operation operation, const QJsonObject& data, QJsonObject& response) {
    switch(operation){
        case Operation::CheckUser:{
            return opCheckUser(data, response);
        }
        case Operation::Login:{
            return opLogin(data, response);
        }
        case Operation::Register:{
            return opRegister(data, response);
        }
        case Operation::SearchInTable:{
            return opSearchInTable(data, response);
        }
        case Operation::SearchInAllParcels:{
            return opSearchInAllParcels(data, response);
        }
        case Operation::GetUserId:{
            return opGetUserId(data, response);
        }
        case Operation::GetAllParcels:{
            return opGetAllParcels(data, response);
        }
        case Operation::GetUserTables:{
            return opGetUserTables(data, response);
        }
        case Operation::CreateTable:{
            return opCreateTable(data, response);
        }
        case Operation::RenameTable:{
            return opRenameTable(data, response);
        }
        case Operation::EditTable:{
            return opEditTransportation(data, response);
        }
        case Operation::DeleteTable:{
            return opDeleteTable(data, response);
        }
        case Operation::ExportTable:{
            return opExportTable(data, response);
        }
        case Operation::ImportTable:{
            return opImportTable(data, response);
        }
        case Operation::OpenTable:{
            return opOpenTable(data, response);
        }
        case Operation::GetTransportion:{
            return opGetTransportation(data, response);
        }
        case Operation::ViewTable:{
            return opViewTable(data, response);
        }

        case Operation::GetParcelData:{
            return opGetParcelData(data, response);
        }
        case Operation::AddParcel:{
            return opAddParcel(data, response);
        }
        case Operation::UpdateParcel:{
            return opUpdateParcel(data, response);
        }
        case Operation::DeleteParcel:{
            return opDeleteParcel(data, response);
        }
        case Operation::MoveParcel:{
            return opMoveParcel(data, response);
        }

        default:{
            response["message"] = "Неизвестная операция";
            return Error::UnknownOperation;
        }
    }
}
