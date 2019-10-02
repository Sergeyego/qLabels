#include "edtpositionwidget.h"
#include "ui_edtpositionwidget.h"

EdtPositionWidget::EdtPositionWidget(ItemBase *b, QWidget *parent) :
    item(b), QWidget(parent),
    ui(new Ui::EdtPositionWidget)
{
    ui->setupUi(this);
    connect(ui->spinBoxX1,SIGNAL(valueChanged(int)),this,SLOT(changed()));
    connect(ui->spinBoxY1,SIGNAL(valueChanged(int)),this,SLOT(changed()));
    connect(ui->spinBoxX2,SIGNAL(valueChanged(int)),this,SLOT(changed()));
    connect(ui->spinBoxY2,SIGNAL(valueChanged(int)),this,SLOT(changed()));
    connect(item->itScene,SIGNAL(changed(QList<QRectF>)),this,SLOT(setNewPosition()));
}

EdtPositionWidget::~EdtPositionWidget()
{
    delete ui;
}

void EdtPositionWidget::setPosition(int x1, int y1, int x2, int y2)
{
    ui->spinBoxX1->blockSignals(true);
    ui->spinBoxX1->setValue(x1);
    ui->spinBoxX1->blockSignals(false);

    ui->spinBoxY1->blockSignals(true);
    ui->spinBoxY1->setValue(y1);
    ui->spinBoxY1->blockSignals(false);

    ui->spinBoxX2->blockSignals(true);
    ui->spinBoxX2->setValue(x2);
    ui->spinBoxX2->blockSignals(false);

    ui->spinBoxY2->blockSignals(true);
    ui->spinBoxY2->setValue(y2);
    ui->spinBoxY2->blockSignals(false);
}

void EdtPositionWidget::changed()
{
    int x1=ui->spinBoxX1->value();
    int y1=ui->spinBoxY1->value();
    int x2=ui->spinBoxX2->value();
    int y2=ui->spinBoxY2->value();
    item->setPosition(x1,y1,x2,y2);
}

void EdtPositionWidget::setNewPosition()
{
    int x1(0),y1(0),x2(0),y2(0);
    item->getPosition(x1,y1,x2,y2);
    setPosition(x1,y1,x2,y2);
}
