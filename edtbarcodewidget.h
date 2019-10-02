#ifndef EDTBARCODEWIDGET_H
#define EDTBARCODEWIDGET_H

#include <QWidget>
#include "items.h"

namespace Ui {
class EdtBarcodeWidget;
}

class BarcodeItem;

class EdtBarcodeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EdtBarcodeWidget(BarcodeItem *b, QWidget *parent = 0);
    ~EdtBarcodeWidget();
    void setBarcode(QString b);

private:
    Ui::EdtBarcodeWidget *ui;
    BarcodeItem *item;

private slots:
    void newBarcode(QString s);
};

#endif // EDTBARCODEWIDGET_H
