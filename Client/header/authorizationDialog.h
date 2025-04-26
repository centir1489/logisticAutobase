#ifndef AUTHORIZATIONDIALOG_H
#define AUTHORIZATIONDIALOG_H

#include<QDialog>
#include<QStackedWidget>
#include<QLineEdit>
#include<QPushButton>
#include<QLabel>
#include<QMessageBox>
#include<QRegExpValidator>
#include<QVBoxLayout>
#include<QTimer>
#include<QRegExpValidator>
#include"client.h"

class AuthDialog : public QDialog{
    Q_OBJECT
public:
    explicit AuthDialog(QWidget *parent = nullptr);
    ~AuthDialog() = default;

    QString getUsername() const;
    int getUserId() const;

signals:
    void accepted();
    void rejected();

private slots:
    void handleProceed();
    void handleLogin();
    void handleRegister();
    void handleBack();

    void handleServerResponse(const QJsonObject &response);
    void handleConnectionError(const QString &error);
    void checkConnection();

private:
    void interface();
    enum Screen{
        LoginScreen,
        PasswordScreen,
        RegisterScreen
    };

    void createLoginScreen();
    void createPasswordScreen();
    void createRegisterScreen();
    void showScreen(Screen screen);
    void setLoading(bool loading);
    bool correctPasswd(const QString& passwd);

    QStackedWidget* stackedWidget;
    QString username;
    int user_id = -1;

    // вход
    QLineEdit* usernameEdit;
    QPushButton* proceedButton;

    //пароль
    QLineEdit* passwdEdit;
    QPushButton* loginButton;
    QPushButton* backButton1;

    //регистарция
    QLineEdit* regUsernameEdit;
    QLineEdit* regpasswdEdit;
    QLineEdit* regConfirmpasswdEdit;
    QPushButton* registerButton;
    QPushButton* backButton2;
};

#endif // AUTHORIZATIONDIALOG_H
