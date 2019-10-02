#ifndef DIALOGNEWLABEL_H
#define DIALOGNEWLABEL_H

#include <QDialog>
#include <QMessageBox>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QDomElement>
#include "lib/papers.h"
#include "lib/tools.h"
#include "lib/templates.h"

namespace Ui {
class DialogNewLabel;
}

class DialogNewLabel : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNewLabel(QWidget *parent = 0);
    ~DialogNewLabel();
    bool isRotate();
    QDomElement getCurrentTemplate();

private slots:
    void cchanged(int index);
    void tchanged(int index);


private:
    Ui::DialogNewLabel *ui;
    Templates *templates;
    Papers *papers;
    QGraphicsScene *scene;
    QGraphicsRectItem * pr;
    QDomNodeList l1;
    QDomElement current_paper;
    QDomElement current_template;
    void draw_template();
    void clear();
};

#endif // DIALOGNEWLABEL_H
