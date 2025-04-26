#ifndef PARCELDIALOGOPEN_H
#define PARCELDIALOGOPEN_H

#include<QDialog>
#include<QLabel>
#include<QPushButton>
#include<QVBoxLayout>
#include<QHBoxLayout>
#include<QTableWidget>
#include<QHeaderView>
#include<QIcon>
#include<QDate>
#include<QSpacerItem>

class ParcelDialogOpen : public QDialog {
    Q_OBJECT

public:
    ParcelDialogOpen(const QString& numberParcel, const QString& sender, const QString& recipient, const QDate& dateDispatch, const QDate& deliveryDate, const QString& status, double weight, QWidget *parent = nullptr)
        : QDialog(parent){
        setWindowTitle("Информация о перевозке");
        setWindowIcon(QIcon(":/resource/resources/parcel50.png"));
        resize(600, 400);

        QLabel* titleLabel = new QLabel(QString("Посылка №%1").arg(numberParcel));
        titleLabel -> setAlignment(Qt::AlignCenter);
        QFont titleFont = titleLabel -> font();
        titleFont.setBold(true);
        titleFont.setPointSize(14);
        titleLabel -> setFont(titleFont);

        // отображение
        QTableWidget* tableWidget = new QTableWidget(6, 2);
        tableWidget -> setHorizontalHeaderLabels(QStringList() << "Параметр" << "Значение");
        tableWidget -> verticalHeader() -> setVisible(false);
        tableWidget -> horizontalHeader() -> setSectionResizeMode(QHeaderView::Stretch);
        tableWidget -> setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableWidget -> setAlternatingRowColors(true);

        //фикс строк
        for(int q = 0; q < tableWidget -> rowCount(); ++q){
            tableWidget->setRowHeight(q, 30);
        }

        // таблица
        tableWidget -> setItem(0, 0, new QTableWidgetItem("Отправитель"));
        tableWidget -> setItem(0, 1, new QTableWidgetItem(sender));
        tableWidget -> setItem(1, 0, new QTableWidgetItem("Получатель"));
        tableWidget -> setItem(1, 1, new QTableWidgetItem(recipient));
        tableWidget -> setItem(2, 0, new QTableWidgetItem("Дата отправки"));
        tableWidget -> setItem(2, 1, new QTableWidgetItem(dateDispatch.toString("dd.MM.yyyy")));
        tableWidget -> setItem(3, 0, new QTableWidgetItem("Дата доставки"));
        tableWidget -> setItem(3, 1, new QTableWidgetItem(deliveryDate.toString("dd.MM.yyyy")));
        tableWidget -> setItem(4, 0, new QTableWidgetItem("Статус"));
        tableWidget -> setItem(4, 1, new QTableWidgetItem(status));
        tableWidget -> setItem(5, 0, new QTableWidgetItem("Вес"));
        tableWidget -> setItem(5, 1, new QTableWidgetItem(QString("%1 кг").arg(weight)));

        for(int q = 0; q < tableWidget->rowCount(); ++q){
            for(int p = 0; p < tableWidget->columnCount(); ++p){
                if(tableWidget -> item(q, p)){
                    tableWidget->item(q, p)->setTextAlignment(Qt::AlignCenter);
                }
            }
        }

        QPushButton* editButton = new QPushButton();
        QIcon editIcon(":/resource/resources/redaction50.png");

        editButton -> setIcon(editIcon);
        editButton -> setFixedSize(50, 50);
        editButton -> setToolTip("Редактировать");
        connect(editButton, &QPushButton::clicked, this, &ParcelDialogOpen::onEditClicked);

        QPushButton* closeButton = new QPushButton("Закрыть");
        closeButton -> setFixedSize(100, 30);
        connect(closeButton, &QPushButton::clicked, this, &ParcelDialogOpen::close);


        QHBoxLayout* buttonLayout = new QHBoxLayout;
        buttonLayout -> addStretch();
        buttonLayout -> addWidget(closeButton);
        buttonLayout -> addStretch();

        QVBoxLayout* mainLayout = new QVBoxLayout;
        mainLayout -> addWidget(titleLabel);
        mainLayout -> addSpacing(15);
        mainLayout -> addWidget(tableWidget, 1);
        mainLayout -> addSpacing(15);
        mainLayout -> addLayout(buttonLayout);
        mainLayout -> addWidget(editButton, 0, Qt::AlignRight);

        // отступы
        mainLayout -> setContentsMargins(20, 20, 20, 20);
        setLayout(mainLayout);
    }

signals:
    void editRequested();

private slots:
    void onEditClicked(){
        emit editRequested();
    }
};

#endif // PARCELDIALOGOPEN_H
