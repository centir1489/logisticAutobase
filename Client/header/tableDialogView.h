#ifndef TABLEDIALOGVIEW_H
#define TABLEDIALOGVIEW_H

#include<QDialog>
#include<QLabel>
#include<QPushButton>
#include<QVBoxLayout>
#include<QHBoxLayout>
#include<QTableWidget>
#include<QHeaderView>
#include<QIcon>

class TableDialogView : public QDialog {
    Q_OBJECT
public:
    TableDialogView(int tableId, const QString& tableName, const QString& departure, const QString& destination, QWidget *parent = nullptr) : QDialog(parent), tableId(tableId) {
        setWindowTitle("Информация о перевозке");
        setWindowIcon(QIcon(":/resource/resources/transportation32.png"));
        resize(600, 400);

        QLabel* titleLabel = new QLabel(QString("Таблица перевозок: %1").arg(tableName));
        titleLabel -> setAlignment(Qt::AlignCenter);
        QFont titleFont = titleLabel -> font();
        titleFont.setBold(true);
        titleFont.setPointSize(14);
        titleLabel -> setFont(titleFont);

        QTableWidget* tableWidget = new QTableWidget(3, 2);
        tableWidget -> setHorizontalHeaderLabels(QStringList() << "Параметр" << "Значение");
        tableWidget -> verticalHeader() -> setVisible(false);
        tableWidget -> horizontalHeader() -> setSectionResizeMode(QHeaderView::Stretch);
        tableWidget -> setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableWidget -> setAlternatingRowColors(true);

        // Фикс высоты строк
        for(int q = 0; q < tableWidget -> rowCount(); ++q){
            tableWidget -> setRowHeight(q, 40);
        }

        tableWidget -> setItem(0, 0, new QTableWidgetItem("Название таблицы"));
        tableWidget -> setItem(0, 1, new QTableWidgetItem(tableName));
        tableWidget -> setItem(1, 0, new QTableWidgetItem("Место отбытия"));
        tableWidget -> setItem(1, 1, new QTableWidgetItem(departure));
        tableWidget -> setItem(2, 0, new QTableWidgetItem("Место прибытия"));
        tableWidget -> setItem(2, 1, new QTableWidgetItem(destination));

        for(int q = 0; q < tableWidget -> rowCount(); ++q){
            for(int p = 0; p < tableWidget -> columnCount(); ++p){
                if(tableWidget -> item(q, p)){
                    tableWidget -> item(q, p) -> setTextAlignment(Qt::AlignCenter);
                }
            }
        }

        QPushButton* closeButton = new QPushButton("Закрыть");
        closeButton -> setFixedSize(100, 30);
        connect(closeButton, &QPushButton::clicked, this, &TableDialogView::close);

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

        mainLayout -> setContentsMargins(20, 20, 20, 20);
        setLayout(mainLayout);
    }

    int getTableId() const{
        return tableId;
    }

private:
    int tableId;
};

#endif // TABLEDIALOGVIEW_H
