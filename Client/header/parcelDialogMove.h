#ifndef PARCELDIALOGMOVE_H
#define PARCELDIALOGMOVE_H

#include<QDialog>
#include<QLabel>
#include<QPushButton>
#include<QVBoxLayout>
#include<QHBoxLayout>
#include<QComboBox>
#include<QMessageBox>
#include<QIcon>

class ParcelDialogMove : public QDialog{
    Q_OBJECT
public:
    explicit ParcelDialogMove(const QString& parcelNumber, const QStringList& availableTables, int activeTable_id, QWidget* parent = nullptr)
        : QDialog(parent), number_parcel(parcelNumber), activeTable_id(activeTable_id){
        setWindowTitle("Перемещение посылки");
        setWindowIcon(QIcon(":/resource/resources/dmove20.png"));
        setFixedSize(450, 220);
        setStyleSheet("QDialog { background-color: #f5f5f5; }");

        interface(availableTables);
        startConnection();
    }

signals:
    void moveRequested(const QString& parcelNumber, int sourceTableId, const QString& targetTable);

private:
    void interface(const QStringList& availableTables) {
        QLabel* titleLabel = new QLabel(QString("Перемещение посылки №%1").arg(number_parcel));
        titleLabel -> setAlignment(Qt::AlignCenter);
        QFont titleFont = titleLabel -> font();
        titleFont.setBold(true);
        titleFont.setPointSize(12);
        titleLabel -> setFont(titleFont);
        titleLabel -> setStyleSheet("color: #333;");

        QLabel* targetLabel = new QLabel("Целевая таблица:");
        targetLabel -> setStyleSheet("font-size: 11pt;");

        combo = new QComboBox();
        for(const QString& table : availableTables){
            if(table != QString::number(activeTable_id)){
                combo -> addItem(table);
            }
        }
        combo -> setCurrentIndex(-1);
        combo -> setStyleSheet(
            "QComboBox {"
            "   padding: 6px;"
            "   border: 1px solid #ccc;"
            "   border-radius: 4px;"
            "   min-width: 200px;"
            "}"
            "QComboBox::drop-down {"
            "   subcontrol-origin: padding;"
            "   subcontrol-position: top right;"
            "   width: 20px;"
            "   border-left-width: 1px;"
            "   border-left-color: #ccc;"
            "   border-left-style: solid;"
            "}"
            );

        moveButton = new QPushButton("Переместить");
        moveButton -> setFixedSize(120, 35);
        moveButton -> setStyleSheet(
            "QPushButton {"
            "   background-color: #91d494;"
            "   color: white;"
            "   border: none;"
            "   border-radius: 4px;"
            "   font-weight: bold;"
            "}"
            "QPushButton:hover {"
            "   background-color: #51d957;"
            "}"
            "QPushButton:disabled {"
            "   background-color: #cecece;"
            "}"
            );

        QPushButton* cancelButton = new QPushButton("Отмена");
        cancelButton -> setFixedSize(120, 35);
        cancelButton -> setStyleSheet(
            "QPushButton {"
            "   background-color: #eda8a8;"
            "   color: white;"
            "   border: none;"
            "   border-radius: 4px;"
            "   font-weight: bold;"
            "}"
            "QPushButton:hover {"
            "   background-color: #fc5858;"
            "}"
            );


        QVBoxLayout* mainLayout = new QVBoxLayout();
        QHBoxLayout* comboLayout = new QHBoxLayout();
        QHBoxLayout* buttonLayout = new QHBoxLayout();

        comboLayout -> addWidget(targetLabel);
        comboLayout -> addWidget(combo, 1);
        comboLayout -> setSpacing(15);

        buttonLayout -> addStretch();
        buttonLayout -> addWidget(moveButton);
        buttonLayout -> addSpacing(15);
        buttonLayout -> addWidget(cancelButton);
        buttonLayout -> addStretch();

        mainLayout -> addWidget(titleLabel);
        mainLayout -> addSpacing(20);
        mainLayout -> addLayout(comboLayout);
        mainLayout -> addSpacing(30);
        mainLayout -> addLayout(buttonLayout);
        mainLayout -> setContentsMargins(25, 20, 25, 20);

        setLayout(mainLayout);

        moveButton -> setEnabled(false);
    }

    void startConnection() {
        connect(moveButton, &QPushButton::clicked, this, &ParcelDialogMove::onMoveClicked);
        connect(combo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ParcelDialogMove::onTableSelected);

        QList<QPushButton*> buttons = findChildren<QPushButton*>();
        for(QPushButton* button : buttons){
            if (button->text() == "Отмена"){
                connect(button, &QPushButton::clicked, this, &ParcelDialogMove::reject);
                break;
            }
        }
    }

private slots:
    void onTableSelected(int index){
        moveButton -> setEnabled(index >= 0);
    }

    void onMoveClicked(){
        if (combo -> currentIndex() == -1) {
            QMessageBox::warning(this, "Ошибка", "Пожалуйста, выберите целевую таблицу");
            return;
        }

        emit moveRequested(number_parcel, activeTable_id, combo->currentText());
        accept();
    }

private:
    QString number_parcel;
    int activeTable_id;
    QComboBox* combo;
    QPushButton* moveButton;
};

#endif // PARCELDIALOGMOVE_H
