#ifndef EDTBRUSHWIDGET_H
#define EDTBRUSHWIDGET_H

#include <QWidget>
#include "items.h"

namespace Ui {
class EdtBrushWidget;
}

class EdtBrushWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EdtBrushWidget(QGraphicsRectItem *i, QWidget *parent = 0);
    ~EdtBrushWidget();

private:
    Ui::EdtBrushWidget *ui;
    QGraphicsRectItem *item;

private slots:
    void setFillColor();
    void setOpacity(int o);
};

#endif // EDTBRUSHWIDGET_H
