#ifndef USERTABLE_H
#define USERTABLE_H

#include<QPushButton>
#include<QDebug>
#include<QMenu>
#include<QLabel>
#include<QWidgetAction>
#include<QMessageBox>
#include<QMouseEvent>

#include"tableDialogAdd.h"
#include"tableDialogRename.h"
#include"tableDialogEdit.h"
#include"tableDialogExport.h"
#include"tableDialogDelete.h"
#include"tableDialogOpen.h"
#include"tableDialogView.h"

class UserTable : public QPushButton{
    Q_OBJECT
public:
    explicit UserTable(const QString &tableName, const QString &departure, const QString &destination, QWidget *parent = nullptr);
    QString getTableName() const;
    QString getDeparture() const;
    QString getDestination() const;
    int getTableId() const;
    void setTableId(int id);
    void setTableData(const QString &name, const QString &departure, const QString &destination);

signals:
    void tableEdited(int tableId, const QString& newName, const QString& newDeparture, const QString& newDestination);
    void tableRenamed(const QString& newName);
    void tableDeleted(int tableId);
    void tableExported(const QString& tableName, const QString& filePath, const QString& format);
    void tableViewRequested(int tableId);
    void transportViewRequested(int tableId);
    void tableOpened(int tableId);
    void addParcelRequested(int tableId);

private slots:
    void showContextMenu(const QPoint &pos);
    void handleViewAction();
    void handleOpenAction();
    void handleRenameAction();
    void handleEditAction();
    void handleExportAction();
    void handleDeleteAction();
    void handleAddParcelAction();


protected:
    void mousePressEvent(QMouseEvent* event) override;

private:
    QString tableName;
    QString departureLocation;
    QString destinationLocation;
    int table_id = -1;
};

#endif // USERTABLE_H
