#include"mainwindow.h"

MainWindow::MainWindow(const QString& username, int user_id, QWidget* parent) : QMainWindow(parent), user_id(user_id), activeUser(username){
    setWindowTitle("Логистическая автобаза");
    resize(1920, 1080);

    interface();
}

void MainWindow::interface(){
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout -> setContentsMargins(10, 10, 10, 10);
    mainLayout -> setSpacing(10);

    createTopPanel(mainLayout);
    createContentArea(mainLayout);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateDateTime);
    timer -> start(1000);

    initNetwork();
}

MainWindow::~MainWindow(){
    timer -> stop();
    delete timer;
}

void MainWindow::initNetwork(){
    connect(Client::instance(), &Client::responseReceived, this, &MainWindow::handleServerResponse);
    connect(Client::instance(), &Client::errorOccurred, this, &MainWindow::handleConnectionError);

    loadUserTables();
    loadAllParcels();
}

void MainWindow::createTopPanel(QVBoxLayout* mainLayout){
    QWidget* topPanel = new QWidget();
    QHBoxLayout* topLayout = new QHBoxLayout(topPanel);
    topLayout -> setContentsMargins(0, 0, 0, 0);
    topLayout -> setSpacing(20);

    createDateTimeWidget(topLayout);
    createSearchField(topLayout);
    createExitButton(topLayout);

    mainLayout -> addWidget(topPanel);
}

void MainWindow::createDateTimeWidget(QHBoxLayout* topLayout){
    QWidget* datetimeWidget = new QWidget();
    QVBoxLayout* datetimeLayout = new QVBoxLayout(datetimeWidget);
    datetimeLayout -> setContentsMargins(0, 0, 0, 0);
    datetimeLayout -> setSpacing(2);

    dateLabel = new QLabel();
    timeLabel = new QLabel();
    updateDateTime();

    QFont datetimeFont("Arial", 15);
    dateLabel -> setFont(datetimeFont);
    timeLabel -> setFont(datetimeFont);
    timeLabel -> setStyleSheet("color: #555;");

    datetimeLayout -> addWidget(dateLabel);
    datetimeLayout -> addWidget(timeLabel);
    topLayout -> addWidget(datetimeWidget);
    topLayout -> addStretch();
}

void MainWindow::createSearchField(QHBoxLayout* topLayout){
    searchLineEdit = new QLineEdit();
    searchLineEdit -> setPlaceholderText("Найти");
    searchLineEdit -> setFixedWidth(250);
    searchLineEdit -> setStyleSheet(
        "QLineEdit {"
        "   padding: 8px 15px;"
        "   border: 1px solid #ccc;"
        "   border-radius: 15px;"
        "   background: white;"
        "}");
    searchLineEdit -> addAction(QIcon(":/resource/resources/loupe24x24.png"), QLineEdit::LeadingPosition);

    connect(searchLineEdit, &QLineEdit::textChanged, this, &MainWindow::handleSearch);

    topLayout -> addWidget(searchLineEdit);
}

void MainWindow::createExitButton(QHBoxLayout* topLayout){
    QPushButton* exitButton = new QPushButton();
    exitButton -> setFixedSize(40, 40);
    exitButton -> setIcon(QIcon(":/resource/resources/exit50.png"));
    exitButton -> setToolTip("Выход");
    exitButton -> setIconSize(QSize(24, 24));
    exitButton -> setStyleSheet(
        "QPushButton {"
        "   border: 1px solid #ccc;"
        "   border-radius: 5px;"
        "   background: #f8f8f8;"
        "}"
        "QPushButton:hover {"
        "   background: #f0f0f0;"
        "}"
        "QPushButton:pressed {"
        "   background: #e0e0e0;"
        "}");
    connect(exitButton, &QPushButton::clicked, this, &MainWindow::close);
    topLayout -> addWidget(exitButton);
}


void MainWindow::createContentArea(QVBoxLayout* mainLayout){
    QHBoxLayout* contentLayout = new QHBoxLayout();
    contentLayout -> setSpacing(15);
    contentLayout -> setContentsMargins(0, 0, 0, 0);

    createLeftPanel(contentLayout);
    createRightPanel(contentLayout);

    mainLayout -> addLayout(contentLayout, 1);
}

void MainWindow::createLeftPanel(QHBoxLayout* contentLayout){
    QWidget* leftPanel = new QWidget();
    leftPanel -> setFixedWidth(250);
    QVBoxLayout* leftLayout = new QVBoxLayout(leftPanel);
    leftLayout -> setContentsMargins(0, 0, 0, 0);
    leftLayout -> setSpacing(10);

    QLabel* tablesLabel = new QLabel("Таблицы");
    tablesLabel -> setFont(QFont("Arial", 12, QFont::Bold));
    tablesLabel -> setAlignment(Qt::AlignCenter);
    leftLayout -> addWidget(tablesLabel);

    buttonLayout = new QVBoxLayout();
    buttonLayout -> setSpacing(8);
    leftLayout -> addLayout(buttonLayout);

    QPushButton* addTblBtn = new QPushButton(" Добавить переозку");
    addTblBtn -> setIcon(QIcon(":/resource/resources/plus50.png"));
    addTblBtn -> setStyleSheet(
        "QPushButton {"
        "   text-align: center;"
        "   padding: 8px;"
        "   border: 1px dashed #aaa;"
        "   border-radius: 5px;"
        "   color: #555;"
        "}"
        "QPushButton:hover {"
        "   background: #f0f0f0;"
        "}");
    leftLayout -> addWidget(addTblBtn);
    leftLayout -> addStretch();
    connect(addTblBtn, &QPushButton::clicked, this, &MainWindow::openAddTblDialog);

    contentLayout -> addWidget(leftPanel);
}

void MainWindow::createRightPanel(QHBoxLayout* contentLayout){
    QWidget* rightPanel = new QWidget();
    QVBoxLayout* rightLayout = new QVBoxLayout(rightPanel);
    rightLayout -> setContentsMargins(0, 0, 0, 0);
    rightLayout -> setSpacing(10);

    activeTableLabel = new QLabel("Общая таблица");
    activeTableLabel -> setFont(QFont("Arial", 14, QFont::Bold));
    activeTableLabel -> setAlignment(Qt::AlignCenter);
    rightLayout -> addWidget(activeTableLabel);

    cardsWidget = new QWidget();
    cardsGridLayout = new QGridLayout(cardsWidget);
    cardsGridLayout -> setContentsMargins(10, 10, 10, 10);
    cardsGridLayout -> setSpacing(15);
    cardsGridLayout -> setAlignment(Qt::AlignTop);

    cardsScrollArea = new QScrollArea();
    cardsScrollArea -> setWidget(cardsWidget);
    cardsScrollArea -> setWidgetResizable(true);
    cardsScrollArea -> setFrameShape(QFrame::NoFrame);
    rightLayout -> addWidget(cardsScrollArea);

    contentLayout -> addWidget(rightPanel, 1);
}

void MainWindow::loadUserTables(){
    QLayoutItem* item;
    while((item = buttonLayout -> takeAt(0))){
        delete item -> widget();
        delete item;
    }
    QVariantMap data;
    data["user_id"] = user_id;
    Client::instance() -> sendRequest("getUserTables", data);
}

void MainWindow::loadTableParcels(int table_id){
    QVariantMap data;
    data["table_id"] = table_id;
    Client::instance() -> sendRequest("viewTable", data);
}

void MainWindow::loadAllParcels(){
    QVariantMap data;
    data["user_id"] = user_id;
    Client::instance() -> sendRequest("getAllParcels", data);
}

void MainWindow::clearParcels(){
    QLayoutItem* item;
    while((item = cardsGridLayout -> takeAt(0))){
        delete item -> widget();
        delete item;
    }
}

void MainWindow::TableButtonConnections(UserTable* tableBtn, int table_id){
    QString table_name = tableBtn -> getTableName();
    QString departure = tableBtn -> getDeparture();
    QString destination = tableBtn -> getDestination();
    connect(tableBtn, &UserTable::tableViewRequested, this, [this, table_id, table_name](){
        activeTableLabel -> setText(table_name);
        this -> table_id = table_id;
        loadTableParcels(table_id);
    });

    connect(tableBtn, &UserTable::tableOpened, this, [this, table_id](){
        QVariantMap data;
        data["table_id"] = table_id;
        Client::instance() -> sendRequest("openTable", data);
    });

    connect(tableBtn, &UserTable::tableRenamed, this, [this, table_id](const QString& newName){
        QVariantMap data;
        data["user_id"] = user_id;
        data["table_id"] = table_id;
        data["new_name"] = newName;
        Client::instance() -> sendRequest("renameTable", data);
    });

    connect(tableBtn, &UserTable::tableEdited, this, [this](int tableId, const QString& newName, const QString& newDeparture, const QString& newDestination){
        QVariantMap data;
        data["user_id"] = user_id;
        data["table_id"] = tableId;
        data["departure_location"] = newDeparture;
        data["destination_location"] = newDestination;
        Client::instance() -> sendRequest("editTable", data);
    });

    connect(tableBtn, &UserTable::transportViewRequested, this, [this](int transportId){
        QVariantMap data;
        data["table_id"] = transportId;
        Client::instance() -> sendRequest("getTransportion", data);
    });


    connect(tableBtn, &UserTable::tableDeleted, this, [this](int tableId){
        QMessageBox::StandardButton reply;
        QVariantMap data;
        data["user_id"] = user_id;
        data["table_id"] = tableId;
        Client::instance() -> sendRequest("deleteTable", data);
    });

    connect(tableBtn, &UserTable::tableExported, this, [this, table_id](const QString& table_name, const QString& filePath, const QString& format){
        QVariantMap data;
        data["user_id"] = user_id;
        data["table_id"] = table_id;
        data["file_path"] = filePath;
        data["format"] = format;
        Client::instance() -> sendRequest("exportTable", data);
    });
    connect(tableBtn, &UserTable::addParcelRequested, this, &MainWindow::showParcelAdd);
}

void MainWindow::renderTables(const QJsonArray& tables){
    QLayoutItem* item;
    while((item = buttonLayout -> takeAt(0))){
        delete item -> widget();
        delete item;
    }
    userTables.clear();
    tableButtons.clear();

    for(const QJsonValue& tableValue : tables){
        QJsonObject jsTransportation = tableValue.toObject();
        int table_id = jsTransportation["table_id"].toInt();
        QString table_name = jsTransportation["table_name"].toString();
        QString departure = jsTransportation["departure_location"].toString();
        QString destination = jsTransportation["destination_location"].toString();

        userTables[table_id] = table_name;

        UserTable* tableBtn = new UserTable(table_name, departure, destination);
        tableBtn -> setTableId(table_id);
        tableBtn -> setIcon(QIcon(":/resource/resources/transportation32.png"));
        tableBtn -> setCursor(Qt::PointingHandCursor);
        tableBtn -> setStyleSheet(
            "QPushButton {"
            "   text-align: left;"
            "   padding: 8px;"
            "   border: 1px solid #e0e0e0;"
            "   border-radius: 5px;"
            "   background: white;"
            "}"
            "QPushButton:hover {"
            "   border: 2px solid #2196F3;"
            "}");

        buttonLayout -> addWidget(tableBtn);
        TableButtonConnections(tableBtn, table_id);
        tableButtons.append(tableBtn);
    }

    QPushButton* allTablesBtn = new QPushButton("Общая таблица");
    allTablesBtn -> setIcon(QIcon(":/resource/resources/allparcels32.png"));
    allTablesBtn -> setStyleSheet(
        "QPushButton {"
        "   text-align: left;"
        "   padding: 8px;"
        "   border: 1px solid #e0e0e0;"
        "   border-radius: 5px;"
        "   background: white;"
        "}"
        "QPushButton:hover {"
        "   border: 2px solid #2196F3;"
        "}");
    buttonLayout -> addWidget(allTablesBtn);

    connect(allTablesBtn, &QPushButton::clicked, this, [this](){
        table_id = -1;
        activeTableLabel -> setText("Общая таблица");
        loadAllParcels();
    });
}

void MainWindow::renderParcels(const QJsonArray& parcels){
    clearParcels();
    int row = 0, col = 0;
    const int maxColumns = 8;

    for(const QJsonValue& parcelValue : parcels){
        QJsonObject parcel = parcelValue.toObject();
        QVariantMap parcelData;
        parcelData["parcel_id"] = parcel["parcel_id"].toInt();
        parcelData["number_parcel"] = parcel["number_parcel"].toString();
        parcelData["sender"] = parcel["sender"].toString();
        parcelData["recipient"] = parcel["recipient"].toString();
        parcelData["date_dispatch"] = QDate::fromString(parcel["date_dispatch"].toString(), Qt::ISODate);
        parcelData["delivery_date"] = QDate::fromString(parcel["delivery_date"].toString(), Qt::ISODate);
        parcelData["status_parcel"] = parcel["status_parcel"].toString();
        parcelData["weight"] = parcel["weight"].toDouble();
        parcelData["table_id"] = parcel["table_id"].toInt();

        ParcelCard* card = new ParcelCard("default", parcelData["number_parcel"].toString(), this);
        card -> setParcelData(parcelData);
        card -> setCursor(Qt::PointingHandCursor);
        card -> setStyleSheet(
            "QPushButton {"
            "   border: 1px solid #e0e0e0;"
            "   border-radius: 10px;"
            "   background: #f8f8f8;"
            "}"
            "QPushButton:hover {"
            "   border: 2px solid #4CAF50;"
            "}");

        cardsGridLayout -> addWidget(card, row, col);
        connect(card, &ParcelCard::openRequested, this, &MainWindow::showParcelView);
        connect(card, &ParcelCard::editRequested, this, &MainWindow::showParcelEdit);
        connect(card, &ParcelCard::moveRequested, this, &MainWindow::showParcelMove);
        connect(card, &ParcelCard::deleteRequested, this, &MainWindow::showParcelDelete);

        if(++col >= maxColumns){
            col = 0;
            row++;
        }
    }
}
void MainWindow::refreshView(){
    if(table_id == -1){
        loadAllParcels();
    }
    else{
        loadTableParcels(table_id);
    }
}

void MainWindow::refreshAllData(){
    loadUserTables();
    refreshView();
}

void MainWindow::handleServerResponse(const QJsonObject &response){
    if(response["status"].toString() != "success"){
        QMessageBox::warning(this, "Ошибка", response["message"].toString());
        return;
    }

    QString operation = response["operation"].toString();
    QString successMessage;

    if(operation == "getUserTables"){
        renderTables(response["tables"].toArray());
        return;
    }
    else if(operation == "openTable"){
        QString tableName = response["table_name"].toString();
        int tableId = response["table_id"].toInt();
        QJsonArray parcels = response["parcels"].toArray();
        showTableDetails(tableName, tableId, parcels);
        return;
    }
    else if(operation == "viewTable" || operation == "getAllParcels"){
        renderParcels(response["parcels"].toArray());
        return;
    }
    else if(operation == "getTransportion"){
        QVariantMap tableData;
        tableData["table_id"] = response["table_id"].toInt();
        tableData["table_name"] = response["table_name"].toString();
        tableData["departure_location"] = response["departure_location"].toString();
        tableData["destination_location"] = response["destination_location"].toString();

        showViewTransportation(tableData);
        return;
    }
    else if(operation == "searchInTable" || operation == "searchInAllParcels"){
        renderParcels(response["parcels"].toArray());
        return;
    }
    else if(operation == "createTable"){
        successMessage = "Перевозка успешно создана";
    }
    else if(operation == "renameTable"){
        successMessage = "Таблица успешно переименована";
    }
    else if(operation == "editTable"){
        successMessage = "Перевозка успешно обновлена";
    }
    else if(operation == "deleteTable"){
        table_id = -1;
        activeTableLabel -> setText("Общая таблица");
        successMessage = "Перевозка успешно удалена";
    }
    else if(operation == "addParcel"){
        successMessage = "Посылка успешно добавлена";
    }
    else if(operation == "deleteParcel"){
        successMessage = "Посылка удалена";
    }
    else if(operation == "updateParcel"){
        successMessage = "Данные посылки успешно обновлены";
    }
    else if(operation == "moveParcel"){
        successMessage = "Посылка успешно перемещена";
    }
    else if(operation == "exportTable"){
        QString filePath = response["file_path"].toString();
        return;
    }
    else if(operation == "importTable"){
        QString tableName = response["table_name"].toString();
        QMessageBox::information(this, "Успех", QString("Перевозка '%1' успешно импортирована").arg(tableName));
    }
    else{
        qWarning() << "Неизвестная операция: " << operation;
        return;
    }

    if(!successMessage.isEmpty()){
        QMessageBox::information(this, "Успех", successMessage);
    }

    refreshAllData();
}

void MainWindow::handleConnectionError(const QString &error) {
    QMessageBox::critical(this, "Ошибка подключения", error);
    QTimer::singleShot(3000, this, [](){
        Client::instance() -> connectToServer();
    });
}

void MainWindow::updateDateTime(){
    QDateTime time = QDateTime::currentDateTime();
    dateLabel -> setText(time.toString("dd.MM.yyyy"));
    timeLabel -> setText(time.toString("HH:mm"));
}

void MainWindow::onTableImportRequested(const QString& tableName, const QString& filePath){
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл");
        return;
    }

    QJsonArray parcelsArray;
    QTextStream in(&file);
    in.setCodec("UTF-8");

    in.readLine();

    while(!in.atEnd()){
        QString line = in.readLine().trimmed();
        if(line.isEmpty()){
            continue;
        }
        line.replace("\"", "");
        QStringList fields = line.split(',');

        if(fields.size() >= 8){
            QJsonObject parcel;
            parcel["number_parcel"] = fields[1].trimmed();
            parcel["sender"] = fields[2].trimmed();
            parcel["recipient"] = fields[3].trimmed();
            parcel["date_dispatch"] = fields[4].trimmed();
            parcel["delivery_date"] = fields[5].trimmed();
            parcel["status_parcel"] = fields[6].trimmed();
            parcel["weight"] = fields[7].trimmed().toDouble();
            parcelsArray.append(parcel);
        }
    }
    file.close();

    QVariantMap data;
    data["user_id"] = user_id;
    data["table_name"] = tableName;
    data["parcels"] = QJsonDocument(parcelsArray).toVariant();

    Client::instance() -> sendRequest("importTable", data);
}

void MainWindow::handleSearch(const QString& searchText){
    if(searchText.isEmpty()){
        refreshView();
        return;
    }
    QVariantMap data;
    data["user_id"] = user_id;
    data["search_text"] = searchText;

    if(table_id != -1){
        data["table_id"] = table_id;
        Client::instance() -> sendRequest("searchInTable", data);
    }
    else{
        Client::instance() -> sendRequest("searchInAllParcels", data);
    }
}

bool MainWindow::readTransportationImpt(const QString& filePath, QString& tableName, QString& departure, QString& destination){
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл: " + file.errorString());
        return false;
    }
    QTextStream in(&file);
    bool foundSection = false;
    while(!in.atEnd()){
        QString line = in.readLine().trimmed();
        if (line == "[user_tables]") {
            foundSection = true;
            in.readLine();

            line = in.readLine().trimmed();
            QStringList fields = line.split(',');
            if(fields.size() >= 5){
                tableName = fields[2].trimmed().replace("\"", "");
                departure = fields[3].trimmed().replace("\"", "");
                destination = fields[4].trimmed().replace("\"", "");
                file.close();
                return true;
            }
            break;
        }
    }

    file.close();
    if(!foundSection){
        QMessageBox::warning(this, "Ошибка", "Файл не содержит секции [user_tables]");
    }
    else{
        QMessageBox::warning(this, "Ошибка", "Не удалось прочитать данные таблицы из файла");
    }
    return false;
}

void MainWindow::openAddTblDialog(){
    TableDialogAdd dialog(this);

    connect(&dialog, &TableDialogAdd::tableCreated, this, [this](const QString& tableName, const QString& departure, const QString& destination){
        QVariantMap data;
        data["user_id"] = user_id;
        data["table_name"] = tableName;
        data["departure_location"] = departure;
        data["destination_location"] = destination;
        Client::instance() -> sendRequest("createTable", data);
    });

    connect(&dialog, &TableDialogAdd::tableImported, this, [this](const QString& filePath){
        QString tableName, departure, destination;
        if(!readTransportationImpt(filePath, tableName, departure, destination)){
            return;
        }

        QVariantMap data;
        data["user_id"] = user_id;
        data["table_name"] = tableName;
        data["file_path"] = filePath;
        data["departure_location"] = departure;
        data["destination_location"] = destination;
        Client::instance()->sendRequest("importTable", data);
    });
    dialog.exec();
}

void MainWindow::showViewTransportation(const QVariantMap& tData){
    int table_id = tData["table_id"].toInt();
    QString tableName = tData["table_name"].toString();
    QString departure = tData["departure_location"].toString();
    QString destination = tData["destination_location"].toString();

    TableDialogView* viewDialog = new TableDialogView(table_id, tableName, departure, destination, this);
    viewDialog -> setAttribute(Qt::WA_DeleteOnClose);

    viewDialog -> exec();
}

void MainWindow::showParcelView(const QVariantMap &parcelData){
    ParcelDialogOpen* dialog = new ParcelDialogOpen(
        parcelData["number_parcel"].toString(),
        parcelData["sender"].toString(),
        parcelData["recipient"].toString(),
        parcelData["date_dispatch"].toDate(),
        parcelData["delivery_date"].toDate(),
        parcelData["status_parcel"].toString(),
        parcelData["weight"].toDouble(),
        this);

    dialog -> setAttribute(Qt::WA_DeleteOnClose);

    connect(dialog, &ParcelDialogOpen::editRequested, this, [this, parcelData, dialog](){
        dialog -> close();
        showParcelEdit(parcelData);
    });

    dialog -> exec();
}

void MainWindow::showParcelEdit(const QVariantMap &parcelData){
    if(parcelData.isEmpty() || !parcelData.contains("number_parcel")){
        return;
    }
    ParcelDialogEdit* dialog = new ParcelDialogEdit(
        parcelData["number_parcel"].toString(),
        parcelData["sender"].toString(),
        parcelData["recipient"].toString(),
        parcelData["date_dispatch"].toDate(),
        parcelData["delivery_date"].toDate(),
        parcelData["status_parcel"].toString(),
        parcelData["weight"].toDouble(),
        this);

    dialog -> setAttribute(Qt::WA_DeleteOnClose);

    connect(dialog, &ParcelDialogEdit::saveRequested, this, [this](const QVariantMap &updatedData){
        if(updatedData.isEmpty()){
            return;
        }
        QVariantMap data;
        data["number_parcel"] = updatedData["number_parcel"].toString();
        data["sender"] = updatedData["sender"].toString();
        data["recipient"] = updatedData["recipient"].toString();

        QDate dispatchDate = updatedData["date_dispatch"].toDate();
        QDate deliveryDate = updatedData["delivery_date"].toDate();
        if(!dispatchDate.isValid() || !deliveryDate.isValid()) return;

        data["date_dispatch"] = dispatchDate.toString("dd-MM-yyyy");
        data["delivery_date"] = deliveryDate.toString("dd-MM-yyyy");
        data["status_parcel"] = updatedData["status_parcel"].toString();
        data["weight"] = updatedData["weight"].toDouble();

        if(Client::instance()){
            Client::instance() -> sendRequest("updateParcel", data);
        }
    });

    dialog -> exec();
}
QStringList MainWindow::getAvailableTables(int excludeTableId) const{
    QStringList tables;
    for(auto iter = userTables.constBegin(); iter != userTables.constEnd(); ++iter){
        if(iter.key() != excludeTableId){
            tables << iter.value();
        }
    }
    return tables;
}

void MainWindow::showParcelMove(const QString &parcelNumber, int tableId){
    QStringList availableTables;
    for(auto iter = userTables.begin(); iter != userTables.end(); ++iter){
        if(iter.key() != tableId){
            availableTables << iter.value();
        }
    }

    ParcelDialogMove* dialog = new ParcelDialogMove(parcelNumber, availableTables, tableId, this);
    dialog -> setAttribute(Qt::WA_DeleteOnClose);

    connect(dialog, &ParcelDialogMove::moveRequested, this, [this](const QString& number_parcel, int TableId, const QString& targetTable){
        QVariantMap data;
        data["number_parcel"] = number_parcel;
        data["user_id"] = user_id;
        data["target_table"] = targetTable;
        Client::instance() -> sendRequest("moveParcel", data);
    });
    dialog -> exec();
}

void MainWindow::showParcelAdd(int table_id){
    ParcelDialogAdd* dialog = new ParcelDialogAdd(table_id, this);
    dialog -> setAttribute(Qt::WA_DeleteOnClose);

    connect(dialog, &ParcelDialogAdd::addRequested, this, [this](const QVariantMap& parcelData){
        Client::instance() -> sendRequest("addParcel", parcelData);
    });
    dialog -> exec();
}

void MainWindow::showViewTableDetails(const QString &tableName, int tableId, const QJsonArray &parcels){
    TableDialogView* dialog = new TableDialogView(tableId, tableName, parcels[0]["departure_location"].toString(), parcels[0]["destination_location"].toString(), this);
    dialog -> setAttribute(Qt::WA_DeleteOnClose);
    dialog -> exec();
}

void MainWindow::showTableDetails(const QString &tableName, int tableId, const QJsonArray &parcels){
    TableDialogOpen *dialog = new TableDialogOpen(tableName, parcels, this);
    dialog -> setAttribute(Qt::WA_DeleteOnClose);
    dialog -> exec();
}

void MainWindow::showParcelDelete(const QString &parcelNumber){
    ParcelDialogDelete dialog(parcelNumber, this);
    if(dialog.exec() == QDialog::Accepted){
        QVariantMap data;
        data["number_parcel"] = parcelNumber;
        Client::instance() -> sendRequest("deleteParcel", data);
    }
}


