#include "edtlinewidget.h"
#include "ui_edtlinewidget.h"

EdtLineWidget::EdtLineWidget(QGraphicsItem *b, QWidget *parent) :
    item(b), QWidget(parent),
    ui(new Ui::EdtLineWidget)
{
    ui->setupUi(this);
    QPen p=getItemPen();

    ui->spinBoxWidth->setValue(p.width());
    ui->toolButtonColor->setPalette(QPalette(p.color()));

    ui->radioButtonSolid->setChecked(p.style()==Qt::SolidLine);
    ui->radioButtonDash->setChecked(p.style()==Qt::DashLine);
    ui->radioButtonDot->setChecked(p.style()==Qt::DotLine);
    ui->radioButtonDashDot->setChecked(p.style()==Qt::DashDotLine);
    ui->radioButtonDashDotDot->setChecked(p.style()==Qt::DashDotDotLine);

    connect(ui->spinBoxWidth,SIGNAL(valueChanged(int)),this,SLOT(setLineWidth(int)));
    connect(ui->toolButtonColor,SIGNAL(clicked()),this,SLOT(setColor()));

    connect(ui->radioButtonSolid,SIGNAL(clicked(bool)),this,SLOT(setLineStyle()));
    connect(ui->radioButtonDash,SIGNAL(clicked(bool)),this,SLOT(setLineStyle()));
    connect(ui->radioButtonDot,SIGNAL(clicked(bool)),this,SLOT(setLineStyle()));
    connect(ui->radioButtonDashDot,SIGNAL(clicked(bool)),this,SLOT(setLineStyle()));
    connect(ui->radioButtonDashDotDot,SIGNAL(clicked(bool)),this,SLOT(setLineStyle()));

}

EdtLineWidget::~EdtLineWidget()
{
    delete ui;
}

void EdtLineWidget::setItemPen(QPen p)
{
    RectItemBase *ri = qgraphicsitem_cast<RectItemBase *>(item);
    if (ri){
        ri->setPen(p);
    } else {
        LineItem *li = qgraphicsitem_cast<LineItem *>(item);
        if (li){
            li->setPen(p);
        }
    }
}

QPen EdtLineWidget::getItemPen()
{
    QPen p;
    RectItemBase *ri = qgraphicsitem_cast<RectItemBase *>(item);
    if (ri){
        p=ri->pen();
    } else {
        LineItem *li = qgraphicsitem_cast<LineItem *>(item);
        if (li){
            p=li->pen();
        }
    }
    return p;
}

void EdtLineWidget::setLineWidth(int w)
{
    QPen p=getItemPen();
    p.setWidth(w);
    setItemPen(p);
}

void EdtLineWidget::setColor()
{
    QPen p = getItemPen();
    QColor color = QColorDialog::getColor(p.color(), this);
    if (color.isValid()) {
        color.setAlpha(p.color().alpha());
        ui->toolButtonColor->setPalette(QPalette(color));
        p.setColor(color);
        setItemPen(p);
    }
}

void EdtLineWidget::setLineStyle()
{
    QPen p = getItemPen();
    Qt::PenStyle style;
    if (ui->radioButtonSolid->isChecked()){
        style=Qt::SolidLine;
    } else if (ui->radioButtonDash->isChecked()){
        style=Qt::DashLine;
    } else if (ui->radioButtonDot->isChecked()){
        style=Qt::DotLine;
    } else if (ui->radioButtonDashDot->isChecked()){
        style=Qt::DashDotLine;
    } else if (ui->radioButtonDashDotDot->isChecked()){
        style=Qt::DashDotDotLine;
    } else {
        style=Qt::SolidLine;
    }
    p.setStyle(style);
    setItemPen(p);
}
