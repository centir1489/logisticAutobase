#ifndef TABLEDIALOGDELETE_H
#define TABLEDIALOGDELETE_H

#include<QDialog>
#include<QLabel>
#include<QVBoxLayout>
#include<QPushButton>
#include<QMessageBox>

class TableDialogDelete : public QDialog{
    Q_OBJECT
public:
    explicit TableDialogDelete(const QString &tableName, QWidget* parent = nullptr) : QDialog(parent){
        setWindowTitle("Удаление перевозки");
        setWindowIcon(QIcon(":/resource/resources/del50.png"));
        setFixedSize(400, 150);
        setModal(true);

        QVBoxLayout* layout = new QVBoxLayout(this);

        QLabel* warningLabel = new QLabel(QString("Вы уверены, что хотите удалить перевозку \"%1\"?\n" "Все данные в таблице будут потеряны.").arg(tableName));
        warningLabel -> setWordWrap(true);
        warningLabel -> setAlignment(Qt::AlignCenter);
        layout -> addWidget(warningLabel);

        QHBoxLayout* buttonLayout = new QHBoxLayout();
        QPushButton* confirmButton = new QPushButton("Удалить");
        QPushButton* cancelButton = new QPushButton("Отмена");

        confirmButton -> setStyleSheet(
            "QPushButton {"
            "   background-color: #ff4444;"
            "   color: white;"
            "   padding: 8px 16px;"
            "   border-radius: 4px;"
            "}"
            "QPushButton:hover {"
            "   background-color: #cc0000;"
            "}");

        buttonLayout -> addStretch();
        buttonLayout -> addWidget(confirmButton);
        buttonLayout -> addWidget(cancelButton);
        layout -> addLayout(buttonLayout);

        connect(confirmButton, &QPushButton::clicked, this, &QDialog::accept);
        connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    }
};

#endif // TABLEDIALOGDELETE_H
