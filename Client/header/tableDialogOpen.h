#ifndef TABLEDIALOGOPEN_H
#define TABLEDIALOGOPEN_H

#include<QDialog>
#include<QLabel>
#include<QPushButton>
#include<QVBoxLayout>
#include<QHBoxLayout>
#include<QTableWidget>
#include<QHeaderView>
#include<QIcon>
#include<QJsonArray>
#include<QJsonObject>

class TableDialogOpen : public QDialog{
    Q_OBJECT
public:
    explicit TableDialogOpen(const QString &tableName, const QJsonArray &parcels, QWidget *parent = nullptr) : QDialog(parent){
        setWindowTitle("Прсмотр листа перевозки");
        setWindowIcon(QIcon(":/resource/resources/table50.png"));
        resize(1000, 600);

        QLabel* titleLabel = new QLabel(tableName);
        titleLabel->setAlignment(Qt::AlignCenter);
        QFont titleFont = titleLabel->font();
        titleFont.setBold(true);
        titleFont.setPointSize(14);
        titleLabel -> setFont(titleFont);

        QTableWidget* tableWidget = new QTableWidget(this);
        tableWidget -> setColumnCount(7);
        tableWidget -> setHorizontalHeaderLabels({
            "Номер посылки", "Отправитель", "Получатель",
            "Дата отправки", "Дата доставки", "Статус", "Вес (кг)"
        });
        tableWidget -> verticalHeader() -> setVisible(false);
        tableWidget -> horizontalHeader() -> setSectionResizeMode(QHeaderView::Stretch);
        tableWidget -> setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableWidget -> setAlternatingRowColors(true);


        tableWidget -> setRowCount(parcels.size());
        for(int q = 0; q < parcels.size(); ++q){
            QJsonObject parcel = parcels[q].toObject();

            tableWidget -> setItem(q, 0, new QTableWidgetItem(parcel["number_parcel"].toString()));
            tableWidget -> setItem(q, 1, new QTableWidgetItem(parcel["sender"].toString()));
            tableWidget -> setItem(q, 2, new QTableWidgetItem(parcel["recipient"].toString()));
            tableWidget -> setItem(q, 3, new QTableWidgetItem(parcel["date_dispatch"].toString()));
            tableWidget -> setItem(q, 4, new QTableWidgetItem(parcel["delivery_date"].toString()));
            tableWidget -> setItem(q, 5, new QTableWidgetItem(parcel["status_parcel"].toString()));
            double weight = parcel["weight"].toDouble();
            QString weightStr = QLocale().toString(weight, 'f', 2);
            tableWidget->setItem(q, 6, new QTableWidgetItem(weightStr + " кг"));

            // Центрируем текст
            for(int col = 0; col < tableWidget->columnCount(); ++col) {
                tableWidget->item(q, col)->setTextAlignment(Qt::AlignCenter);
            }
        }

        // Фиксируем высоту строк
        for(int row = 0; row < tableWidget->rowCount(); ++row) {
            tableWidget->setRowHeight(row, 30);
        }

        QPushButton* closeButton = new QPushButton("Закрыть");
        closeButton -> setFixedSize(100, 30);
        connect(closeButton, &QPushButton::clicked, this, &TableDialogOpen::close);

        QHBoxLayout *buttonLayout = new QHBoxLayout;
        buttonLayout -> addStretch();
        buttonLayout -> addWidget(closeButton);
        buttonLayout -> addStretch();

        QVBoxLayout *mainLayout = new QVBoxLayout;
        mainLayout -> addWidget(titleLabel);
        mainLayout -> addSpacing(15);
        mainLayout -> addWidget(tableWidget, 1);
        mainLayout -> addSpacing(15);
        mainLayout -> addLayout(buttonLayout);
        mainLayout -> setContentsMargins(20, 20, 20, 20);
        setLayout(mainLayout);
    }
};

#endif // TABLEDIALOGOPEN_H
