#ifndef TABLEDIALOGEXPORT_H
#define TABLEDIALOGEXPORT_H

#include<QDialog>
#include<QLabel>
#include<QVBoxLayout>
#include<QPushButton>
#include<QLineEdit>
#include<QMessageBox>
#include<QWidget>
#include<QComboBox>
#include<QFileDialog>
#include<QFileInfo>
#include<QFileInfo>
#include<QDir>
#include<QHBoxLayout>


class TableDialogExport : public QDialog{
    Q_OBJECT
public:
    TableDialogExport(const QString &tableName, QWidget* parent = nullptr) : QDialog(parent){
        setWindowTitle("Экспорт таблицы");
        setWindowIcon(QIcon(":/resource/resources/dexport32.png"));
        setFixedSize(500, 250);

        QVBoxLayout* layout = new QVBoxLayout(this);

        QLabel* titleLabel = new QLabel("Экспорт таблицы: " + tableName);
        titleLabel -> setAlignment(Qt::AlignCenter);
        layout -> addWidget(titleLabel);

        QLabel* formatLabel = new QLabel("Выберите формат экспорта:");
        layout -> addWidget(formatLabel);

        formatComboBox = new QComboBox(this);
        formatComboBox -> addItem("CSV (*.csv)");
        layout -> addWidget(formatComboBox);

        QLabel* pathLabel = new QLabel("Путь для экспорта:");
        layout -> addWidget(pathLabel);

        QHBoxLayout* pathLayout = new QHBoxLayout();
        pathEdit = new QLineEdit();
        pathEdit -> setPlaceholderText("Выберите путь для сохранения файла");
        pathLayout -> addWidget(pathEdit);

        QPushButton* browseButton = new QPushButton("Обзор...");
        pathLayout -> addWidget(browseButton);
        layout -> addLayout(pathLayout);

        connect(browseButton, &QPushButton::clicked, this, &TableDialogExport::onBrowse);

        QHBoxLayout* buttonLayout = new QHBoxLayout();
        QPushButton* exportButton = new QPushButton("Экспорт");
        QPushButton* cancelButton = new QPushButton("Отмена");

        buttonLayout -> addWidget(exportButton);
        buttonLayout -> addWidget(cancelButton);
        layout -> addLayout(buttonLayout);

        connect(exportButton, &QPushButton::clicked, this, &TableDialogExport::onExport);
        connect(cancelButton, &QPushButton::clicked, this, &TableDialogExport::reject);
    }

    QString getSelectedFormat() const {
        return formatComboBox -> currentText();
    }

    QString getExportPath() const {
        return exportPath;
    }

signals:
    void exportRequested(const QString& path, const QString& format);

private slots:
    void onBrowse(){
        QString filter = formatComboBox -> currentText();
        QString baseName = QFileInfo("Export_table").baseName().replace(" ", "_") + ".csv";
        QString path = QFileDialog::getSaveFileName(this, "Экспорт таблицы", QDir::homePath() + "/" + baseName, filter);

        if(!path.isEmpty()){
            if(!path.endsWith(".csv", Qt::CaseInsensitive)){
                path += ".csv";
            }
            pathEdit->setText(QFileInfo(path).absoluteFilePath());
        }
    }

    void onExport(){
        exportPath = pathEdit -> text().trimmed();

        if(exportPath.isEmpty()){
            QMessageBox::warning(this, "Ошибка", "Не указан путь для экспорта");
            return;
        }

        if(!exportPath.endsWith(".csv", Qt::CaseInsensitive)){
            exportPath += ".csv";
            pathEdit->setText(exportPath);
        }

        QFileInfo fileInfo(exportPath);
        if(!fileInfo.dir().exists()){
            if(!QDir().mkpath(fileInfo.dir().path())){
                QMessageBox::warning(this, "Ошибка", "Не удалось создать директорию");
                return;
            }
        }

        emit exportRequested(exportPath, formatComboBox -> currentText());
        accept();
    }

private:
    QComboBox* formatComboBox;
    QLineEdit* pathEdit;
    QString exportPath;
};

#endif // TABLEDIALOGEXPORT_H
