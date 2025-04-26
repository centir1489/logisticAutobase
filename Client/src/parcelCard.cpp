#include"parcelCard.h"

ParcelCard::ParcelCard(const QString &color, const QString number_parcel, QWidget *parent)  : QPushButton(parent), number_parcel(number_parcel){
    setFixedSize(160, 160);
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &ParcelCard::customContextMenuRequested, this, &ParcelCard::showContextMenu);

    QVBoxLayout *cardLayout = new QVBoxLayout(this);
    cardLayout -> setAlignment(Qt::AlignCenter);
    cardLayout -> setSpacing(12);

    QLabel* iconCard = new QLabel();
    iconCard -> setPixmap(QPixmap(":/resource/resources/parcel100.png").scaled(72, 72, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    cardLayout -> addWidget(iconCard);

    titleLabel = new QLabel(number_parcel);
    titleLabel -> setFont(QFont("Arial", 10));
    titleLabel -> setAlignment(Qt::AlignCenter);
    cardLayout -> addWidget(titleLabel);

    setColor(color);
}

QString ParcelCard::getnumber_parcel() const{
    return number_parcel;
}

void ParcelCard::setColor(const QString &color){
    QString colour;
    if(color == "red"){
        colour = "#ffe6e6";
    }
    else if(color == "green"){
        colour = "#e6ffe6";
    }
    else if(color == "yellow"){
        colour = "#ffffe6";
    }
    else{
        colour = "#f8f8f8";
    }

    setStyleSheet(QString("QPushButton {"
                "   border: 1px solid #e0e0e0;"
                "   border-radius: 10px;"
                "   background: %1;"
                "}"
                "QPushButton:hover {"
                "   background: #f8f8f8;"
                "   border-color: #d0d0d0;"
                "}")
            .arg(colour));
}

void ParcelCard::showContextMenu(const QPoint& pos){
    QMenu menu(this);

    QWidgetAction* titleAction = new QWidgetAction(&menu);
    QLabel* titleLabel = new QLabel("Посылка: " + number_parcel);
    titleLabel -> setStyleSheet("font-weight: bold; padding: 5px 10px; background: #f0f0f0;");
    titleLabel -> setAlignment(Qt::AlignCenter);
    titleAction -> setDefaultWidget(titleLabel);
    menu.addAction(titleAction);
    menu.addSeparator();

    QAction* openAct = menu.addAction(QIcon(":/resource/resources/open50.png"), "Открыть");
    QAction* editAct = menu.addAction(QIcon(":/resource/resources/redact50.png"), "Редактировать");
    menu.addSeparator();
    QAction* moveAct = menu.addAction(QIcon(":/resource/resources/move50.png"), "Переместить");
    menu.addSeparator();
    QAction* delAct = menu.addAction(QIcon(":/resource/resources/del50.png"), "Удалить");

    connect(openAct, &QAction::triggered, this, [this](){
        emit openRequested(parcelData);
    });

    connect(editAct, &QAction::triggered, this, [this](){
        emit editRequested(parcelData);
    });

    connect(moveAct, &QAction::triggered, this, [this](){
        emit moveRequested(parcelData["number_parcel"].toString(), parcelData["table_id"].toInt(), parcelData["targetTable"].toString());
    });

    connect(delAct, &QAction::triggered, this, [this](){
        emit deleteRequested(parcelData["number_parcel"].toString(), parcelData["table_id"].toInt());
    });

    menu.setStyleSheet(
        "QMenu {"
        "   background-color: white;"
        "   border: 1px solid #e0e0e0;"
        "   border-radius: 5px;"
        "   padding: 5px;"
        "}"
        "QMenu::item {"
        "   padding: 8px 25px 8px 20px;"
        "   margin: 2px;"
        "   border-radius: 3px;"
        "}"
        "QMenu::item:selected {"
        "   background-color: #f0f0f0;"
        "}"
        "QMenu::icon {"
        "   padding-left: 5px;"
        "}");

    menu.exec(mapToGlobal(pos));
}

void ParcelCard::setParcelData(const QVariantMap& data){
    parcelData = data;
    parcelData["number_parcel"] = data["number_parcel"];
    parcelData["sender"] = data["sender"];
    parcelData["recipient"] = data["recipient"];
    parcelData["date_dispatch"] = data["date_dispatch"];
    parcelData["delivery_date"] = data["delivery_date"];
    parcelData["status_parcel"] = data["status_parcel"];
    parcelData["weight"] = data["weight"];
}

void ParcelCard::handleOpenAction(){
    if(parcelData.isEmpty()){
        qDebug() << "Не возможно открыть посылку: " << number_parcel;
        return;
    }

    QString numberParcel = parcelData["number_parcel"].toString();
    QString sender = parcelData["sender"].toString();
    QString recipient = parcelData["recipient"].toString();
    QDate dateDispatch = parcelData["date_dispatch"].toDate();
    QDate deliveryDate = parcelData["delivery_date"].toDate();
    QString status = parcelData["status_parcel"].toString();
    double weight = parcelData["weight"].toDouble();

    ParcelDialogOpen *dialog = new ParcelDialogOpen(numberParcel, sender, recipient, dateDispatch, deliveryDate, status, weight, this -> parentWidget());

    dialog -> setAttribute(Qt::WA_DeleteOnClose);
    dialog -> show();

    // сигнал редактирования
    connect(dialog, &ParcelDialogOpen::editRequested, [this, dialog](){
        dialog->close();
        handleEditAction();
    });
}

void ParcelCard::handleEditAction(){
    QString numberParcel = parcelData["number_parcel"].toString();
    QString sender = parcelData["sender"].toString();
    QString recipient = parcelData["recipient"].toString();
    QDate dateDispatch = parcelData["date_dispatch"].toDate();
    QDate deliveryDate = parcelData["delivery_date"].toDate();
    QString status = parcelData["status_parcel"].toString();
    double weight = parcelData["weight"].toDouble();

    ParcelDialogEdit* dialog = new ParcelDialogEdit(numberParcel, sender, recipient, dateDispatch, deliveryDate, status, weight, this -> parentWidget());

    dialog -> setAttribute(Qt::WA_DeleteOnClose);
    dialog -> show();
}

void ParcelCard::handleAddAction(){
    int table_id = parcelData.contains("table_id") ? parcelData["table_id"].toInt() : 1;

    ParcelDialogAdd* dialog = new ParcelDialogAdd(table_id, parentWidget());
    dialog -> setAttribute(Qt::WA_DeleteOnClose);

    dialog -> exec();
}

void ParcelCard::handleMoveAction(){
    QString parcelNumber = parcelData["number_parcel"].toString();
    int activeTable_id = parcelData["table_id"].toInt();

    MainWindow* mainWindow = qobject_cast<MainWindow*>(parent()->parent());
    if (!mainWindow) {
        qWarning() << "Не удалось получить доступ к MainWindow";
        return;
    }

    QStringList availableTables = mainWindow -> getAvailableTables(activeTable_id);

    ParcelDialogMove* dialog = new ParcelDialogMove(parcelNumber, availableTables, activeTable_id, parentWidget());

    dialog -> setAttribute(Qt::WA_DeleteOnClose);

    connect(dialog, &ParcelDialogMove::moveRequested, this, [this](const QString& parcelNum, int sourceTableId, const QString& targetTable){
            emit moveRequested(parcelNum, sourceTableId, targetTable);
    });
    dialog -> exec();
}

void ParcelCard::mousePressEvent(QMouseEvent* event){
    if(event -> button() == Qt::LeftButton){
        if(!parcelData.isEmpty()){
            emit openRequested(parcelData);
        }
    }
    QPushButton::mousePressEvent(event);
}
