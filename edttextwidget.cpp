#include "edttextwidget.h"
#include "ui_edttextwidget.h"

EdtTextWidget::EdtTextWidget(TextItem *t, QWidget *parent) :
    item(t), QWidget(parent),
    ui(new Ui::EdtTextWidget)
{
    ui->setupUi(this);
    ui->textEdit->setDocument(item->doc);
    cursor = new QTextCursor(item->doc);

    ui->fontComboBox->setCurrentFont(cursor->charFormat().font());
    ui->spinBox->setValue(cursor->charFormat().font().pointSize());
    QColor c=cursor->charFormat().foreground().color();
    ui->toolButtonColor->setPalette(QPalette(c));

    Qt::Alignment align = cursor->blockFormat().alignment();
    ui->toolButtonCenter->setChecked(align==Qt::AlignCenter);
    ui->toolButtonLeft->setChecked(align==Qt::AlignLeft);
    ui->toolButtonRight->setChecked(align==Qt::AlignRight);
    ui->toolButtonJustify->setChecked(align==Qt::AlignJustify);

    ui->toolButtonBold->setChecked(cursor->charFormat().font().bold());
    ui->toolButtonItalic->setChecked(cursor->charFormat().font().italic());
    ui->toolButtonUnder->setChecked(cursor->charFormat().font().underline());

    connect(ui->textEdit->document(),SIGNAL(contentsChanged()),this,SLOT(changed()));
    connect(ui->fontComboBox,SIGNAL(currentFontChanged(QFont)),this,SLOT(setFont(QFont)));
    connect(ui->spinBox,SIGNAL(valueChanged(int)),this,SLOT(setFontSize(int)));
    connect(ui->toolButtonColor,SIGNAL(clicked()),this,SLOT(setColor()));
    connect(ui->toolButtonCenter,SIGNAL(clicked()),this,SLOT(setAlignCenter()));
    connect(ui->toolButtonLeft,SIGNAL(clicked()),this,SLOT(setAlignLeft()));
    connect(ui->toolButtonRight,SIGNAL(clicked()),this,SLOT(setAlignRight()));
    connect(ui->toolButtonJustify,SIGNAL(clicked()),this,SLOT(setAlignJustify()));
    connect(ui->toolButtonBold,SIGNAL(clicked()),this,SLOT(setBold()));
    connect(ui->toolButtonItalic,SIGNAL(clicked()),this,SLOT(setItalic()));
    connect(ui->toolButtonUnder,SIGNAL(clicked()),this,SLOT(setUnder()));
}

EdtTextWidget::~EdtTextWidget()
{
    delete ui;
}

void EdtTextWidget::setAlign(Qt::Alignment align)
{
    ui->toolButtonCenter->setChecked(align==Qt::AlignCenter);
    ui->toolButtonLeft->setChecked(align==Qt::AlignLeft);
    ui->toolButtonRight->setChecked(align==Qt::AlignRight);
    ui->toolButtonJustify->setChecked(align==Qt::AlignJustify);
    cursor->select(QTextCursor::Document);
    QTextBlockFormat bf=cursor->blockFormat();
    bf.setAlignment(align);
    cursor->mergeBlockFormat(bf);
}

void EdtTextWidget::changed()
{
    item->update();
}

void EdtTextWidget::setFont(QFont f)
{
    cursor->select(QTextCursor::Document);
    QTextCharFormat format = cursor->charFormat();
    f.setPointSize(ui->spinBox->value());
    format.setFont(f);
    cursor->mergeCharFormat(format);
    item->update();
}

void EdtTextWidget::setFontSize(int s)
{
    QFont f=cursor->charFormat().font();
    f.setPointSize(s);
    setFont(f);
}

void EdtTextWidget::setColor()
{
    QColor color = QColorDialog::getColor(cursor->charFormat().foreground().color(), this);
    if (color.isValid()){
        cursor->select(QTextCursor::Document);
        QTextCharFormat format=cursor->charFormat();
        format.setForeground(QBrush(color));
        cursor->mergeCharFormat(format);
        ui->toolButtonColor->setPalette(QPalette(color));
    }
}

void EdtTextWidget::setAlignCenter()
{
    setAlign(Qt::AlignCenter);
}

void EdtTextWidget::setAlignLeft()
{
    setAlign(Qt::AlignLeft);
}

void EdtTextWidget::setAlignRight()
{
    setAlign(Qt::AlignRight);
}

void EdtTextWidget::setAlignJustify()
{
    setAlign(Qt::AlignJustify);
}

void EdtTextWidget::setBold()
{
    QFont f=cursor->charFormat().font();
    f.setBold(ui->toolButtonBold->isChecked());
    setFont(f);
}

void EdtTextWidget::setItalic()
{
    QFont f=cursor->charFormat().font();
    f.setItalic(ui->toolButtonItalic->isChecked());
    setFont(f);
}

void EdtTextWidget::setUnder()
{
    QFont f=cursor->charFormat().font();
    f.setUnderline(ui->toolButtonUnder->isChecked());
    setFont(f);
}
