#ifndef TABLEDIALOGRENAME_H
#define TABLEDIALOGRENAME_H

#include<QDialog>
#include<QLabel>
#include<QVBoxLayout>
#include<QPushButton>
#include<QLineEdit>
#include<QMessageBox>

class TableDialogRename : public QDialog{
    Q_OBJECT
public:
    TableDialogRename(const QString& oldName, QWidget* parent = nullptr) : QDialog(parent){
        setWindowTitle("Переименовать таблицу");
        setWindowIcon(QIcon(":/resource/resources/rename50.png"));
        setFixedSize(400, 150);

        QVBoxLayout* layout = new QVBoxLayout(this);

        QLabel* titleLabel = new QLabel("Введите новое название таблицы");
        titleLabel -> setAlignment(Qt::AlignCenter);
        layout -> addWidget(titleLabel);

        tableNameInpt = new QLineEdit(this);
        tableNameInpt -> setPlaceholderText("Новое название таблицы");
        tableNameInpt -> setText(oldName);
        tableNameInpt -> selectAll();
        layout -> addWidget(tableNameInpt);

        // Кнопки
        QHBoxLayout* buttonLayout = new QHBoxLayout();
        QPushButton* confirmButton = new QPushButton("Подтвердить");
        QPushButton* cancelButton = new QPushButton("Отмена");

        buttonLayout -> addWidget(confirmButton);
        buttonLayout -> addWidget(cancelButton);
        layout -> addLayout(buttonLayout);

        connect(confirmButton, &QPushButton::clicked, this, &TableDialogRename::onConfirm);
        connect(cancelButton, &QPushButton::clicked, this, &TableDialogRename::reject);
    }

    QString getNewName() const{
        return tableNameInpt -> text();
    }

private slots:
    void onConfirm(){
        QString newName = tableNameInpt -> text();
        if(!newName.isEmpty()){
            accept();
        }
        else{
            QMessageBox::warning(this, "Ошибка", "Название таблицы не может быть пустым");
        }
    }

private:
    QLineEdit* tableNameInpt;
};

#endif // TABLEDIALOGRENAME_H
