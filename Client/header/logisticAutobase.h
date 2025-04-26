#ifndef LOGISTICAUTOBASE_H
#define LOGISTICAUTOBASE_H

#include<QApplication>
#include"authorizationDialog.h"
#include"mainwindow.h"
#include"client.h"

class LogisticAutobase : public QApplication {
    Q_OBJECT
public:
    LogisticAutobase(int &argc, char **argv) : QApplication(argc, argv), authDialog(nullptr), mainWindow(nullptr){
        if(!Client::instance() -> isConnected()){
            if(!Client::instance() -> connectToServer()){
                qDebug() << "Не удалось подключиться к серверу";
            }
        }
        showLoginDialog();
    }


    ~LogisticAutobase(){
        cleanup();
    }

private slots:
    void handleLoginSuccess(const QString &username, int userId){
        cleanup();
        mainWindow = new MainWindow(username, userId);
        mainWindow -> show();

        if(authDialog){
            authDialog -> close();
            authDialog -> deleteLater();
            authDialog = nullptr;
        }
    }

private:
    void handleAuthSuccess(){
        if(!authDialog){
            qCritical() << "AuthDialog не существует!";
            return;
        }

        QString username = authDialog -> getUsername();
        int userId = authDialog -> getUserId();
        cleanup();
        try{
            mainWindow = new MainWindow(username, userId);
            mainWindow -> show();

        }
        catch (const std::exception &e){
            qCritical() << "Ошибка при создании MainWindow:" << e.what();
            QMessageBox::critical(nullptr, "Ошибка", "Не удалось открыть главное окно");

            showLoginDialog();
        }
    }
    void showLoginDialog(){
        cleanup();
        authDialog = new AuthDialog();

        connect(authDialog, &AuthDialog::accepted, this, &LogisticAutobase::handleAuthSuccess, Qt::QueuedConnection);
        connect(authDialog, &AuthDialog::rejected, this, &QApplication::quit);
        authDialog -> show();
    }

    void cleanup(){
        if(mainWindow){
            mainWindow -> deleteLater();
            mainWindow = nullptr;
        }
        if(authDialog){
            authDialog -> deleteLater();
            authDialog = nullptr;
        }
    }

    AuthDialog* authDialog = nullptr;
    MainWindow* mainWindow = nullptr;
};

#endif // LOGISTICAUTOBASE_H
