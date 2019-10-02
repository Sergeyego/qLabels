#ifndef EDTLINEWIDGET_H
#define EDTLINEWIDGET_H

#include <QWidget>
#include "items.h"
#include <QColorDialog>

namespace Ui {
class EdtLineWidget;
}

class QGraphicsItem;
class LineItem;

class EdtLineWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EdtLineWidget(QGraphicsItem *b, QWidget *parent = 0);
    ~EdtLineWidget();

private:
    Ui::EdtLineWidget *ui;
    QGraphicsItem *item;
    void setItemPen(QPen p);
    QPen getItemPen();

private slots:
    void setLineWidth(int w);
    void setColor();
    void setLineStyle();

};

#endif // EDTLINEWIDGET_H
