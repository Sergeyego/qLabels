#include "edtbarcodewidget.h"
#include "ui_edtbarcodewidget.h"

EdtBarcodeWidget::EdtBarcodeWidget(BarcodeItem *b, QWidget *parent) :
    item(b), QWidget(parent),
    ui(new Ui::EdtBarcodeWidget)
{
    ui->setupUi(this);
    connect(ui->lineEdit,SIGNAL(textChanged(QString)),this,SLOT(newBarcode(QString)));
}

EdtBarcodeWidget::~EdtBarcodeWidget()
{
    delete ui;
}

void EdtBarcodeWidget::setBarcode(QString b)
{
    ui->lineEdit->setText(b);
}

void EdtBarcodeWidget::newBarcode(QString s)
{
    item->setBarcode(s);
}
