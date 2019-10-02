#include "edtimagewidget.h"
#include "ui_edtimagewidget.h"

EdtImageWidget::EdtImageWidget(ImageItem *b, QWidget *parent) :
    item(b), QWidget(parent),
    ui(new Ui::EdtImageWidget)
{
    ui->setupUi(this);
    connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(loadImage()));
}

EdtImageWidget::~EdtImageWidget()
{
    delete ui;
}

void EdtImageWidget::loadImage()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Select image..."),QDir::homePath(),
                                                    tr("Images (*.png *.jpg);;All Files (*)"));
    if (!fileName.isEmpty()) {
        item->setImage(QPixmap(fileName));
    }
}
