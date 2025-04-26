#ifndef TABLEDIALOGADD_H
#define TABLEDIALOGADD_H

#include<QDialog>
#include<QLabel>
#include<QVBoxLayout>
#include<QPushButton>
#include<QLineEdit>
#include<QMessageBox>
#include<QFileDialog>
#include<QTableWidget>
#include<QHeaderView>
#include<QIcon>

class TableDialogAdd : public QDialog{
    Q_OBJECT
public:
    TableDialogAdd(QWidget* parent = nullptr) : QDialog(parent){
        interface();
    }

    QString getTableName() const{
        return tableNameEdit -> text().trimmed();
    }

    QString getDepartureLocation() const{
        return departureEdit -> text().trimmed();
    }

    QString getDestinationLocation() const{
        return destinationEdit -> text().trimmed();
    }

    QString getImportFilePath() const{
        return importFilePath;
    }

signals:
    void tableCreated(const QString& tableName, const QString& departure, const QString& destination);
    void tableImported(const QString& filePath);

private slots:
    void createTable(){
        if(!validateInput()){
            return;
        }
        emit tableCreated(getTableName(), getDepartureLocation(), getDestinationLocation());
        accept();
    }

    void importFromCSV(){
        importFilePath = QFileDialog::getOpenFileName(this, "Выберите CSV файл", "", "CSV Files (*.csv);;All Files (*)");

        if(importFilePath.isEmpty()){
            return;
        }

        emit tableImported(importFilePath);
        accept();
    }

private:
    void interface(){
        setWindowTitle("Создать перевозку");
        setWindowIcon(QIcon(":/resource/resources/daddparcel20.png"));
        resize(600, 400);

        QLabel* titleLabel = new QLabel("Добавление новой таблицы перевозок");
        titleLabel -> setAlignment(Qt::AlignCenter);
        QFont titleFont = titleLabel->font();
        titleFont.setBold(true);
        titleFont.setPointSize(14);
        titleLabel -> setFont(titleFont);

        QTableWidget* tableWidget = new QTableWidget(3, 2);
        tableWidget -> setHorizontalHeaderLabels(QStringList() << "Поле" << "Значение");
        tableWidget -> verticalHeader() -> setVisible(false);
        tableWidget -> horizontalHeader() -> setSectionResizeMode(QHeaderView::Stretch);

        // Фикс высоты строк
        for(int q = 0; q < tableWidget->rowCount(); ++q){
            tableWidget -> setRowHeight(q, 40);
        }

        tableWidget -> setItem(0, 0, new QTableWidgetItem("Название перевозки"));
        tableNameEdit = new QLineEdit();
        tableNameEdit -> setPlaceholderText("Введите название таблицы");
        tableWidget -> setCellWidget(0, 1, tableNameEdit);

        tableWidget -> setItem(1, 0, new QTableWidgetItem("Место отбытия"));
        departureEdit = new QLineEdit();
        departureEdit -> setPlaceholderText("Введите место отправки");
        tableWidget -> setCellWidget(1, 1, departureEdit);

        tableWidget -> setItem(2, 0, new QTableWidgetItem("Место прибытия"));
        destinationEdit = new QLineEdit();
        destinationEdit -> setPlaceholderText("Введите место назначения");
        tableWidget -> setCellWidget(2, 1, destinationEdit);

        for(int q = 0; q < tableWidget -> rowCount(); ++q){
            if(tableWidget -> item(q, 0)){
                tableWidget -> item(q, 0) -> setTextAlignment(Qt::AlignCenter);
            }
        }

        QPushButton* createButton = new QPushButton("Создать");
        createButton -> setFixedSize(100, 30);
        connect(createButton, &QPushButton::clicked, this, &TableDialogAdd::createTable);

        QPushButton* importButton = new QPushButton("Загрузить");
        importButton -> setFixedSize(100, 30);
        connect(importButton, &QPushButton::clicked, this, &TableDialogAdd::importFromCSV);

        QPushButton* cancelButton = new QPushButton("Отмена");
        cancelButton -> setFixedSize(100, 30);
        connect(cancelButton, &QPushButton::clicked, this, &TableDialogAdd::reject);

        QHBoxLayout* buttonLayout = new QHBoxLayout;
        buttonLayout -> addStretch();
        buttonLayout -> addWidget(createButton);
        buttonLayout -> addSpacing(20);
        buttonLayout -> addWidget(importButton);
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
        if(getTableName().isEmpty()){
            QMessageBox::warning(this, "Ошибка", "Поле 'Название перевозки' не может быть пустым!");
            tableNameEdit -> setFocus();
            return false;
        }
        if(getDepartureLocation().isEmpty()){
            QMessageBox::warning(this, "Ошибка", "Поле 'Место отбытия' не может быть пустым!");
            departureEdit -> setFocus();
            return false;
        }
        if(getDestinationLocation().isEmpty()){
            QMessageBox::warning(this, "Ошибка", "Поле 'Место прибытия' не может быть пустым!");
            destinationEdit -> setFocus();
            return false;
        }
        return true;
    }

    QLineEdit* tableNameEdit;
    QLineEdit* departureEdit;
    QLineEdit* destinationEdit;
    QString importFilePath;
};

#endif // TABLEDIALOGADD_H
