#ifndef ITEMS_H
#define ITEMS_H

#include <QObject>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QPen>
#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <QDebug>
#include <QTextLayout>
#include <QTextDocument>
#include <QPlainTextDocumentLayout>
#include <QLineEdit>
#include "edtpositionwidget.h"
#include "lib/ean13.h"
#include "edtlinewidget.h"
#include "edtbarcodewidget.h"
#include "edtimagewidget.h"
#include "edttextwidget.h"
#include "edtbrushwidget.h"
#include <QCoreApplication>
#include <QDomElement>
#include <QBuffer>


enum resize_direction
{
    rd_none,
    rd_left,
    rd_top,
    rd_right,
    rd_bottom,
    rd_topleft,
    rd_bottomright
};

class EdtPositionWidget;
class EdtLineWidget;
class EdtBarcodeWidget;
class EdtImageWidget;
class EdtTextWidget;
class EdtBrushWidget;
class QGraphicsRectItem;

class ItemBase
{
public:
    ItemBase(QGraphicsScene *s=0);
    ~ItemBase();
    QTabWidget *tabWidget;
    int w;
    virtual void getPosition(int &x1, int &y1, int &x2, int &y2);
    virtual void setPosition(int x1, int y1, int x2, int y2);
    virtual QString getName();
    virtual QDomElement toXml(QDomDocument doc);
    QGraphicsScene *itScene;
};

class RectItemResize : public QGraphicsRectItem
{
public:
    explicit RectItemResize(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

};

class RectItemBase : public QGraphicsRectItem, public ItemBase
{
public:
    explicit RectItemBase(QGraphicsItem *parent=0, QGraphicsScene *s=0);
    ~RectItemBase();
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual void setRect(const QRectF &rect);
    void getPosition(int &x1, int &y1, int &x2, int &y2);
    void setPosition(int x1, int y1, int x2, int y2);
private:
    EdtPositionWidget *posWidget;
    RectItemResize *topLeft;
    RectItemResize *bottomRight;
    resize_direction resize_direction_;

};

class RectItem : public RectItemBase
{
public:
    RectItem(QGraphicsItem *parent=0, QGraphicsScene *s=0);
    RectItem(QDomElement el, QGraphicsScene *s=0);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QDomElement toXml(QDomDocument doc);
    QString getName();
private:
    EdtLineWidget *lineWidget;
    EdtBrushWidget *brushWidget;
    void createTabs();
};

class BarcodeItem : public RectItemBase
{
public:
    BarcodeItem(QGraphicsItem *parent=0, QGraphicsScene *s=0);
    BarcodeItem(QDomElement el, QGraphicsScene *s=0);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setBarcode(QString s);
    QDomElement toXml(QDomDocument doc);
    QString getName();
private:
    EAN13   *barcode;
    EdtBarcodeWidget *barcodeWidget;
    void createTabs();
};

class ImageItem : public RectItemBase
{
public:
    ImageItem(QGraphicsItem *parent=0, QGraphicsScene *s=0);
    ImageItem(QDomElement el, QGraphicsScene *s=0);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setImage(QPixmap pix);
    QDomElement toXml(QDomDocument doc);
    QString getName();
private:
    QPixmap image;
    EdtImageWidget *imageWidget;
    void createTabs();
};

class TextItem : public RectItemBase
{
public:
    TextItem(QGraphicsItem *parent=0, QGraphicsScene *s=0);
    TextItem(QDomElement el, QGraphicsScene *s=0);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QTextDocument *doc;
    QDomElement toXml(QDomDocument d);
    QString getName();
private:
    EdtTextWidget *textWidget;
    void createTabs();
};

class EllipseItem : public RectItem
{
public:
    EllipseItem(QGraphicsItem *parent=0, QGraphicsScene *s=0);
    EllipseItem(QDomElement el, QGraphicsScene *s=0);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QDomElement toXml(QDomDocument doc);
    QString getName();
private:
    QString text;
};

class LineItem : public QGraphicsLineItem, public ItemBase
{
public:
    LineItem(QGraphicsItem *parent=0, QGraphicsScene *s=0);
    LineItem(QDomElement el, QGraphicsScene *s=0);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void setLine(const QLineF &line);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void getPosition(int &x1, int &y1, int &x2, int &y2);
    void setPosition(int x1, int y1, int x2, int y2);
    QDomElement toXml(QDomDocument doc);
    QString getName();
private:
    EdtPositionWidget *posWidget;
    EdtLineWidget *lineWidget;
    RectItemResize *p1;
    RectItemResize *p2;
    resize_direction resize_direction_;
    void createTabs();
};

#endif // ITEMS_H
