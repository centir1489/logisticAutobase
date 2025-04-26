#ifndef PARCELCARD_H
#define PARCELCARD_H

#include<QPushButton>
#include<QWidgetAction>
#include<QLabel>
#include<QMenu>
#include<QVBoxLayout>
#include<QDebug>
#include<QDate>
#include<QRandomGenerator>
#include<QMouseEvent>

#include"mainwindow.h"

#include"parcelDialogOpen.h"
#include"parcelDialogEdit.h"
#include"parcelDialogMove.h"
#include"parcelDialogAdd.h"
#include"parcelDialogDelete.h"



class ParcelCard : public QPushButton{
    Q_OBJECT
public:
    explicit ParcelCard(const QString &color, const QString number_parcel, QWidget *parent);
    QString getnumber_parcel() const;
    void setColor(const QString &color);
    void setParcelData(const QVariantMap &data);

signals:
    void openRequested(const QVariantMap& parcelData);
    void editRequested(const QVariantMap &parcelData);
    void moveRequested(const QString& parcelNumber, int sourceTableId, const QString& targetTable);
    void deleteRequested(const QString &parcelNumber, int tableId);


private slots:
    void showContextMenu(const QPoint &pos);
    void handleOpenAction();
    void handleEditAction();
    void handleMoveAction();
    void handleAddAction();

protected:
    void mousePressEvent(QMouseEvent* event) override;

private:
    QString number_parcel;
    QLabel* titleLabel;
    QVariantMap parcelData;
};

#endif // PARCELCARD_H
