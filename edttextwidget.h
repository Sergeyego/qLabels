#ifndef EDTTEXTWIDGET_H
#define EDTTEXTWIDGET_H

#include <QWidget>
#include "items.h"
#include <QTextBlock>
#include <QTextCharFormat>

namespace Ui {
class EdtTextWidget;
}

class TextItem;

class EdtTextWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EdtTextWidget(TextItem *t, QWidget *parent = 0);
    ~EdtTextWidget();

private:
    Ui::EdtTextWidget *ui;
    TextItem *item;
    void setAlign(Qt::Alignment align);
    QTextCursor *cursor;

private slots:
    void changed();
    void setFont(QFont f);
    void setFontSize(int s);
    void setColor();
    void setAlignCenter();
    void setAlignLeft();
    void setAlignRight();
    void setAlignJustify();
    void setBold();
    void setItalic();
    void setUnder();
};

#endif // EDTTEXTWIDGET_H
