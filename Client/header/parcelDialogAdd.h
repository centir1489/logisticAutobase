#ifndef PARCELDIALOGADD_H
#define PARCELDIALOGADD_H

#include<QDialog>
#include<QLabel>
#include<QPushButton>
#include<QVBoxLayout>
#include<QHBoxLayout>
#include<QTableWidget>
#include<QHeaderView>
#include<QIcon>
#include<QDate>
#include<QLineEdit>
#include<QDateEdit>
#include<QComboBox>
#include<QDoubleSpinBox>
#include<QMessageBox>
#include<QVariantMap>
#include<QToolButton>
#include<QRandomGenerator>

class ParcelDialogAdd : public QDialog{
    Q_OBJECT

public:
    ParcelDialogAdd(int table_id, QWidget *parent = nullptr) : QDialog(parent), table_id(table_id){
        interface();
    }

    QString getNumber() const{
        return numberEdit -> text().trimmed();
    }
    QString getSender() const{
        return senderEdit -> text().trimmed();
    }
    QString getRecipient() const{
        return recipientEdit -> text().trimmed();
    }
    QDate getDateDispatch() const{
        return dateDispatchEdit -> date();
    }
    QDate getDeliveryDate() const{
        return deliveryDateEdit -> date();
    }
    QString getStatus() const{
        return statusCombo -> currentText();
    }
    double getWeight() const{
        return weightEdit -> value();
    }
    int getTableId() const{
        return table_id;
    }

signals:
    void addRequested(const QVariantMap &parcelData);

private slots:
    void onAddClicked(){
        if(!validateInput()){
            return;
        }
        QVariantMap parcelData;
        parcelData["number_parcel"] = getNumber();
        parcelData["sender"] = getSender();
        parcelData["recipient"] = getRecipient();
        parcelData["date_dispatch"] = getDateDispatch();
        parcelData["delivery_date"] = getDeliveryDate();
        parcelData["status_parcel"] = getStatus();
        parcelData["weight"] = getWeight();
        parcelData["table_id"] = getTableId();
        emit addRequested(parcelData);
        accept();
    }

    void generateParcelNumber(){
        QString lettrs = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        QString numbs = "0123456789";
        QString number_parcel;

        for(int q = 0; q < 4; ++q){
            int indx = QRandomGenerator::global() -> bounded(lettrs.length());
            number_parcel.append(lettrs[indx]);
        }
        for(int q = 0; q < 3; ++q){
            int indx = QRandomGenerator::global() -> bounded(numbs.length());
            number_parcel.append(numbs[indx]);
        }
        for(int q = 0; q < 2; ++q){
            int indx = QRandomGenerator::global() -> bounded(lettrs.length());
            number_parcel.append(lettrs[indx]);
        }

        int indx = QRandomGenerator::global() -> bounded(numbs.length());
        number_parcel.append(numbs[indx]);

        indx = QRandomGenerator::global() -> bounded(lettrs.length());
        number_parcel.append(lettrs[indx]);

        numberEdit -> setText(number_parcel);
    }

private:
    void interface(){
        setWindowTitle("Добавление новой посылки");
        setWindowIcon(QIcon(":/resource/resources/daddparcel20.png"));
        resize(600, 400);

        QLabel* titleLabel = new QLabel("Добавление новой посылки");
        titleLabel -> setAlignment(Qt::AlignCenter);
        QFont titleFont = titleLabel->font();
        titleFont.setBold(true);
        titleFont.setPointSize(14);
        titleLabel -> setFont(titleFont);

        QTableWidget* tableWidget = new QTableWidget(7, 2);
        tableWidget -> setHorizontalHeaderLabels(QStringList() << "Поле" << "Значение");
        tableWidget -> verticalHeader() -> setVisible(false);
        tableWidget -> horizontalHeader() -> setSectionResizeMode(QHeaderView::Stretch);

        // Фикс высоты строк
        for(int q = 0; q < tableWidget -> rowCount(); ++q){
            tableWidget -> setRowHeight(q, 40);
        }

        QTableWidgetItem* numberLabel = new QTableWidgetItem("Номер посылки");
        numberLabel -> setFlags(numberLabel -> flags() & ~Qt::ItemIsEditable);
        tableWidget -> setItem(0, 0, numberLabel);

        QWidget* numberWidget = new QWidget();
        QHBoxLayout* numberLayout = new QHBoxLayout(numberWidget);
        numberLayout -> setContentsMargins(0, 0, 0, 0);
        numberLayout -> setSpacing(2);

        numberEdit = new QLineEdit();
        numberEdit -> setMaxLength(100);
        numberLayout -> addWidget(numberEdit);

        QToolButton* generateButton = new QToolButton();
        generateButton -> setIcon(QIcon(":/resource/resources/dice30.png"));
        generateButton -> setToolTip("Сгенерировать номер посылки");
        generateButton -> setCursor(Qt::PointingHandCursor);
        generateButton -> setStyleSheet("QToolButton { border: none; padding: 2px; }");
        connect(generateButton, &QToolButton::clicked, this, &ParcelDialogAdd::generateParcelNumber);
        numberLayout -> addWidget(generateButton);
        tableWidget -> setCellWidget(0, 1, numberWidget);


        QTableWidgetItem* senderLabel = new QTableWidgetItem("Отправитель");
        senderLabel -> setFlags(senderLabel -> flags() & ~Qt::ItemIsEditable);
        tableWidget -> setItem(1, 0, senderLabel);
        senderEdit = new QLineEdit();
        senderEdit -> setMaxLength(100);
        tableWidget -> setCellWidget(1, 1, senderEdit);

        QTableWidgetItem* recipientLabel = new QTableWidgetItem("Получатель");
        recipientLabel -> setFlags(recipientLabel -> flags() & ~Qt::ItemIsEditable);
        tableWidget -> setItem(2, 0, recipientLabel);
        recipientEdit = new QLineEdit();
        recipientEdit -> setMaxLength(100);
        tableWidget -> setCellWidget(2, 1, recipientEdit);

        QTableWidgetItem* dispatchLabel = new QTableWidgetItem("Дата отправки");
        dispatchLabel -> setFlags(dispatchLabel -> flags() & ~Qt::ItemIsEditable);
        tableWidget -> setItem(3, 0, dispatchLabel);
        dateDispatchEdit = new QDateEdit(QDate::currentDate());
        dateDispatchEdit -> setCalendarPopup(true);
        dateDispatchEdit -> setDisplayFormat("dd.MM.yyyy");
        dateDispatchEdit -> setMinimumDate(QDate(2000, 1, 1));
        dateDispatchEdit -> setMaximumDate(QDate(2100, 12, 31));
        tableWidget -> setCellWidget(3, 1, dateDispatchEdit);

        QTableWidgetItem* deliveryLabel = new QTableWidgetItem("Дата доставки");
        deliveryLabel -> setFlags(deliveryLabel -> flags() & ~Qt::ItemIsEditable);
        tableWidget -> setItem(4, 0, deliveryLabel);
        deliveryDateEdit = new QDateEdit(QDate::currentDate().addDays(3));
        deliveryDateEdit -> setCalendarPopup(true);
        deliveryDateEdit -> setDisplayFormat("dd.MM.yyyy");
        deliveryDateEdit -> setMinimumDate(QDate(2000, 1, 1));
        deliveryDateEdit -> setMaximumDate(QDate(2100, 12, 31));
        tableWidget -> setCellWidget(4, 1, deliveryDateEdit);

        QTableWidgetItem* statusLabel = new QTableWidgetItem("Статус");
        statusLabel -> setFlags(statusLabel -> flags() & ~Qt::ItemIsEditable);
        tableWidget -> setItem(5, 0, statusLabel);
        statusCombo = new QComboBox();
        statusCombo -> addItems({"Готов к отправке", "Забрать с пункта", "Доставлен", "Отменен", "Отложена", "Возращена", "Задержена"});
        statusCombo -> setCurrentIndex(0);
        tableWidget -> setCellWidget(5, 1, statusCombo);

        QTableWidgetItem* weightLabel = new QTableWidgetItem("Вес (кг)");
        weightLabel -> setFlags(weightLabel->flags() & ~Qt::ItemIsEditable);
        tableWidget -> setItem(6, 0, weightLabel);
        weightEdit = new QDoubleSpinBox();
        weightEdit -> setRange(0.1, 100.0);
        weightEdit -> setValue(1.0);
        weightEdit -> setSingleStep(0.1);
        weightEdit -> setDecimals(2);
        tableWidget -> setCellWidget(6, 1, weightEdit);


        for(int q = 0; q < tableWidget->rowCount(); ++q){
            if(tableWidget -> item(q, 0)){
                tableWidget -> item(q, 0) -> setTextAlignment(Qt::AlignCenter);
            }
        }

        QPushButton* saveButton = new QPushButton("Создать");
        saveButton -> setFixedSize(100, 30);
        connect(saveButton, &QPushButton::clicked, this, &ParcelDialogAdd::onAddClicked);

        QPushButton* cancelButton = new QPushButton("Отмена");
        cancelButton -> setFixedSize(100, 30);
        connect(cancelButton, &QPushButton::clicked, this, &ParcelDialogAdd::reject);

        QHBoxLayout* buttonLayout = new QHBoxLayout;
        buttonLayout -> addStretch();
        buttonLayout -> addWidget(saveButton);
        buttonLayout -> addSpacing(20);
        buttonLayout -> addWidget(cancelButton);
        buttonLayout -> addStretch();

        QVBoxLayout* mainLayout = new QVBoxLayout;
        mainLayout -> addWidget(titleLabel);
        mainLayout -> addSpacing(15);
        mainLayout -> addWidget(tableWidget, 1);
        mainLayout -> addSpacing(15);
        mainLayout -> addLayout(buttonLayout);
        mainLayout -> setContentsMargins(20, 20, 20, 20);
        setLayout(mainLayout);
    }

    bool validateInput(){
        if(getNumber().isEmpty()){
            QMessageBox::warning(this, "Ошибка", "Поле 'Номер посылки' не может быть пустым!");
            numberEdit -> setFocus();
            return false;
        }

        if(getSender().isEmpty()){
            QMessageBox::warning(this, "Ошибка", "Поле 'Отправитель' не может быть пустым!");
            senderEdit -> setFocus();
            return false;
        }

        if(getRecipient().isEmpty()){
            QMessageBox::warning(this, "Ошибка", "Поле 'Получатель' не может быть пустым!");
            recipientEdit -> setFocus();
            return false;
        }

        QDate dispatchDate = getDateDispatch();
        QDate deliveryDate = getDeliveryDate();

        if(!dispatchDate.isValid()){
            QMessageBox::warning(this, "Ошибка", "Некорректная дата отправки!");
            dateDispatchEdit -> setFocus();
            return false;
        }

        if(!deliveryDate.isValid()){
            QMessageBox::warning(this, "Ошибка", "Некорректная дата доставки!");
            deliveryDateEdit -> setFocus();
            return false;
        }

        if(dispatchDate > deliveryDate){
            QMessageBox::warning(this, "Ошибка", "Дата отправки не может быть позже даты доставки!");
            dateDispatchEdit -> setFocus();
            return false;
        }

        return true;
    }

    int table_id;
    QLineEdit* numberEdit;
    QLineEdit* senderEdit;
    QLineEdit* recipientEdit;
    QDateEdit* dateDispatchEdit;
    QDateEdit* deliveryDateEdit;
    QComboBox* statusCombo;
    QDoubleSpinBox* weightEdit;
};

#endif // PARCELDIALOGADD_H
