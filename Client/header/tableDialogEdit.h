#ifndef TABLEDIALOGEDIT_H
#define TABLEDIALOGEDIT_H

#include<QDialog>
#include<QLabel>
#include<QPushButton>
#include<QVBoxLayout>
#include<QHBoxLayout>
#include<QTableWidget>
#include<QHeaderView>
#include<QLineEdit>
#include<QMessageBox>

class TableDialogEdit : public QDialog{
    Q_OBJECT
public:
    TableDialogEdit(int tableId, const QString& name, const QString& departure, const QString& destination, QWidget *parent = nullptr) : QDialog(parent), tableId(tableId){
        setWindowTitle("Редактирование таблицы перевозок");
        setWindowIcon(QIcon(":/resource/resources/dedit-20.png"));
        resize(600, 400);

        QLabel* titleLabel = new QLabel(name);
        titleLabel -> setAlignment(Qt::AlignCenter);
        QFont titleFont = titleLabel -> font();
        titleFont.setBold(true);
        titleFont.setPointSize(14);
        titleLabel -> setFont(titleFont);

        QTableWidget* tableWidget = new QTableWidget(3, 2);
        tableWidget -> setHorizontalHeaderLabels(QStringList() << "Параметр" << "Значение");
        tableWidget -> verticalHeader() -> setVisible(false);
        tableWidget -> horizontalHeader() -> setSectionResizeMode(QHeaderView::Stretch);

        // Фикс высоты строк
        for(int q = 0; q < tableWidget -> rowCount(); ++q){
            tableWidget -> setRowHeight(q, 40);
        }

        QTableWidgetItem* nameLabelItem = new QTableWidgetItem("Название таблицы");
        nameLabelItem -> setFlags(nameLabelItem->flags() & ~Qt::ItemIsEditable);
        tableWidget -> setItem(0, 0, nameLabelItem);

        QLabel* nameLabel = new QLabel(name);
        nameLabel -> setAlignment(Qt::AlignCenter);
        nameLabel -> setStyleSheet("background-color: #f0f0f0;");
        tableWidget -> setCellWidget(0, 1, nameLabel);

        QTableWidgetItem* departureItem = new QTableWidgetItem("Место отбытия");
        departureItem -> setFlags(departureItem->flags() & ~Qt::ItemIsEditable);
        tableWidget -> setItem(1, 0, departureItem);

        departureEdit = new QLineEdit(departure);
        departureEdit -> setMaxLength(100);
        tableWidget -> setCellWidget(1, 1, departureEdit);

        QTableWidgetItem* destinationItem = new QTableWidgetItem("Место прибытия");
        destinationItem -> setFlags(destinationItem->flags() & ~Qt::ItemIsEditable);
        tableWidget -> setItem(2, 0, destinationItem);

        destinationEdit = new QLineEdit(destination);
        destinationEdit -> setMaxLength(100);
        tableWidget -> setCellWidget(2, 1, destinationEdit);

        for(int q = 0; q < tableWidget -> rowCount(); ++q){
            if(tableWidget -> item(q, 0)){
                tableWidget -> item(q, 0) -> setTextAlignment(Qt::AlignCenter);
            }
        }

        QPushButton* saveButton = new QPushButton("Сохранить");
        saveButton -> setFixedSize(100, 30);
        connect(saveButton, &QPushButton::clicked, this, &TableDialogEdit::onSaveClicked);

        QPushButton* cancelButton = new QPushButton("Отмена");
        cancelButton -> setFixedSize(100, 30);
        connect(cancelButton, &QPushButton::clicked, this, &TableDialogEdit::reject);

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

    int getTableId() const{
        return tableId;
    }
    QString getTableName() const{
        return name;
    }
    QString getDeparture() const{
        return departureEdit -> text().trimmed();
    }
    QString getDestination() const{
        return destinationEdit -> text().trimmed();
    }

signals:
    void updateRequested(int tableId, const QString& newDeparture, const QString& newDestination);

private slots:
    void onSaveClicked(){
        if(validateInput()){
            emit updateRequested(tableId, getDeparture(), getDestination());
            accept();
        }
    }

private:
    bool validateInput(){
        if(getDeparture().isEmpty()){
            QMessageBox::warning(this, "Ошибка", "Место отбытия не может быть пустым!");
            departureEdit -> setFocus();
            return false;
        }

        if(getDestination().isEmpty()){
            QMessageBox::warning(this, "Ошибка", "Место прибытия не может быть пустым!");
            destinationEdit -> setFocus();
            return false;
        }
        return true;
    }

    int tableId;
    QString name;
    QLineEdit* departureEdit;
    QLineEdit* destinationEdit;
};

#endif // TABLEDIALOGEDIT_H
