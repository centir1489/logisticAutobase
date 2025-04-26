#ifndef SERVER_H
#define SERVER_H

#include<QTcpServer>
#include<QTcpSocket>
#include<QJsonObject>
#include<QJsonDocument>
#include<QJsonParseError>
#include<QJsonArray>
#include<QDebug>
#include<QTextCodec>
#include<QCoreApplication>
#include<iostream>

#include "sqlwork.h"

class Server : public QTcpServer{
    Q_OBJECT
public:
    explicit Server(QObject* parent = nullptr);
    ~Server() override;

    void startServer();

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    void onReadyRead();
    void onDisconnected();

private:
    enum Operation{
        CheckUser,
        Login,
        Register,
        GetUserId,

        SearchInTable,
        SearchInAllParcels,

        GetParcelData,
        GetAllParcels,

        OpenTable,
        GetUserTables,
        ViewTable,
        CreateTable,
        RenameTable,
        EditTable,
        GetTransportion,
        DeleteTable,
        ImportTable,
        ExportTable,

        AddParcel,
        UpdateParcel,
        DeleteParcel,
        MoveParcel,

        Unknown
    };

    enum Error{
        None,
        UserExists,
        InvalidCredentials,
        InvalidPassword,
        RegistrationError,
        InvalidData,
        UnknownOperation,
        DatabaseError
    };

    Operation defineOperation(const QString& operationStr);
    Error processOperation(Operation operation, const QJsonObject& data, QJsonObject& response);


    Error opCheckUser(const QJsonObject& data, QJsonObject& response);
    Error opLogin(const QJsonObject& data, QJsonObject& response);
    Error opRegister(const QJsonObject& data, QJsonObject& response);

    Error opSearchInAllParcels(const QJsonObject& data, QJsonObject& response);
    Error opSearchInTable(const QJsonObject& data, QJsonObject& response);

    Error opGetUserId(const QJsonObject& data, QJsonObject& response);
    Error opGetUserTables(const QJsonObject& data, QJsonObject& response);
    Error opOpenTable(const QJsonObject& data, QJsonObject& response);
    Error opViewTable(const QJsonObject& data, QJsonObject& response);
    Error opCreateTable(const QJsonObject& data, QJsonObject& response);
    Error opEditTransportation(const QJsonObject& data, QJsonObject& response);
    Error opDeleteTable(const QJsonObject& data, QJsonObject& response);
    Error opRenameTable(const QJsonObject& data, QJsonObject& response);
    Error opGetTransportation(const QJsonObject& data, QJsonObject& response);
    Error opExportTable(const QJsonObject& data, QJsonObject& response);
    Error opImportTable(const QJsonObject& data, QJsonObject& response);

    Error opGetAllParcels(const QJsonObject& data, QJsonObject& response);
    Error opGetParcelData(const QJsonObject& data, QJsonObject& response);
    Error opUpdateParcel(const QJsonObject& data, QJsonObject& response);
    Error opDeleteParcel(const QJsonObject& data, QJsonObject& response);
    Error opAddParcel(const QJsonObject& data, QJsonObject& response);
    Error opMoveParcel(const QJsonObject& data, QJsonObject& response);


    void processRequest(QTcpSocket *clientSocket, const QJsonObject &request);
    void sendResponse(QTcpSocket *clientSocket, const QJsonObject &response);
    void sendErrorResponse(QTcpSocket *clientSocket, const QString &message);

    Database db;
    QHash<QTcpSocket*, QByteArray> clientBuffers;
};

#endif // SERVER_H
