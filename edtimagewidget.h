#ifndef EDTIMAGEWIDGET_H
#define EDTIMAGEWIDGET_H

#include <QWidget>
#include <QFileDialog>
#include "items.h"

namespace Ui {
class EdtImageWidget;
}

class ImageItem;

class EdtImageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EdtImageWidget(ImageItem *b, QWidget *parent = 0);
    ~EdtImageWidget();

private:
    Ui::EdtImageWidget *ui;
    ImageItem *item;

private slots:
    void loadImage();
};

#endif // EDTIMAGEWIDGET_H
