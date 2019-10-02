#include "items.h"


ItemBase::ItemBase(QGraphicsScene *s) : itScene(s)
{
    w=6;
    tabWidget = new QTabWidget();
}

ItemBase::~ItemBase()
{
    delete tabWidget;
}

void ItemBase::getPosition(int &x1, int &y1, int &x2, int &y2)
{
    x1=0;
    x2=0;
    y1=0;
    y2=0;
}

void ItemBase::setPosition(int x1, int y1, int x2, int y2)
{
    return;
}

QString ItemBase::getName()
{
    return QString();
}

QDomElement ItemBase::toXml(QDomDocument doc)
{
    return QDomElement();
}


RectItemResize::RectItemResize(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent) : QGraphicsRectItem(x,y,w,h,parent)
{
    this->setBrush(QBrush(Qt::green));
}

void RectItemResize::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    RectItemBase *item = qgraphicsitem_cast<RectItemBase *>(this->parentItem());
    if (item){
        item->mousePressEvent(event);
    } else {
        LineItem *it = qgraphicsitem_cast<LineItem *>(this->parentItem());
        if (it){
            it->mousePressEvent(event);
        }
    }
}

void RectItemResize::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    RectItemBase *item = qgraphicsitem_cast<RectItemBase *>(this->parentItem());
    if (item){
        item->mouseMoveEvent(event);
    }
    LineItem *it = qgraphicsitem_cast<LineItem *>(this->parentItem());
    if (it){
        it->mouseMoveEvent(event);
    }
}

RectItemBase::RectItemBase(QGraphicsItem *parent, QGraphicsScene *s) : QGraphicsRectItem(20,20,80,100,parent), ItemBase(s)
{
    setFlag(QGraphicsItem::ItemIsSelectable);

    topLeft = new RectItemResize(20-w/2,20-w/2,w,w,this);
    topLeft->setCursor(Qt::SizeFDiagCursor);
    bottomRight = new RectItemResize(100-w/2,120-w/2,w,w,this);
    bottomRight->setCursor(Qt::SizeFDiagCursor);

    posWidget = new EdtPositionWidget(this,tabWidget);
    tabWidget->addTab(posWidget,QString("Позиция"));

    topLeft->setVisible(false);
    bottomRight->setVisible(false);
}

RectItemBase::~RectItemBase()
{
}

void RectItemBase::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF p =  event->pos();
    if (topLeft->contains(p)){
        resize_direction_=rd_topleft;
    } else if(bottomRight->contains(p)){
        resize_direction_=rd_bottomright;
    } else
        resize_direction_ = rd_none;

    QGraphicsRectItem::mousePressEvent(event);
}

void RectItemBase::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (isSelected() && this->flags().testFlag(QGraphicsItem::ItemIsMovable))
    {
        QPointF p = event->pos();
        QPointF pp = mapToItem(this, p);
        QRectF r = rect();
        switch (resize_direction_)
        {
        case rd_left:
            r.setLeft(pp.x());
            setRect(r);
            break;
        case rd_top:
            r.setTop(pp.y());
            setRect(r);
            break;
        case rd_right:
            r.setRight(pp.x());
            setRect(r);
            break;
        case rd_bottom:
            r.setBottom(pp.y());
            setRect(r);
            break;
        case rd_topleft:
            r.setTop(pp.y());
            r.setLeft(pp.x());
            setRect(r);
            break;
        case rd_bottomright:
            r.setBottom(pp.y());
            r.setRight(pp.x());
            setRect(r);
            break;
        default:
            QGraphicsRectItem::mouseMoveEvent(event);
            break;
        }
        return;
   }
    QGraphicsRectItem::mouseMoveEvent(event);
}

void RectItemBase::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (option->state & (QStyle::State_Selected | QStyle::State_HasFocus)) {
        topLeft->setVisible(this->flags().testFlag(QGraphicsItem::ItemIsMovable));
        bottomRight->setVisible(this->flags().testFlag(QGraphicsItem::ItemIsMovable));
        if (this->cursor().shape()!=Qt::SizeAllCursor){
            this->setCursor(Qt::SizeAllCursor);
        }
        painter->setPen(QPen(QBrush(Qt::black),0,Qt::DashLine));
        painter->setBrush(Qt::NoBrush);
        QRectF r = this->boundingRect();
        painter->drawRect(r);
    } else {
        if (this->cursor().shape()!=Qt::ArrowCursor){
            this->setCursor(Qt::ArrowCursor);
        }
        if (topLeft->isVisible()) topLeft->setVisible(false);
        if (bottomRight->isVisible()) bottomRight->setVisible(false);
    }
}

void RectItemBase::setRect(const QRectF &rect)
{
    if (rect.height()>=0&&rect.width()>=0){
        QGraphicsRectItem::setRect(rect);
        topLeft->setRect(rect.x()-w/2,rect.y()-w/2,w,w);
        bottomRight->setRect(rect.x()+rect.width()-w/2,rect.y()+rect.height()-w/2,w,w);
    }
}

void RectItemBase::getPosition(int &x1, int &y1, int &x2, int &y2)
{
    QRectF r=rect();
    x1=this->x()+r.left();
    y1=this->y()+r.top();
    x2=x1+r.width();
    y2=y1+r.height();
}

void RectItemBase::setPosition(int x1, int y1, int x2, int y2)
{
    QRectF r=rect();
    r.setLeft(x1-this->x());
    r.setTop(y1-this->y());
    r.setRight(x2-this->x());
    r.setBottom(y2-this->y());
    setRect(r);
}


RectItem::RectItem(QGraphicsItem *parent, QGraphicsScene *s) : RectItemBase(parent,s)
{
    setFlag(QGraphicsItem::ItemIsMovable);
    this->setBrush(QBrush(Qt::white));
    createTabs();
}

RectItem::RectItem(QDomElement el, QGraphicsScene *s) : RectItemBase(NULL,s)
{
    QPen p;
    p.setWidth(el.attribute("width").toInt());
    p.setColor(QColor(el.attribute("color")));
    p.setStyle(Qt::PenStyle(el.attribute("style").toInt()));
    this->setPen(p);

    QColor cb(QColor(el.attribute("fillcolor")));
    cb.setAlpha(el.attribute("alpha").toInt());
    this->setBrush(QBrush(cb));

    int x1=el.attribute("x1").toInt();
    int y1=el.attribute("y1").toInt();
    int x2=el.attribute("x2").toInt();
    int y2=el.attribute("y2").toInt();
    this->setPosition(x1,y1,x2,y2);

    createTabs();
}

void RectItem::createTabs()
{
    lineWidget = new EdtLineWidget(this,tabWidget);
    tabWidget->addTab(lineWidget,QString("Перо"));
    brushWidget = new EdtBrushWidget(this,tabWidget);
    tabWidget->addTab(brushWidget,QString("Кисть"));
}

void RectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QGraphicsRectItem::paint(painter,option,widget);
    RectItemBase::paint(painter,option,widget);
}

QDomElement RectItem::toXml(QDomDocument doc)
{
    QDomElement l =doc.createElement(this->getName());
    int x1,y1,x2,y2;
    this->getPosition(x1,y1,x2,y2);
    l.setAttribute("x1",x1);
    l.setAttribute("y1",y1);
    l.setAttribute("x2",x2);
    l.setAttribute("y2",y2);
    l.setAttribute("width",this->pen().width());
    l.setAttribute("color",this->pen().color().name());
    l.setAttribute("style",this->pen().style());
    QColor color = this->brush().color();
    l.setAttribute("fillcolor",color.name());
    l.setAttribute("alpha",color.alpha());
    return l;
}

QString RectItem::getName()
{
    return QString("rectangle");
}


BarcodeItem::BarcodeItem(QGraphicsItem *parent, QGraphicsScene *s) : RectItemBase(parent,s)
{
    createTabs();
    barcodeWidget->setBarcode("4627120423432");
    setFlag(QGraphicsItem::ItemIsMovable);
}

BarcodeItem::BarcodeItem(QDomElement el, QGraphicsScene *s) : RectItemBase(NULL,s)
{
    createTabs();
    barcodeWidget->setBarcode(el.attribute("code"));
    int x1=el.attribute("x1").toInt();
    int y1=el.attribute("y1").toInt();
    int x2=el.attribute("x2").toInt();
    int y2=el.attribute("y2").toInt();
    this->setPosition(x1,y1,x2,y2);
}


void BarcodeItem::createTabs()
{
    barcode = new EAN13();
    barcodeWidget = new EdtBarcodeWidget(this,tabWidget);
    tabWidget->addTab(barcodeWidget,QString("Штрих код"));
}


void BarcodeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    barcode->draw(this->rect(),painter);
    RectItemBase::paint(painter,option,widget);
}

void BarcodeItem::setBarcode(QString s)
{
    barcode->makePattern(s);
    this->update();
}

QDomElement BarcodeItem::toXml(QDomDocument doc)
{
    QDomElement l =doc.createElement(this->getName());
    int x1,y1,x2,y2;
    this->getPosition(x1,y1,x2,y2);
    l.setAttribute("x1",x1);
    l.setAttribute("y1",y1);
    l.setAttribute("x2",x2);
    l.setAttribute("y2",y2);
    l.setAttribute("code",barcode->currentBarcode());
    return l;
}

QString BarcodeItem::getName()
{
    return QString("barcode");
}

ImageItem::ImageItem(QGraphicsItem *parent, QGraphicsScene *s) : RectItemBase(parent,s)
{
    image=QPixmap(":images/checkerboard.png");
    createTabs();
    setFlag(QGraphicsItem::ItemIsMovable);
}

ImageItem::ImageItem(QDomElement el, QGraphicsScene *s) : RectItemBase(NULL,s)
{
    QDomCDATASection cd = el.firstChild().toCDATASection();
    QByteArray bytes = QByteArray::fromBase64(cd.data().toLatin1());
    image.loadFromData(bytes);
    int x1=el.attribute("x1").toInt();
    int y1=el.attribute("y1").toInt();
    int x2=el.attribute("x2").toInt();
    int y2=el.attribute("y2").toInt();
    this->setPosition(x1,y1,x2,y2);
    createTabs();
}

void ImageItem::createTabs()
{
    imageWidget = new EdtImageWidget(this,tabWidget);
    tabWidget->addTab(imageWidget,QString("Изображение"));
}

void ImageItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawPixmap(this->rect(),image,image.rect());
    RectItemBase::paint(painter,option,widget);
}

void ImageItem::setImage(QPixmap pix)
{
    image=pix;
    update();
}

QDomElement ImageItem::toXml(QDomDocument doc)
{
    QDomElement l =doc.createElement(this->getName());
    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");
    QDomCDATASection cd = doc.createCDATASection(bytes.toBase64 ());
    l.appendChild(cd);
    int x1,y1,x2,y2;
    this->getPosition(x1,y1,x2,y2);
    l.setAttribute("x1",x1);
    l.setAttribute("y1",y1);
    l.setAttribute("x2",x2);
    l.setAttribute("y2",y2);
    return l;
}

QString ImageItem::getName()
{
    return QString("image");
}

TextItem::TextItem(QGraphicsItem *parent, QGraphicsScene *s) : RectItemBase(parent,s)
{
    doc = new QTextDocument(QString::fromUtf8("text"));
    createTabs();
    setFlag(QGraphicsItem::ItemIsMovable);
}

TextItem::TextItem(QDomElement el, QGraphicsScene *s) : RectItemBase(NULL,s)
{
    doc=new QTextDocument();
    doc->setHtml(el.attribute("data"));
    int x1=el.attribute("x1").toInt();
    int y1=el.attribute("y1").toInt();
    int x2=el.attribute("x2").toInt();
    int y2=el.attribute("y2").toInt();
    this->setPosition(x1,y1,x2,y2);
    createTabs();
}

void TextItem::createTabs()
{
    textWidget = new EdtTextWidget(this,tabWidget);
    tabWidget->addTab(textWidget,QString("Текст"));
}

void TextItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();
    QTextDocument *d=doc->clone();
    painter->setViewport(0,0,rect().width(),rect().height());
    painter->setWindow(0,0,rect().width(),rect().height());
    painter->translate(rect().x(),rect().y());
    d->setTextWidth(rect().width());
    d->drawContents(painter,painter->window());
    painter->restore();
    RectItemBase::paint(painter,option,widget);
}

QDomElement TextItem::toXml(QDomDocument d)
{
    QDomElement l =d.createElement(this->getName());
    int x1,y1,x2,y2;
    this->getPosition(x1,y1,x2,y2);
    l.setAttribute("x1",x1);
    l.setAttribute("y1",y1);
    l.setAttribute("x2",x2);
    l.setAttribute("y2",y2);
    l.setAttribute("data",doc->toHtml());
    return l;
}

QString TextItem::getName()
{
    return QString("text");
}


EllipseItem::EllipseItem(QGraphicsItem *parent, QGraphicsScene *s) : RectItem(parent,s)
{
}

EllipseItem::EllipseItem(QDomElement el, QGraphicsScene *s) : RectItem(el,s)
{
}

void EllipseItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    RectItemBase::paint(painter,option,widget);
    painter->setPen(this->pen());
    painter->setBrush(this->brush());
    painter->drawEllipse(this->rect());
}

QDomElement EllipseItem::toXml(QDomDocument doc)
{
    QDomElement l =doc.createElement(this->getName());
    int x1,y1,x2,y2;
    this->getPosition(x1,y1,x2,y2);
    l.setAttribute("x1",x1);
    l.setAttribute("y1",y1);
    l.setAttribute("x2",x2);
    l.setAttribute("y2",y2);
    l.setAttribute("width",this->pen().width());
    l.setAttribute("color",this->pen().color().name());
    l.setAttribute("style",this->pen().style());
    QColor color = this->brush().color();
    l.setAttribute("fillcolor",color.name());
    l.setAttribute("alpha",color.alpha());
    return l;
}

QString EllipseItem::getName()
{
    return QString("ellipse");
}

LineItem::LineItem(QGraphicsItem *parent, QGraphicsScene *s) : QGraphicsLineItem(20,20,80,100,parent), ItemBase(s)
{
    createTabs();
    setFlag(QGraphicsItem::ItemIsMovable);
}

LineItem::LineItem(QDomElement el, QGraphicsScene *s) : QGraphicsLineItem(NULL), ItemBase(s)
{
    QPen p;
    p.setWidth(el.attribute("width").toInt());
    p.setColor(QColor(el.attribute("color")));
    p.setStyle(Qt::PenStyle(el.attribute("style").toInt()));
    this->setPen(p);

    int x1=el.attribute("x1").toInt();
    int y1=el.attribute("y1").toInt();
    int x2=el.attribute("x2").toInt();
    int y2=el.attribute("y2").toInt();

    createTabs();

    this->setPosition(x1,y1,x2,y2);
}

void LineItem::createTabs()
{
    setFlag(QGraphicsItem::ItemIsSelectable);
    posWidget = new EdtPositionWidget(this,tabWidget);
    tabWidget->addTab(posWidget,QString("Позиция"));
    lineWidget = new EdtLineWidget(this,tabWidget);
    tabWidget->addTab(lineWidget,QString("Перо"));
    p1 = new RectItemResize(20-w/2,20-w/2,w,w,this);
    p2 = new RectItemResize(80-w/2,100-w/2,w,w,this);
    p1->setCursor(Qt::CrossCursor);
    p2->setCursor(Qt::CrossCursor);
    resize_direction_ = rd_none;
}

void LineItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF p =  event->pos();
    if (p1->contains(p)){
        resize_direction_=rd_topleft;
    } else if(p2->contains(p)){
        resize_direction_=rd_bottomright;
    } else
        resize_direction_ = rd_none;

    QGraphicsLineItem::mousePressEvent(event);
}

void LineItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (isSelected() && this->flags().testFlag(QGraphicsItem::ItemIsMovable))
    {
        QPointF p = event->pos();
        QPointF pp = mapToItem(this, p);
        QLineF l=this->line();
        switch (resize_direction_)
        {
        case rd_topleft:
            l.setP1(pp);
            this->setLine(l);
            break;
        case rd_bottomright:
            l.setP2(pp);
            this->setLine(l);
            break;
        default:
            QGraphicsLineItem::mouseMoveEvent(event);
            break;
        }
        return;
    }
    QGraphicsLineItem::mouseMoveEvent(event);
}

void LineItem::setLine(const QLineF &line)
{
    QGraphicsLineItem::setLine(line);
    p1->setRect(QRectF(line.p1().x()-w/2,line.p1().y()-w/2,w,w));
    p2->setRect(QRectF(line.p2().x()-w/2,line.p2().y()-w/2,w,w));
}

void LineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QGraphicsLineItem::paint(painter,option,widget);
    if (option->state & (QStyle::State_Selected | QStyle::State_HasFocus)) {
        p1->setVisible(this->flags().testFlag(QGraphicsItem::ItemIsMovable));
        p2->setVisible(this->flags().testFlag(QGraphicsItem::ItemIsMovable));
        if (this->cursor().shape()!=Qt::SizeAllCursor){
            this->setCursor(Qt::SizeAllCursor);
        }
    } else {
        if (this->cursor().shape()!=Qt::ArrowCursor){
            this->setCursor(Qt::ArrowCursor);
        }
        if (p1->isVisible()) p1->setVisible(false);
        if (p2->isVisible()) p2->setVisible(false);
    }
}

void LineItem::getPosition(int &x1, int &y1, int &x2, int &y2)
{
    QLineF l=this->line();
    x1=this->x()+l.p1().x();
    y1=this->y()+l.p1().y();
    x2=this->x()+(l.p2().x());
    y2=this->y()+(l.p2().y());
}

void LineItem::setPosition(int x1, int y1, int x2, int y2)
{
    QLineF l=this->line();
    l.setP1(QPointF(x1-this->x(),y1-this->y()));
    l.setP2(QPointF(x2-this->x(),y2-this->y()));
    setLine(l);
}

QDomElement LineItem::toXml(QDomDocument doc)
{
    QDomElement l =doc.createElement(this->getName());
    int x1,y1,x2,y2;
    this->getPosition(x1,y1,x2,y2);
    l.setAttribute("x1",x1);
    l.setAttribute("y1",y1);
    l.setAttribute("x2",x2);
    l.setAttribute("y2",y2);
    l.setAttribute("width",this->pen().width());
    l.setAttribute("color",this->pen().color().name());
    l.setAttribute("style",this->pen().style());
    return l;
}

QString LineItem::getName()
{
    return QString("line");
}


