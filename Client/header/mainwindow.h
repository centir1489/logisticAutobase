#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include<QMainWindow>
#include<QTimer>
#include<QJsonObject>
#include<QJsonArray>
#include<QScrollArea>
#include<QMap>
#include<QVBoxLayout>
#include<QHBoxLayout>
#include<QMessageBox>
#include<QDebug>
#include<QDateTime>
#include<QScrollBar>

#include"client.h"
#include"parcelCard.h"
#include"userTable.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(const QString& username, int user_id, QWidget* parent = nullptr);
    ~MainWindow();
    QStringList getAvailableTables(int excludeTableId = -1) const;

private:
    void interface();
    void initNetwork();
    void createTopPanel(QVBoxLayout* mainLayout);
    void createDateTimeWidget(QHBoxLayout* topLayout);
    void createSearchField(QHBoxLayout* topLayout);
    void createExitButton(QHBoxLayout* topLayout);
    void createContentArea(QVBoxLayout* mainLayout);
    void createLeftPanel(QHBoxLayout* contentLayout);
    void createRightPanel(QHBoxLayout* contentLayout);

    void loadUserTables();
    void loadTableParcels(int table_id);
    void loadAllParcels();
    void clearParcels();
    void refreshView();
    void refreshAllData();

    void renderTables(const QJsonArray& tables);
    void renderParcels(const QJsonArray& parcels);
    void updateDateTime();

    void openAddTblDialog();
    void showTableView(const QVariantMap& tableData);

    void showParcelView(const QVariantMap &parcelData);
    void showParcelEdit(const QVariantMap &parcelData);
    void showParcelMove(const QString &parcelNumber, int tableId);
    void showParcelAdd(int table_id);
    void showParcelDelete(const QString &parcelNumber);
    void showTableDetails(const QString &tableName, int tableId, const QJsonArray &parcels);
    void showViewTransportation(const QVariantMap& tData);
    void showViewTableDetails(const QString &tableName, int tableId, const QJsonArray &parcels);

    void TableButtonConnections(UserTable* tableBtn, int table_id);

    void handleSearch(const QString& searchText);
    bool readTransportationImpt(const QString& filePath, QString& tableName, QString& departure, QString& destination);
    void onTableImportRequested(const QString& tableName, const QString& filePath);

private slots:
    void handleServerResponse(const QJsonObject &response);
    void handleConnectionError(const QString &error);

private:
    int user_id;
    int table_id = -1;
    QString activeUser;
    QTimer* timer;

    QWidget* centralWidget;
    QLabel* dateLabel;
    QLabel* timeLabel;
    QLabel* activeTableLabel;
    QLineEdit* searchLineEdit;

    QVBoxLayout* buttonLayout;
    QGridLayout* cardsGridLayout;
    QWidget* cardsWidget;
    QScrollArea* cardsScrollArea;

    QMap<int, QString> userTables;
    QList<UserTable*> tableButtons;
};

#endif // MAINWINDOW_H

