#ifndef SQLWORK_H
#define SQLWORK_H

#include<QSqlDatabase>
#include<QSqlQuery>
#include<QSqlError>
#include<QSqlRecord>
#include<QJsonDocument>
#include<QJsonObject>
#include<QString>
#include<QDebug>
#include<QCryptographicHash>
#include<QDate>
#include<QHash>
#include<QVariant>
#include<QFile>
#include<QTextStream>
#include<QFileInfo>


class Database {
public:
    Database();
    ~Database();

    bool openConnection();
    void closeConnection();
    bool checkConnection();

    bool userExists(const QString& username);
    bool authenticUser(const QString& username, const QString& passwd);
    bool addRegUser(const QString &username, const QString& passwd);
    bool isCorrectPassword(const QString& passwd);
    QString hashPasswd(const QString& passwd);
    int getuser_id(const QString& username);
    QString escapeSql(const QString& value);

    int getTblId(int user_id, const QString& table_name);
    QVector<QPair<int, QString>> getUserTbls(int user_id);

    int getParcelId(const QString &number_parcel);
    QMap<QString, QVariant> getParcelData(int parcel_id);

    bool fillParcelResults(QSqlQuery& query, QVector<QMap<QString, QVariant>>& result);
    bool searchInAllParcels(int user_id, const QString& searchText, QVector<QMap<QString, QVariant>>& result);
    bool searchInTable(int table_id, const QString& searchText, QVector<QMap<QString, QVariant>>& result);

    bool updateParcel(const QMap<QString, QVariant>& parcel);
    bool addParcel(const QMap<QString, QVariant>& parcel);
    bool delParcel(int parcel_id);
    bool moveParcelToTable(int parcel_id, int user_id, const QString &new_table_name);
    bool moveParcel(int parcel_id, int new_table_id);

    bool tblExistsUser(int user_id, const QString &table_name);
    bool renameTable(int user_id, int table_id, const QString &newTableName);
    bool checkCsvData(const QMap<QString, QVariant>& rowData);
    bool compareDates(const QDate& date_dispatch, const QDate& delivery_date);
    QStringList parseCsvLine(const QString& line);
    bool exportTableCSV(int table_id, const QString& filePath);
    bool importTablesCSV(int user_id, const QString& filePath, QVariantList& importedParcels, const QString& tableName, const QString& departure, const QString& destination);
    QMap<QString, QVariant> getTransportation(int table_id);

    QVector<QMap<QString, QVariant>> getTableParcels(int table_id);
    bool updateTable(int user_id, int table_id, const QString &newDeparture, const QString &newDestination);

    bool delTableNParcels(int user_id, int table_id);
    bool AddTable(int user_id, const QString &tableName, const QString &departure, const QString &destination);
    int gettable_id(const QString& nameTable, int user_id);


private:
    QSqlDatabase db;
};

#endif // SQLWORK_H
