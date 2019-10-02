#include "edtbrushwidget.h"
#include "ui_edtbrushwidget.h"

EdtBrushWidget::EdtBrushWidget(QGraphicsRectItem *i, QWidget *parent) :
    item(i), QWidget(parent),
    ui(new Ui::EdtBrushWidget)
{
    ui->setupUi(this);
    QColor c(item->brush().color());
    c.setAlpha(255);
    ui->toolButtonFillColor->setPalette(QPalette(c));
    ui->spinBoxOpacity->setValue(item->brush().color().alpha());
    connect(ui->toolButtonFillColor,SIGNAL(clicked()),this,SLOT(setFillColor()));
    connect(ui->spinBoxOpacity,SIGNAL(valueChanged(int)),this,SLOT(setOpacity(int)));
}

EdtBrushWidget::~EdtBrushWidget()
{
    delete ui;
}

void EdtBrushWidget::setFillColor()
{
    QColor color = QColorDialog::getColor(item->brush().color(), this);
    if (color.isValid()) {
        ui->toolButtonFillColor->setPalette(QPalette(color));
        QBrush b = item->brush();
        color.setAlpha(b.color().alpha());
        b.setColor(color);
        item->setBrush(b);
    }
}

void EdtBrushWidget::setOpacity(int o)
{
    QBrush b = item->brush();
    QColor c = b.color();
    c.setAlpha(o);
    b.setColor(c);
    item->setBrush(b);
}
