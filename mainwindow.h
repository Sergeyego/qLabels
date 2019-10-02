#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrintPreviewDialog>
#include <QSettings>
#include <QMap>
#include <QListWidgetItem>
#include "items.h"
#include "dialognewlabel.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void loadFile(QString filename);

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QDomElement current_template;
    bool is_rotated;
    qreal currentScale;
    void printPage(double scale, QPainter *pp);
    void drawTemplate();
    void clear();
    void loadSettings();
    void saveSettings();
    QMap <QListWidgetItem *,QGraphicsItem *> map;
    QMap <QString, QPrinter::PaperSize> paperSize;
    void addItem(QGraphicsItem *i);

private slots:
    void addRect();
    void addBarcode();
    void addImage();
    void addText();
    void addEllipse();
    void addLine();
    void zoomIn();
    void zoomOut();
    void zoomFit();
    void setTabs();
    void delItem();
    void newLbl();
    void saveLbl();
    void openLbl();
    void setAntiAliasing(bool b);
    void printPreview();
    void print();
    void exportPdf();
    void printLbl(QPrinter *p);
    void setLock(bool l);
    void updList();
    void selectItem(int p);
    void blockItem(QListWidgetItem *i);

};

#endif // MAINWINDOW_H
