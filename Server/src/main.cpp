#include"server.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Server server;
    server.startServer();

    qInfo() << "Сервер запущен. Ожидание подключений...";
    return a.exec();
}
