#ifndef CLIENT_H
#define CLIENT_H

#include<QObject>
#include<QTcpSocket>
#include<QJsonDocument>
#include<QJsonObject>
#include<QTimer>
#include<QTextCodec>
#include<QDebug>


class Client : public QObject {
    Q_OBJECT
public:
    static Client* instance();

    bool connectToServer(const QString &host = "localhost", quint16 port = 12345);
    void sendRequest(const QString &operation, const QVariantMap &data = QVariantMap());
    bool isConnected() const;

signals:
    void connected();
    void disconnected();
    void responseReceived(const QJsonObject &response);
    void errorOccurred(const QString &error);

private:
    explicit Client(QObject *parent = nullptr);
    ~Client() = default;

    void reconnect();
    void processBuffer();

    QTcpSocket* socket;
    QByteArray buffer;
    QString host;
    quint16 port;
    QTimer* reconnectTimer;
    static Client* m_instance;
};

#endif // CLIENT_H
