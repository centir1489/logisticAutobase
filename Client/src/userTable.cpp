#include "userTable.h"

UserTable::UserTable(const QString& tableName, const QString& departure, const QString& destination, QWidget *parent)
    : QPushButton(parent), tableName(tableName), departureLocation(departure), destinationLocation(destination){
    setText("  " + tableName);
    setStyleSheet(
        "QPushButton {"
        "   text-align: left;"
        "   padding: 10px 15px;"
        "   border: 1px solid #e0e0e0;"
        "   border-radius: 6px;"
        "   background: white;"
        "   font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "   background: #f5f5f5;"
        "}"
        "QPushButton:pressed {"
        "   background: #e9e9e9;"
        "}");

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &UserTable::customContextMenuRequested, this, &UserTable::showContextMenu);
}

QString UserTable::getTableName() const{
    return tableName;
}

QString UserTable::getDeparture() const{
    return departureLocation;
}

QString UserTable::getDestination() const{
    return destinationLocation;
}

int UserTable::getTableId() const{
    return table_id;
}

void UserTable::setTableId(int id){
    table_id = id;
}

void UserTable::setTableData(const QString& name, const QString& departure, const QString& destination){
    tableName = name;
    departureLocation = departure;
    destinationLocation = destination;
    setText("  " + tableName);
}

void UserTable::showContextMenu(const QPoint& pos){
    QMenu menu(this);

    QWidgetAction* titleAction = new QWidgetAction(&menu);
    QLabel* titleLabel = new QLabel(tableName);
    titleLabel -> setStyleSheet("font-weight: bold; padding: 5px 10px; background: #f0f0f0;");
    titleLabel -> setAlignment(Qt::AlignCenter);
    titleAction -> setDefaultWidget(titleLabel);
    menu.addAction(titleAction);
    menu.addSeparator();

    QAction* openAct = menu.addAction(QIcon(":/resource/resources/open50.png"), "Открыть");
    QAction* renameAct = menu.addAction(QIcon(":/resource/resources/rename50.png"), "Переименовать");
    QAction* editAct = menu.addAction(QIcon(":/resource/resources/dedit-20.png"), "Редактировать");
    menu.addSeparator();
    QAction* viewAct = menu.addAction(QIcon(":/resource/resources/table32.png"), "Просмотреть посылки");
    QAction* addParcelAct = menu.addAction(QIcon(":/resource/resources/daddparcel50.png"), "Создать посылку");
    menu.addSeparator();
    QAction* deleteAct = menu.addAction(QIcon(":/resource/resources/del50.png"), "Удалить");
    QAction* exportAct = menu.addAction(QIcon(":/resource/resources/export50.png"), "Экспортировать");

    connect(openAct, &QAction::triggered, this, &UserTable::handleViewAction);
    connect(renameAct, &QAction::triggered, this, &UserTable::handleRenameAction);
    connect(editAct, &QAction::triggered, this, &UserTable::handleEditAction);
    connect(deleteAct, &QAction::triggered, this, &UserTable::handleDeleteAction);
    connect(exportAct, &QAction::triggered, this, &UserTable::handleExportAction);

    connect(addParcelAct, &QAction::triggered, this, &UserTable::handleAddParcelAction);
    connect(viewAct, &QAction::triggered, this, &UserTable::handleOpenAction);

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

void UserTable::handleViewAction(){
    emit transportViewRequested(table_id);
}

void UserTable::handleOpenAction(){
    emit tableOpened(table_id);
}

void UserTable::handleRenameAction(){
    TableDialogRename* dialog = new TableDialogRename(tableName, parentWidget());
    dialog -> setAttribute(Qt::WA_DeleteOnClose);

    connect(dialog, &QDialog::accepted, [this, dialog](){
        QString newName = dialog->getNewName();
        if(!newName.isEmpty() && newName != tableName){
            QString oldName = tableName;
            tableName = newName;
            setText("  " + newName);
            emit tableRenamed(newName);
        }
    });

    dialog -> exec();
}

void UserTable::handleEditAction() {
    TableDialogEdit* dialog = new TableDialogEdit(table_id, tableName, departureLocation, destinationLocation, parentWidget());
    dialog->setAttribute(Qt::WA_DeleteOnClose);

    connect(dialog, &TableDialogEdit::updateRequested, this, [this](int id, const QString& newDeparture, const QString& newDestination){
        if(id == table_id){
            departureLocation = newDeparture;
            destinationLocation = newDestination;

            setTableData(tableName, newDeparture, newDestination);

            emit tableEdited(table_id, tableName, newDeparture, newDestination);
        }
    });

    dialog -> exec();
}

void UserTable::mousePressEvent(QMouseEvent* event){
    if(event -> button() == Qt::LeftButton){
        emit tableViewRequested(table_id);
    }
    QPushButton::mousePressEvent(event);
}

void UserTable::handleExportAction(){
    TableDialogExport dialog(tableName, parentWidget());

    connect(&dialog, &QDialog::accepted, [this, &dialog](){
        QString filePath = dialog.getExportPath();
        QString format = dialog.getSelectedFormat();
        if(!filePath.isEmpty()){
            emit tableExported(tableName, filePath, format);
            QMessageBox::information(this, "Успех", QString("Перевозка '%1' успешно экспортирована в файл:\n%2").arg(tableName).arg(filePath));
        }
    });

    dialog.exec();
}

void UserTable::handleDeleteAction(){
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Удаление таблицы", QString("Вы уверены, что хотите удалить перевозку '%1'?").arg(tableName), QMessageBox::Yes | QMessageBox::No);

    if(reply == QMessageBox::Yes){
        emit tableDeleted(table_id);
        this -> deleteLater();
    }
}

void UserTable::handleAddParcelAction(){
    emit addParcelRequested(table_id);
}
