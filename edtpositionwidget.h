#ifndef EDTPOSITIONWIDGET_H
#define EDTPOSITIONWIDGET_H

#include <QWidget>
#include <QDebug>
#include "items.h"
#include <QGraphicsScene>

namespace Ui {
class EdtPositionWidget;
}
class RectItemBase;
class ItemBase;

class EdtPositionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EdtPositionWidget(ItemBase *b, QWidget *parent = 0);
    ~EdtPositionWidget();

public slots:
    void setPosition(int x1, int y1, int x2, int y2);

private:
    Ui::EdtPositionWidget *ui;
    ItemBase *item;

private slots:
    void changed();
    void setNewPosition();
};

#endif // EDTPOSITIONWIDGET_H
