#include "client.h"

Client* Client::m_instance = nullptr;

Client* Client::instance(){
    if(!m_instance){
        m_instance = new Client();
    }
    return m_instance;
}

Client::Client(QObject *parent) : QObject(parent), socket(new QTcpSocket(this)), reconnectTimer(new QTimer(this)){
    reconnectTimer -> setInterval(3000);
    reconnectTimer -> setSingleShot(true);

    connect(socket, &QTcpSocket::readyRead, this, &Client::processBuffer);
    connect(socket, &QTcpSocket::connected, this, &Client::connected);
    connect(socket, &QTcpSocket::disconnected, this, [this]() {
        emit disconnected();
        reconnectTimer -> start();
    });

    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred), this, [this](QAbstractSocket::SocketError){
        emit errorOccurred(socket -> errorString());
        reconnectTimer -> start();
    });
    connect(reconnectTimer, &QTimer::timeout, this, &Client::reconnect);
}

bool Client::connectToServer(const QString &host, quint16 port){
    this -> host = host;
    this -> port = port;

    if(socket->state() == QAbstractSocket::ConnectedState){
        return true;
    }

    socket -> connectToHost(host, port);
    return socket -> waitForConnected(3000);
}

void Client::reconnect(){
    if(!isConnected()){
        qDebug() << "Попытка переподключения...";
        connectToServer(host, port);
    }
}

void Client::sendRequest(const QString &operation, const QVariantMap &data){
    if(!socket || socket->state() != QTcpSocket::ConnectedState){
        emit errorOccurred("Нет подключения к серверу");
        return;
    }

    QJsonObject jsonData;
    for(auto iter = data.constBegin(); iter != data.constEnd(); ++iter) {
        jsonData[iter.key()] = QJsonValue::fromVariant(iter.value());
    }

    QJsonObject request;
    request["operation"] = operation;
    request["data"] = jsonData;

    QByteArray requestData = QJsonDocument(request).toJson(QJsonDocument::Compact) + '\n';

    if(socket -> write(requestData) == -1){
        emit errorOccurred("Ошибка отправки: " + socket->errorString());
    }
    socket -> flush();
}


void Client::processBuffer(){
    buffer.append(socket -> readAll());

    while(true){
        int endIndex = buffer.indexOf('\n');
        if(endIndex == -1){
            break;
        }

        QByteArray message = buffer.left(endIndex);
        buffer = buffer.mid(endIndex + 1);

        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(message, &parseError);

        if(parseError.error != QJsonParseError::NoError){
            emit errorOccurred("Ошибка парсинга JSON: " + parseError.errorString());
            continue;
        }

        if(doc.isObject()){
            emit responseReceived(doc.object());
        }
    }
}

bool Client::isConnected() const{
    return socket -> state() == QAbstractSocket::ConnectedState;
}
