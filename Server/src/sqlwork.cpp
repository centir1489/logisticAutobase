#include"sqlwork.h"

Database::Database() {
    if(!QSqlDatabase::isDriverAvailable("QPSQL")) {
        qDebug() << "Драйвер PostgreSQL не доступен!";
        return;
    }

    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("postgres");
    db.setDatabaseName("logisticAutobase");
    db.setUserName("autobase_user");
    db.setPassword("ABc324gR!0");
    db.setPort(5432);

    db.setConnectOptions("requiressl=0;connect_timeout=5");
}

Database::~Database() {
    closeConnection();
}

bool Database::openConnection() {
    if(!db.isValid()) {
        qDebug() << "Неверные параметры подключения или отсутствует драйвер";
        return false;
    }

    if(!db.open()) {
        qDebug() << "Ошибка подключения к БД:" << db.lastError().text();
        return false;
    }

    qDebug() << "Успешное подключение к PostgreSQL!";
    return true;
}

void Database::closeConnection() {
    if(db.isOpen()) {
        db.close();
    }
    QSqlDatabase::removeDatabase(db.connectionName());
}

bool Database::checkConnection(){
    if(!db.isOpen()){
        return openConnection();
    }
    return true;
}

bool Database::userExists(const QString& username) {
    if(!checkConnection()){
        return false;
    }

    QSqlQuery query(db);
    query.prepare("SELECT 1 FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if(!query.exec()){
        qCritical() << "Ошибка проверки пользователя:" << query.lastError().text();
        return false;
    }

    return query.next();
}

bool Database::authenticUser(const QString& username, const QString& passwd) {
    if(!checkConnection()){
        return false;
    }

    QSqlQuery query(db);
    query.prepare("SELECT passwd FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if(!query.exec()){
        qCritical() << "Ошибка аутентификации пользователя: " << query.lastError().text();
        return false;
    }

    if(query.next()){
        QString storedHash = query.value(0).toString();
        return storedHash == hashPasswd(passwd);
    }
    return false;
}

bool Database::addRegUser(const QString& username, const QString& passwd) {
    if(!checkConnection()){
        return false;
    }

    db.transaction();

    QSqlQuery query(db);
    query.prepare("INSERT INTO users (username, passwd) VALUES (:username, :passwd)");
    query.bindValue(":username", username);
    query.bindValue(":passwd", hashPasswd(passwd));

    if(!query.exec()){
        qCritical() << "Ошибка регистрации пользователя: " << query.lastError().text();
        db.rollback();
        return false;
    }

    db.commit();
    return true;
}

int Database::getuser_id(const QString& username) {
    if(!db.isOpen() && !openConnection()){
        qDebug() << "Не удалось открыть БД!";
        return -1;
    }

    QSqlQuery query(db);
    query.prepare("SELECT user_id FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if(!query.exec()) {
        qDebug() << "Неправильный запрос: " << query.lastError().text();
        return -1;
    }
    if(query.next()) {
        return query.value(0).toInt();
    }
    else {
        qDebug() << "Нет пользователя с данным именем";
        return -1;
    }
}

QString Database::hashPasswd(const QString& passwd) {
    return QString(QCryptographicHash::hash(passwd.toUtf8(), QCryptographicHash::Sha256).toHex());
}

bool Database::isCorrectPassword(const QString& passwd) {
    return passwd.length() >= 5 && passwd.length() <= 16;
}

int Database::getParcelId(const QString &number_parcel) {
    int parcel_id = -1;
    QSqlQuery query(db);
    query.prepare("SELECT parcel_id FROM parcels WHERE number_parcel = :number_parcel");
    query.bindValue(":number_parcel", number_parcel);

    if(query.exec() && query.next()) {
        parcel_id = query.value(0).toInt();
    }
    else {
        qDebug() << "Не удалось найти parcel_id по номеру перевозки: " << query.lastError().text();
    }
    return parcel_id;
}

QVector<QMap<QString, QVariant>> Database::getTableParcels(int table_id) {
    QVector<QMap<QString, QVariant>> parcels;
    QSqlQuery query(db);
    query.prepare("SELECT * FROM parcels WHERE table_id = :table_id");
    query.bindValue(":table_id", table_id);

    if(!query.exec()){
        qDebug() << "Ошибка получения посылок: " << query.lastError().text();
        return parcels;
    }

    while(query.next()){
        QMap<QString, QVariant> parcel;
        parcel["parcel_id"] = query.value("parcel_id");
        parcel["number_parcel"] = query.value("number_parcel");
        parcel["sender"] = query.value("sender");
        parcel["recipient"] = query.value("recipient");
        parcel["date_dispatch"] = query.value("date_dispatch");
        parcel["delivery_date"] = query.value("delivery_date");
        parcel["status_parcel"] = query.value("status_parcel");
        parcel["weight"] = query.value("weight");
        parcel["table_id"] = query.value("table_id");
        parcels.append(parcel);
    }
    return parcels;
}



QMap<QString, QVariant> Database::getParcelData(int parcel_id){
    QMap<QString, QVariant> parcel;
    QSqlQuery query(db);
    query.prepare("SELECT * FROM parcels WHERE parcel_id = :parcel_id");
    query.bindValue(":parcel_id", parcel_id);

    if(query.exec() && query.next()){
        parcel["parcel_id"] = query.value("parcel_id");
        parcel["number_parcel"] = query.value("number_parcel");
        parcel["sender"] = query.value("sender");
        parcel["recipient"] = query.value("recipient");
        parcel["date_dispatch"] = query.value("date_dispatch");
        parcel["delivery_date"] = query.value("delivery_date");
        parcel["status_parcel"] = query.value("status_parcel");
        parcel["weight"] = query.value("weight");
        parcel["table_id"] = query.value("table_id");
    }
    else{
        qDebug() << "Не удалось получить данные: " << query.lastError().text();
    }
    return parcel;
}

bool Database::searchInTable(int table_id, const QString& searchText, QVector<QMap<QString, QVariant>>& result) {
    QSqlQuery query(db);

    query.prepare(
        "SELECT * FROM parcels "
        "WHERE table_id = :table_id AND "
        "(number_parcel ILIKE :search OR "
        "sender ILIKE :search OR "
        "recipient ILIKE :search OR "
        "status_parcel ILIKE :search OR "
        "CAST(weight AS TEXT) ILIKE :search) "
        "ORDER BY number_parcel"
        );

    query.bindValue(":table_id", table_id);
    query.bindValue(":search", "%" + searchText + "%");

    if(!query.exec()){
        qDebug() << "Ошибка поиска в таблице: " << query.lastError().text();
        return false;
    }

    return fillParcelResults(query, result);
}

bool Database::searchInAllParcels(int user_id, const QString& searchText, QVector<QMap<QString, QVariant>>& result){
    QSqlQuery query(db);

    query.prepare(
        "SELECT p.* FROM parcels p "
        "JOIN user_tables ut ON p.table_id = ut.table_id "
        "WHERE ut.user_id = :user_id AND "
        "(p.number_parcel ILIKE :search OR "
        "p.sender ILIKE :search OR "
        "p.recipient ILIKE :search OR "
        "p.status_parcel ILIKE :search OR "
        "CAST(p.weight AS TEXT) ILIKE :search) "
        "ORDER BY p.number_parcel"
        );

    query.bindValue(":user_id", user_id);
    query.bindValue(":search", "%" + searchText + "%");

    if(!query.exec()){
        qDebug() << "Ошибка поиска во всех посылках: " << query.lastError().text();
        return false;
    }

    return fillParcelResults(query, result);
}

bool Database::fillParcelResults(QSqlQuery& query, QVector<QMap<QString, QVariant>>& result) {
    result.clear();

    while(query.next()){
        QMap<QString, QVariant> parcel;
        parcel["parcel_id"] = query.value("parcel_id");
        parcel["number_parcel"] = query.value("number_parcel");
        parcel["sender"] = query.value("sender");
        parcel["recipient"] = query.value("recipient");
        parcel["date_dispatch"] = query.value("date_dispatch");
        parcel["delivery_date"] = query.value("delivery_date");
        parcel["status_parcel"] = query.value("status_parcel");
        parcel["weight"] = query.value("weight");
        parcel["table_id"] = query.value("table_id");
        result.append(parcel);
    }
    return true;
}


bool Database::addParcel(const QMap<QString, QVariant>& parcel) {
    if(!checkCsvData(parcel)){
        return false;
    }

    QSqlQuery query(db);
    query.prepare("INSERT INTO parcels (number_parcel, sender, recipient, "
                  "date_dispatch, delivery_date, status_parcel, weight, table_id) "
                  "VALUES (:number_parcel, :sender, :recipient, :date_dispatch, "
                  ":delivery_date, :status_parcel, :weight, :table_id)");

    query.bindValue(":number_parcel", parcel["number_parcel"]);
    query.bindValue(":sender", parcel["sender"]);
    query.bindValue(":recipient", parcel["recipient"]);
    query.bindValue(":date_dispatch", parcel["date_dispatch"]);
    query.bindValue(":delivery_date", parcel["delivery_date"]);
    query.bindValue(":status_parcel", parcel["status_parcel"]);
    query.bindValue(":weight", parcel["weight"]);
    query.bindValue(":table_id", parcel["table_id"]);

    if(!query.exec()){
        qDebug() << "Ошибка добавления посылки: " << query.lastError().text();
        return false;
    }
    return true;
}

bool Database::updateParcel(const QMap<QString, QVariant>& parcel){
    QSqlQuery query(db);
    query.prepare("UPDATE parcels SET "
                  "number_parcel = :number_parcel, "
                  "sender = :sender, "
                  "recipient = :recipient, "
                  "date_dispatch = :date_dispatch, "
                  "delivery_date = :delivery_date, "
                  "status_parcel = :status_parcel, "
                  "weight = :weight "
                  "WHERE parcel_id = :parcel_id");

    query.bindValue(":number_parcel", parcel["number_parcel"]);
    query.bindValue(":sender", parcel["sender"]);
    query.bindValue(":recipient", parcel["recipient"]);
    query.bindValue(":date_dispatch", parcel["date_dispatch"]);
    query.bindValue(":delivery_date", parcel["delivery_date"]);
    query.bindValue(":status_parcel", parcel["status_parcel"]);
    query.bindValue(":weight", parcel["weight"]);
    query.bindValue(":parcel_id", parcel["parcel_id"]);

    if(!query.exec()){
        qDebug() << "Не удалось обновить: " << query.lastError().text();
        return false;
    }

    return true;
}

bool Database::delParcel(int parcel_id) {
    QSqlQuery query(db);
    query.prepare("DELETE FROM parcels WHERE parcel_id = :parcel_id");
    query.bindValue(":parcel_id", parcel_id);

    if(!query.exec()) {
        qDebug() << "Не удалось удалить посылку: " << query.lastError().text();
        return false;
    }
    return true;
}

bool Database::moveParcel(int parcel_id, int new_table_id) {
    if(!db.isOpen() && !openConnection()){
        return false;
    }

    QSqlQuery query(db);
    query.prepare("SELECT 1 FROM user_tables WHERE table_id = :table_id");
    query.bindValue(":table_id", new_table_id);

    if(!query.exec() || !query.next()){
        qDebug() << "Целевая таблица не существует или ошибка запроса: " << query.lastError().text();
        return false;
    }

    QMap<QString, QVariant> parcelData = getParcelData(parcel_id);
    if(parcelData.isEmpty()) {
        qDebug() << "Посылка с ID" << parcel_id << "не найдена";
        return false;
    }

    QSqlQuery updateQuery(db);
    updateQuery.prepare("UPDATE parcels SET table_id = :new_table_id WHERE parcel_id = :parcel_id");
    updateQuery.bindValue(":new_table_id", new_table_id);
    updateQuery.bindValue(":parcel_id", parcel_id);

    if(!updateQuery.exec()){
        qDebug() << "Ошибка перемещения посылки: " << updateQuery.lastError().text();
        return false;
    }
    return updateQuery.numRowsAffected() > 0;
}

QMap<QString, QVariant> Database::getTransportation(int table_id){
    QMap<QString, QVariant> result;
    if(!db.isOpen() && !openConnection()){
        result["error"] = "Не удалось подключиться к базе данных";
        return result;
    }
    QSqlQuery query(db);
    query.prepare("SELECT table_name, departure_location, destination_location FROM user_tables WHERE table_id = :table_id");
    query.bindValue(":table_id", table_id);

    if(!query.exec()){
        result["error"] = "Ошибка запроса: " + query.lastError().text();
        return result;
    }
    if(!query.next()){
        result["error"] = "Таблица с ID " + QString::number(table_id) + " не найдена";
        return result;
    }

    result["table_id"] = table_id;
    result["table_name"] = query.value("table_name");
    result["departure_location"] = query.value("departure_location");
    result["destination_location"] = query.value("destination_location");
    return result;
}


int Database::gettable_id(const QString& nameTable, int user_id){
    QSqlQuery query(db);
    query.prepare("SELECT table_id FROM user_tables WHERE table_name = :nameTable AND user_id = :user_id");
    query.bindValue(":nameTable", nameTable);
    query.bindValue(":user_id", user_id);

    if(query.exec() && query.next()){
        return query.value(0).toInt();
    }
    return -1;
}

bool Database::exportTableCSV(int table_id, const QString& filePath){
    QString absolutePath = QFileInfo(filePath).absoluteFilePath();

    QFile file(absolutePath);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        qCritical() << "Ошибка открытия файла:" << file.errorString() << "Путь:" << absolutePath;
        return false;
    }

    QTextStream out(&file);
    out.setCodec("UTF-8");

    QSqlQuery userTableQuery(db);
    userTableQuery.prepare("SELECT * FROM user_tables WHERE table_id = :table_id");
    userTableQuery.bindValue(":table_id", table_id);

    if(!userTableQuery.exec()){
        qDebug() << "Ошибка экспорта SQL: " << userTableQuery.lastError().text();
        file.close();
        return false;
    }

    out << "[user_tables]\n";
    QSqlRecord userTableRecord = userTableQuery.record();
    for(int q = 0; q < userTableRecord.count(); ++q){
        if(q > 0){
            out << ",";
        }
        out << "\"" << userTableRecord.fieldName(q) << "\"";
    }
    out << "\n";

    if(userTableQuery.next()){
        for(int q = 0; q < userTableRecord.count(); ++q){
            if(q > 0){
                out << ",";
            }
            QVariant value = userTableQuery.value(q);
            out << "\"" << value.toString().replace("\"", "\"\"") << "\"";
        }
        out << "\n";
    }


    out << "\n[parcels]\n";
    QSqlQuery parcelsQuery(db);
    parcelsQuery.prepare("SELECT * FROM parcels WHERE table_id = :table_id");
    parcelsQuery.bindValue(":table_id", table_id);

    if(!parcelsQuery.exec()){
        qDebug() << "Ошибка посылок SQL: " << parcelsQuery.lastError().text();
        file.close();
        return false;
    }

    QSqlRecord parcelsRecord = parcelsQuery.record();
    for(int q = 0; q < parcelsRecord.count(); ++q){
        if(q > 0){
            out << ",";
        }
        out << "\"" << parcelsRecord.fieldName(q) << "\"";
    }
    out << "\n";

    while(parcelsQuery.next()){
        for(int q = 0; q < parcelsRecord.count(); ++q){
            if(q > 0){
                out << ",";
            }
            QVariant value = parcelsQuery.value(q);

            if(parcelsRecord.fieldName(q) == "date_dispatch" || parcelsRecord.fieldName(q) == "delivery_date"){
                QDate date = value.toDate();
                out << "\"" << date.toString("dd-MM-yyyy") << "\"";
            }
            else{
                out << "\"" << value.toString().replace("\"", "\"\"") << "\"";
            }
        }
        out << "\n";
    }

    file.close();
    return true;
}


bool Database::importTablesCSV(int user_id, const QString& filePath, QVariantList& importedParcels,
                               const QString& tableName, const QString& departure, const QString& destination){
    if(!db.isOpen() && !openConnection()){
        qCritical() << "Не удалось подключиться к базе данных";
        return false;
    }

    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qCritical() << "Ошибка открытия файла:" << file.errorString();
        return false;
    }

    db.transaction();

    QSqlQuery checkQuery(db);
    checkQuery.prepare("SELECT table_id FROM user_tables WHERE user_id = :user_id AND table_name = :table_name");
    checkQuery.bindValue(":user_id", user_id);
    checkQuery.bindValue(":table_name", tableName);

    if(checkQuery.exec() && checkQuery.next()){
        qCritical() << "Таблица с именем" << tableName << "уже существует";
        db.rollback();
        return false;
    }


    if(!AddTable(user_id, tableName, departure, destination)){
        qCritical() << "Ошибка создания таблицы";
        db.rollback();
        return false;
    }

    QSqlQuery query(db);
    query.prepare("SELECT table_id FROM user_tables WHERE user_id = :user_id AND table_name = :table_name ORDER BY table_id DESC LIMIT 1");
    query.bindValue(":user_id", user_id);
    query.bindValue(":table_name", tableName);
    if(!query.exec() || !query.next()){
        qCritical() << "Ошибка получения ID таблицы:" << query.lastError().text();
        db.rollback();
        return false;
    }

    int table_id = query.value(0).toInt();

    while(!file.atEnd() && file.readLine().trimmed() != "[parcels]"){}

    if(file.atEnd()){
        qCritical() << "Неверный формат файла: отсутствует секция parcels";
        db.rollback();
        return false;
    }
    file.readLine();

    QString line;
    importedParcels.clear();
    while(!file.atEnd()){
        line = file.readLine().trimmed();
        if(line.isEmpty()){
            continue;
        }
        QStringList fields = parseCsvLine(line);
        if(fields.size() < 8){
            continue;
        }
        QVariantMap parcel;
        parcel["number_parcel"] = fields[1].trimmed();
        parcel["sender"] = fields[2].trimmed();
        parcel["recipient"] = fields[3].trimmed();

        QDate date_dispatch = QDate::fromString(fields[4].trimmed(), "dd-MM-yyyy");
        QDate delivery_date = QDate::fromString(fields[5].trimmed(), "dd-MM-yyyy");
        if(!date_dispatch.isValid() || !delivery_date.isValid()){
            qCritical() << "Неверный формат даты в посылке";
            db.rollback();
            return false;
        }

        if(!compareDates(date_dispatch, delivery_date)){
            qCritical() << "Дата доставки должна быть позже даты отправки";
            db.rollback();
            return false;
        }

        parcel["date_dispatch"] = date_dispatch.toString("dd-MM-yyyy");
        parcel["delivery_date"] = delivery_date.toString("dd-MM-yyyy");
        parcel["status_parcel"] = fields[6].trimmed();

        bool ok;
        double weight = fields[7].trimmed().toDouble(&ok);
        if(!ok || weight <= 0){
            qCritical() << "Неверный вес посылки";
            db.rollback();
            return false;
        }
        parcel["weight"] = weight;

        QMap<QString, QVariant> parcelMap;
        parcelMap["number_parcel"] = parcel["number_parcel"];
        parcelMap["sender"] = parcel["sender"];
        parcelMap["recipient"] = parcel["recipient"];
        parcelMap["date_dispatch"] = date_dispatch;
        parcelMap["delivery_date"] = delivery_date;
        parcelMap["status_parcel"] = parcel["status_parcel"];
        parcelMap["weight"] = weight;
        parcelMap["table_id"] = table_id;

        if(!checkCsvData(parcelMap)){
            qCritical() << "Неверные данные посылки";
            db.rollback();
            return false;
        }

        if(!addParcel(parcelMap)){
            qCritical() << "Ошибка добавления посылки";
            db.rollback();
            return false;
        }

        importedParcels.append(parcel);
    }

    if(!db.commit()){
        db.rollback();
        return false;
    }

    file.close();
    return true;
}

QStringList Database::parseCsvLine(const QString& line){
    QStringList fields;
    QString field;
    bool inQuotes = false;

    for(int q = 0; q < line.length(); ++q){
        QChar item = line.at(q);
        if(item == '\"'){
            if(inQuotes && q + 1 < line.length() && line.at(q + 1) == '\"'){
                field += '\"';
                q++;
            }
            else{
                inQuotes = !inQuotes;
            }
        }
        else if(item == ',' && !inQuotes){
            fields.append(field);
            field.clear();
        }
        else{
            field += item;
        }
    }
    fields.append(field);
    return fields;
}

bool Database::tblExistsUser(int user_id, const QString &table_name) {
    bool exists = false;
    QSqlQuery query(db);
    query.prepare("SELECT 1 FROM user_tables WHERE user_id = :user_id AND table_name = :table_name");
    query.bindValue(":user_id", user_id);
    query.bindValue(":table_name", table_name);
    if(query.exec()) {
        exists = query.next();
    }
    else{
        qDebug() << "Ошибка проверки таблицы пользователя: " << query.lastError().text();
    }
    return exists;
}

int Database::getTblId(int user_id, const QString& table_name) {
    int table_id = -1;
    QSqlQuery query(db);
    query.prepare("SELECT table_id FROM user_tables WHERE user_id = :user_id AND table_name = :table_name");
    query.bindValue(":user_id", user_id);
    query.bindValue(":table_name", table_name);

    if(query.exec() && query.next()) {
        table_id = query.value(0).toInt();
    }
    else {
        qDebug() << "Ошибка получения ID таблицы: " << query.lastError().text();
    }
    return table_id;
}

bool Database::compareDates(const QDate& date_dispatch, const QDate& delivery_date) {
    return delivery_date > date_dispatch;
}

bool Database::checkCsvData(const QMap<QString, QVariant>& rowData) {
    if(rowData["number_parcel"].toString().isEmpty() ||
        rowData["sender"].toString().isEmpty() ||
        rowData["recipient"].toString().isEmpty() ||
        !rowData["date_dispatch"].toDate().isValid() ||
        !rowData["delivery_date"].toDate().isValid() ||
        rowData["status_parcel"].toString().isEmpty() ||
        rowData["weight"].toDouble() <= 0 ||
        rowData["table_id"].toInt() <= 0){
        return false;
    }
    return true;
}

QVector<QPair<int, QString>> Database::getUserTbls(int user_id) {
    QVector<QPair<int, QString>> tbls;
    QSqlQuery query(db);
    query.prepare("SELECT table_id, table_name FROM user_tables WHERE user_id = :user_id ORDER BY table_name");
    query.bindValue(":user_id", user_id);
    if(query.exec()) {
        while(query.next()) {
            tbls.append(qMakePair(query.value("table_id").toInt(), query.value("table_name").toString()));
        }
    }
    else {
        qDebug() << "Ошибка получения таблиц пользователя: " << query.lastError().text();
    }
    return tbls;
}

bool Database::AddTable(int user_id, const QString &tableName, const QString &departure, const QString &destination){
    QSqlQuery query(db);
    query.prepare("INSERT INTO user_tables (user_id, table_name, departure_location, destination_location) "
                  "VALUES (:user_id, :table_name, :departure, :destination)");
    query.bindValue(":user_id", user_id);
    query.bindValue(":table_name", tableName);
    query.bindValue(":departure", departure);
    query.bindValue(":destination", destination);

    if(!query.exec()) {
        qDebug() << "Не удалось добавить таблицу: " << query.lastError().text();
        return false;
    }
    return true;
}

bool Database::updateTable(int user_id, int table_id, const QString &newDeparture, const QString &newDestination){
    QSqlQuery query(db);

    query.prepare("SELECT COUNT(*) FROM user_tables WHERE user_id = :user_id AND table_id = :table_id");
    query.bindValue(":user_id", user_id);
    query.bindValue(":table_id", table_id);

    if(!query.exec()){
        qDebug() << "Ошибка проверки таблицы: " << query.lastError().text();
        return false;
    }

    query.next();
    if(query.value(0).toInt() == 0){
        qDebug() << "Ошибка обновления: таблица не найдена или не принадлежит пользователю";
        return false;
    }

    query.prepare("UPDATE user_tables "
                  "SET departure_location = :departure, "
                  "destination_location = :destination "
                  "WHERE user_id = :user_id AND table_id = :table_id");
    query.bindValue(":departure", newDeparture);
    query.bindValue(":destination", newDestination);
    query.bindValue(":table_id", table_id);
    query.bindValue(":user_id", user_id);

    if(!query.exec()){
        qDebug() << "Ошибка при обновлении таблицы: " << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}

bool Database::renameTable(int user_id, int table_id, const QString &newTableName){
    QSqlQuery query(db);

    query.prepare("SELECT COUNT(*) FROM user_tables WHERE user_id = :user_id AND table_name = :newName");
    query.bindValue(":user_id", user_id);
    query.bindValue(":newName", newTableName);

    if(!query.exec()) {
        qDebug() << "Ошибка SQL: " << query.lastError().text();
        return false;
    }

    query.next();
    if (query.value(0).toInt() > 0){
        qDebug() << "Ошибка переименования таблицы, данное имя для таблицы уже занято (" << newTableName << ')';
        return false;
    }
    query.prepare("UPDATE user_tables SET table_name = :newName WHERE user_id = :user_id AND table_id = :table_id");
    query.bindValue(":newName", newTableName);
    query.bindValue(":table_id", table_id);
    query.bindValue(":user_id", user_id);

    if(!query.exec()){
        qDebug() << "Ошибка при переименовании таблицы: " << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}

bool Database::delTableNParcels(int user_id, int table_id){
    QSqlQuery query(db);

    query.prepare("SELECT table_id FROM user_tables WHERE user_id = :user_id AND table_id = :table_id");
    query.bindValue(":user_id", user_id);
    query.bindValue(":table_id", table_id);
    if(!query.exec() || !query.next()) {
        qDebug() << "Ошибка SQL: " << query.lastError().text();
        db.rollback();
        return false;
    }

    // удаляем посылки
    query.prepare("DELETE FROM parcels WHERE table_id = :table_id");
    query.bindValue(":table_id", table_id);
    if(!query.exec()){
        qDebug() << "Не удалось удалить посылки таблицы: " << query.lastError().text();
        db.rollback();
        return false;
    }
    //удаляем таблицу
    query.prepare("DELETE FROM user_tables WHERE user_id = :user_id AND table_id = :table_id");
    query.bindValue(":user_id", user_id);
    query.bindValue(":table_id", table_id);

    if(!query.exec()){
        qDebug() << "Не удалось удалить таблицу: " << query.lastError().text();
        db.rollback();
        return false;
    }

    return true;
}

