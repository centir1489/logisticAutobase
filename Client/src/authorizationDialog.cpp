#include"authorizationDialog.h"


AuthDialog::AuthDialog(QWidget *parent) : QDialog(parent), user_id(-1){
    interface();

    checkConnection();
}

void AuthDialog::checkConnection(){
    setLoading(true);
    if(Client::instance() -> isConnected()){
        setLoading(false);
        return;
    }

    if(!Client::instance() -> connectToServer()){
        setLoading(false);
    }
}

void AuthDialog::createLoginScreen(){
    QWidget* loginWidget = new QWidget();
    QVBoxLayout* loginLayout = new QVBoxLayout(loginWidget);
    loginLayout -> setAlignment(Qt::AlignCenter);

    QLabel* titleLabel = new QLabel("Вход в систему");
    titleLabel -> setAlignment(Qt::AlignCenter);
    titleLabel -> setStyleSheet("font-size: 18px; font-weight: normal; margin-bottom: 30px;");
    loginLayout -> addWidget(titleLabel);

    usernameEdit = new QLineEdit();
    usernameEdit -> setPlaceholderText("Введите логин");
    usernameEdit -> setMinimumWidth(250);
    QRegExp usernameRegex("^[a-zA-Z0-9_-]*$");
    QRegExpValidator* usernameValidator = new QRegExpValidator(usernameRegex, this);
    usernameEdit -> setValidator(usernameValidator);
    loginLayout -> addWidget(usernameEdit);

    proceedButton = new QPushButton("Продолжить");
    proceedButton -> setMinimumWidth(250);
    proceedButton -> setDefault(true);
    loginLayout -> addWidget(proceedButton);

    connect(usernameEdit, &QLineEdit::returnPressed, proceedButton, &QPushButton::click);

    connect(proceedButton, &QPushButton::clicked, this, &AuthDialog::handleProceed);

    stackedWidget -> addWidget(loginWidget);
}

void AuthDialog::interface(){
    setWindowTitle("Авторизация");
    setWindowIcon(QIcon(":/resource/resources/dautherization20.png"));
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setFixedSize(400, 300);

    stackedWidget = new QStackedWidget(this);

    createLoginScreen();
    createPasswordScreen();
    createRegisterScreen();

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout -> addWidget(stackedWidget);
    setLayout(mainLayout);

    connect(Client::instance(), &Client::responseReceived, this, &AuthDialog::handleServerResponse);
    connect(Client::instance(), &Client::errorOccurred, this, &AuthDialog::handleConnectionError);
}

void AuthDialog::createPasswordScreen(){
    QWidget* passwdWidget = new QWidget();
    QVBoxLayout* passwdLayout = new QVBoxLayout(passwdWidget);
    passwdLayout -> setAlignment(Qt::AlignCenter);

    QLabel* titleLabel = new QLabel("Введите пароль");
    titleLabel -> setAlignment(Qt::AlignCenter);
    titleLabel -> setStyleSheet("font-size: 18px; font-weight: normal; margin-bottom: 30px;");
    passwdLayout -> addWidget(titleLabel);

    passwdEdit = new QLineEdit();
    passwdEdit -> setPlaceholderText("Введите пароль");
    passwdEdit -> setEchoMode(QLineEdit::Password);
    passwdEdit -> setMinimumWidth(250);
    QRegExp passwordRegex("^[ -~]*$");
    QRegExpValidator *passwordValidator = new QRegExpValidator(passwordRegex, this);
    passwdEdit -> setValidator(passwordValidator);
    passwdLayout -> addWidget(passwdEdit);

    loginButton = new QPushButton("Войти");
    loginButton -> setMinimumWidth(250);
    loginButton -> setDefault(true);
    passwdLayout -> addWidget(loginButton);

    backButton1 = new QPushButton("Назад");
    backButton1 -> setMinimumWidth(250);
    passwdLayout -> addWidget(backButton1);

    connect(passwdEdit, &QLineEdit::returnPressed, loginButton, &QPushButton::click);

    connect(loginButton, &QPushButton::clicked, this, &AuthDialog::handleLogin);
    connect(backButton1, &QPushButton::clicked, this, &AuthDialog::handleBack);

    stackedWidget -> addWidget(passwdWidget);
}

void AuthDialog::createRegisterScreen(){
    QWidget* registerWidget = new QWidget();
    QVBoxLayout* registerLayout = new QVBoxLayout(registerWidget);
    registerLayout -> setAlignment(Qt::AlignCenter);

    QLabel* titleLabel = new QLabel("Регистрация");
    titleLabel -> setAlignment(Qt::AlignCenter);
    titleLabel -> setStyleSheet("font-size: 18px; font-weight: normal; margin-bottom: 30px;");
    registerLayout -> addWidget(titleLabel);

    regUsernameEdit = new QLineEdit();
    regUsernameEdit -> setPlaceholderText("Введите логин");
    regUsernameEdit -> setMinimumWidth(250);

    if(!username.isEmpty()){
        regUsernameEdit -> setText(username);
    }
    QRegExp regUsernameRegex("^[a-zA-Z0-9_-]*$");
    QRegExpValidator* regUsernameValidator = new QRegExpValidator(regUsernameRegex, this);
    regUsernameEdit -> setValidator(regUsernameValidator);
    registerLayout -> addWidget(regUsernameEdit);

    regpasswdEdit = new QLineEdit();
    regpasswdEdit -> setPlaceholderText("Введите пароль");
    regpasswdEdit -> setEchoMode(QLineEdit::Password);
    regpasswdEdit -> setMinimumWidth(250);
    QRegExp regPasswordRegex("^[ -~]*$");
    QRegExpValidator* regPasswordValidator = new QRegExpValidator(regPasswordRegex, this);
    regpasswdEdit -> setValidator(regPasswordValidator);
    registerLayout -> addWidget(regpasswdEdit);

    regConfirmpasswdEdit = new QLineEdit();
    regConfirmpasswdEdit -> setPlaceholderText("Подтвердите пароль");
    regConfirmpasswdEdit -> setEchoMode(QLineEdit::Password);
    regConfirmpasswdEdit -> setMinimumWidth(250);
    regConfirmpasswdEdit -> setValidator(regPasswordValidator);
    registerLayout -> addWidget(regConfirmpasswdEdit);

    registerButton = new QPushButton("Зарегистрироваться");
    registerButton -> setMinimumWidth(250);
    registerButton -> setDefault(true);
    registerLayout -> addWidget(registerButton);

    backButton2 = new QPushButton("Назад");
    backButton2 -> setMinimumWidth(250);
    registerLayout -> addWidget(backButton2);

    connect(regpasswdEdit, &QLineEdit::returnPressed, registerButton, &QPushButton::click);
    connect(regConfirmpasswdEdit, &QLineEdit::returnPressed, registerButton, &QPushButton::click);

    connect(registerButton, &QPushButton::clicked, this, &AuthDialog::handleRegister);
    connect(backButton2, &QPushButton::clicked, this, &AuthDialog::handleBack);

    stackedWidget -> addWidget(registerWidget);
}

void AuthDialog::handleProceed(){
    username = usernameEdit -> text().trimmed();
    if(username.length() < 5){
        QMessageBox::warning(this, "Ошибка", "Логин должен содержать не менее 5 символов");
        return;
    }
    if(username.length() > 40){
        QMessageBox::warning(this, "Ошибка", "Логин должен содержать менее 40 символов");
        return;
    }

    if(!Client::instance()->isConnected()){
        QMessageBox::critical(this, "Ошибка", "Нет подключения к серверу");
        checkConnection();
        return;
    }

    setLoading(true);
    Client::instance() -> sendRequest("checkUser", {{"username", username}});
}

void AuthDialog::handleLogin(){
    QString password = passwdEdit -> text();
    if(password.isEmpty()){
        QMessageBox::warning(this, "Ошибка", "Введите пароль");
        return;
    }

    if(!Client::instance() -> isConnected()){
        QMessageBox::critical(this, "Ошибка", "Нет подключения к серверу");
        checkConnection();
        return;
    }

    setLoading(true);
    Client::instance() -> sendRequest("login", {{"username", username}, {"password", password}});
}

void AuthDialog::handleRegister(){
    username = regUsernameEdit -> text().trimmed();
    QString password = regpasswdEdit -> text();
    QString confirmPasswd = regConfirmpasswdEdit -> text();

    if(username.length() < 5){
        QMessageBox::warning(this, "Ошибка", "Логин должен содержать не менее 5 символов");
        return;
    }

    if(!correctPasswd(password)){
        return;
    }

    if(password != confirmPasswd){
        QMessageBox::warning(this, "Ошибка", "Пароли не совпадают");
        return;
    }

    if(!Client::instance() -> isConnected()){
        QMessageBox::critical(this, "Ошибка", "Нет подключения к серверу");
        checkConnection();
        return;
    }

    setLoading(true);
    Client::instance() -> sendRequest("register", {{"username", username}, {"password", password}});
}
void AuthDialog::handleBack(){
    usernameEdit -> setText(username);
    showScreen(LoginScreen);
}

void AuthDialog::handleServerResponse(const QJsonObject &response){
    setLoading(false);
    qDebug() << "Получен ответ от сервера:", QJsonDocument(response).toJson();
    if(!response.contains("operation") || !response.contains("status")){
        QMessageBox::critical(this, "Ошибка", "Некорректный формат ответа сервера");
        return;
    }
    QString operation = response["operation"].toString();
    QString status = response["status"].toString();

    if(operation == "checkUser"){
        if(status != "success" || !response.contains("exists")){
            QMessageBox::critical(this, "Ошибка", "Некорректный ответ на проверку пользователя");
            return;
        }

        if(response["exists"].toBool()){
            showScreen(PasswordScreen);
        }
        else{
            regUsernameEdit -> setText(username);
            showScreen(RegisterScreen);
        }
    }
    else if(operation == "login"){
        if(status != "success"){
            QString errorMsg = response.value("message").toString();
            QMessageBox::critical(this, "Ошибка", errorMsg.isEmpty() ? "Ошибка авторизации" : errorMsg);
            return;
        }

        if(!response.contains("user_id")){
            QMessageBox::critical(this, "Ошибка", "Сервер не вернул ID пользователя");
            return;
        }

        user_id = response["user_id"].toInt();
        if(user_id <= 0){
            QMessageBox::critical(this, "Ошибка", "Некорректный ID пользователя");
            return;
        }

        if(response.contains("username")){
            username = response["username"].toString();
        }

        emit accepted();
    }
    else if(operation == "register"){
        if(status != "success"){
            QString errorMsg = response.value("message").toString();
            QMessageBox::critical(this, "Ошибка", errorMsg.isEmpty() ? "Ошибка регистрации" : errorMsg);
            return;
        }

        if(!response.contains("user_id")){
            QMessageBox::critical(this, "Ошибка", "Сервер не вернул ID пользователя после регистрации");
            return;
        }

        user_id = response["user_id"].toInt();
        QMessageBox::information(this, "Успех", "Регистрация завершена успешно");

        usernameEdit -> setText(username);
        showScreen(LoginScreen);
    }
    else{
        QMessageBox::warning(this, "Предупреждение", "Неизвестная операция: " + operation);
    }
}

void AuthDialog::handleConnectionError(const QString &error){
    setLoading(false);
    QMessageBox::critical(this, "Ошибка соединения", error);
}

void AuthDialog::showScreen(Screen screen){
    stackedWidget -> setCurrentIndex(static_cast<int>(screen));
}

void AuthDialog::setLoading(bool loading){
    proceedButton -> setEnabled(!loading);
    loginButton -> setEnabled(!loading);
    registerButton -> setEnabled(!loading);
    backButton1 -> setEnabled(!loading);
    backButton2 -> setEnabled(!loading);
}

bool AuthDialog::correctPasswd(const QString &passwd){
    if(passwd.length() < 5 || passwd.length() > 16){
        QMessageBox::warning(this, "Ошибка", "Пароль должен быть от 5 до 16 символов");
        return false;
    }
    return true;
}

QString AuthDialog::getUsername() const{
    return username;
}

int AuthDialog::getUserId() const{
    return user_id;
}
