#ifndef PARCELDIALOGEDIT_H
#define PARCELDIALOGEDIT_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QIcon>
#include <QDate>
#include <QLineEdit>
#include <QDateEdit>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QMessageBox>

class ParcelDialogEdit : public QDialog{
    Q_OBJECT
public:
    ParcelDialogEdit(const QString& numberParcel, const QString& sender, const QString& recipient, const QDate& dateDispatch, const QDate& deliveryDate,
                     const QString& status, double weight, QWidget *parent = nullptr)
        : QDialog(parent), activeNumberParcel(numberParcel) {

        setWindowTitle("Редактирование перевозки");
        setWindowIcon(QIcon(":/resource/resources/dedit-20.png"));
        resize(600, 450);

        QLabel* titleLabel = new QLabel(QString("Редактирование посылки №%1").arg(numberParcel));
        titleLabel -> setAlignment(Qt::AlignCenter);
        QFont titleFont = titleLabel->font();
        titleFont.setBold(true);
        titleFont.setPointSize(14);
        titleLabel -> setFont(titleFont);

        QTableWidget* tableWidget = new QTableWidget(6, 2);
        tableWidget -> setHorizontalHeaderLabels(QStringList() << "Поле" << "Значение");
        tableWidget -> verticalHeader() -> setVisible(false);
        tableWidget -> horizontalHeader() -> setSectionResizeMode(QHeaderView::Stretch);

        for(int q = 0; q < tableWidget->rowCount(); ++q){
            tableWidget->setRowHeight(q, 40);
        }

        QTableWidgetItem* senderLabel = new QTableWidgetItem("Отправитель");
        senderLabel -> setFlags(senderLabel -> flags() & ~Qt::ItemIsEditable);
        tableWidget -> setItem(0, 0, senderLabel);
        senderEdit = new QLineEdit(sender);
        senderEdit -> setMaxLength(100);
        tableWidget -> setCellWidget(0, 1, senderEdit);

        QTableWidgetItem* recipientLabel = new QTableWidgetItem("Получатель");
        recipientLabel -> setFlags(recipientLabel -> flags() & ~Qt::ItemIsEditable);
        tableWidget -> setItem(1, 0, recipientLabel);
        recipientEdit = new QLineEdit(recipient);
        recipientEdit -> setMaxLength(100);
        tableWidget -> setCellWidget(1, 1, recipientEdit);

        QTableWidgetItem* dispatchLabel = new QTableWidgetItem("Дата отправки");
        dispatchLabel -> setFlags(dispatchLabel -> flags() & ~Qt::ItemIsEditable);
        tableWidget -> setItem(2, 0, dispatchLabel);
        dateDispatchEdit = new QDateEdit(dateDispatch);
        dateDispatchEdit -> setCalendarPopup(true);
        dateDispatchEdit -> setDisplayFormat("dd.MM.yyyy");
        tableWidget -> setCellWidget(2, 1, dateDispatchEdit);

        QTableWidgetItem* deliveryLabel = new QTableWidgetItem("Дата доставки");
        deliveryLabel -> setFlags(deliveryLabel -> flags() & ~Qt::ItemIsEditable);
        tableWidget -> setItem(3, 0, deliveryLabel);
        deliveryDateEdit = new QDateEdit(deliveryDate);
        deliveryDateEdit -> setCalendarPopup(true);
        deliveryDateEdit -> setDisplayFormat("dd.MM.yyyy");
        tableWidget -> setCellWidget(3, 1, deliveryDateEdit);

        QTableWidgetItem* statusLabel = new QTableWidgetItem("Статус");
        statusLabel -> setFlags(statusLabel -> flags() & ~Qt::ItemIsEditable);
        tableWidget -> setItem(4, 0, statusLabel);
        statusCombo = new QComboBox();
        statusCombo -> addItems({"Готов к отправке", "Забрать с пункта", "Доставлен", "Отменен", "Отложена", "Возращена", "Задержена"});
        statusCombo -> setCurrentText(status);
        tableWidget -> setCellWidget(4, 1, statusCombo);

        QTableWidgetItem* weightLabel = new QTableWidgetItem("Вес (кг)");
        weightLabel -> setFlags(weightLabel -> flags() & ~Qt::ItemIsEditable);
        tableWidget -> setItem(5, 0, weightLabel);
        weightEdit = new QDoubleSpinBox();
        weightEdit -> setRange(0.1, 100.0);
        weightEdit -> setValue(weight);
        weightEdit -> setSingleStep(0.1);
        weightEdit -> setDecimals(2);
        tableWidget -> setCellWidget(5, 1, weightEdit);


        for(int q = 0; q < tableWidget -> rowCount(); ++q){
            if(tableWidget -> item(q, 0)) {
                tableWidget -> item(q, 0) -> setTextAlignment(Qt::AlignCenter);
            }
        }

        QPushButton* saveButton = new QPushButton("Сохранить");
        saveButton -> setFixedSize(100, 30);
        connect(saveButton, &QPushButton::clicked, this, &ParcelDialogEdit::onSaveClicked);

        QPushButton* cancelButton = new QPushButton("Отмена");
        cancelButton -> setFixedSize(100, 30);
        connect(cancelButton, &QPushButton::clicked, this, &ParcelDialogEdit::reject);

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

    QString getNumberParcel() const{
        return activeNumberParcel;
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

signals:
    void saveRequested(const QVariantMap &updatedData);

private slots:
    void onSaveClicked() {
        if(getSender().isEmpty()) {
            QMessageBox::warning(this, "Ошибка", "Поле 'Отправитель' не может быть пустым!");
            senderEdit -> setFocus();
            return;
        }

        if(getRecipient().isEmpty()) {
            QMessageBox::warning(this, "Ошибка", "Поле 'Получатель' не может быть пустым!");
            recipientEdit -> setFocus();
            return;
        }

        QDate dispatchDate = getDateDispatch();
        QDate deliveryDate = getDeliveryDate();

        if(dispatchDate > deliveryDate){
            QMessageBox::warning(this, "Ошибка", "Дата отправки не может быть позже даты доставки!");
            dateDispatchEdit -> setFocus();
            return;
        }

        QVariantMap updatedData;
        updatedData["number_parcel"] = activeNumberParcel;
        updatedData["sender"] = getSender();
        updatedData["recipient"] = getRecipient();
        updatedData["date_dispatch"] = getDateDispatch();
        updatedData["delivery_date"] = getDeliveryDate();
        updatedData["status_parcel"] = getStatus();
        updatedData["weight"] = getWeight();

        emit saveRequested(updatedData);
        accept();
    }

private:
    QString activeNumberParcel;
    QLineEdit* senderEdit;
    QLineEdit* recipientEdit;
    QDateEdit* dateDispatchEdit;
    QDateEdit* deliveryDateEdit;
    QComboBox* statusCombo;
    QDoubleSpinBox* weightEdit;
};

#endif // PARCELDIALOGEDIT_H
