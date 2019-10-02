#include "dialognewlabel.h"
#include "ui_dialognewlabel.h"

DialogNewLabel::DialogNewLabel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewLabel)
{
    ui->setupUi(this);
    papers = new Papers();
    templates = new Templates();
    int i;
    for(i=0;i<papers->list.count();i++) {
        QDomElement child = papers->list.at(i).toElement();
        QString id = child.attribute("id");
        ui->paperCombo->addItem(child.attribute("_name"), QVariant(id));
    }

    scene = new QGraphicsScene(ui->view);
    ui->view->setScene(scene);
    ui->view->setBackgroundRole(QPalette::Window);
    ui->view->setMinimumWidth(190);
    ui->view->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    pr = scene->addRect(QRect(0+13,0,140,200));
    pr->setBrush(QBrush( Qt::white ));

    connect(ui->paperCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(cchanged(int)));
    connect(ui->templateCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(tchanged(int)));
    cchanged(0);
}

DialogNewLabel::~DialogNewLabel()
{
    delete ui;
}

void DialogNewLabel::draw_template()
{
    double width = unit_to_point(current_paper.attribute("width"));
    double height = unit_to_point(current_paper.attribute("height"));
    double width_ = (width/height)*200;
    double sc = width_/width;
    QString desc = current_template.attribute("_description");
    if (desc.isEmpty()) {desc = current_template.attribute("description"); }
    ui->description->setText(desc);
    desc = current_paper.attribute("_name");
    ui->page_size->setText(desc);
    QDomNodeList items = current_template.childNodes();
    int i;
    QList<QGraphicsItem *> list = scene->items();
    QList<QGraphicsItem *>::Iterator it = list.begin();
    for (; it != list.end(); ++it) {
        if ( *it )
            delete *it;
    }
    double ar = width/height;
    pr = scene->addRect(QRect(0+15,0,int(ar*200),200),QPen(),QBrush( Qt::white ));
    int count = 0;
    for(i=0;i<items.count();i++) {
        QDomElement c = items.at(i).toElement();
        if(c.tagName().compare("Label-rectangle", Qt::CaseInsensitive)==0) {
            double rect_width = unit_to_point(c.attribute("width"));
            double rect_height = unit_to_point(c.attribute("height"));
            double mm_width = point_to_unit(rect_width, "mm", 1);
            double mm_height = point_to_unit(rect_height, "mm", 1);
            ui->label_size->setText(QString().number(mm_width, 'f', 2) + " x " + QString().number(mm_height, 'f', 2) + " " + tr("mm"));
            QDomNodeList l = c.elementsByTagName("Layout");
            if (l.isEmpty ()) {

            };
            int k;
            for(k=0;k<l.count();k++) {
                QDomElement layout = l.at(k).toElement();
                double x0 = unit_to_point(layout.attribute("x0"))*sc+15;
                double y0 = unit_to_point(layout.attribute("y0"))*sc;
                double dx = unit_to_point(layout.attribute("dx"))*sc;
                double dy = unit_to_point(layout.attribute("dy"))*sc;

                int xi,yi;
                int nx = layout.attribute("nx").toInt();
                int ny = layout.attribute("ny").toInt();
                for(yi = 0;yi<ny;yi++) {
                    for(xi = 0;xi<nx;xi++) {
                        QGraphicsRectItem *r = scene->addRect(QRect(int(x0+xi*dx), int(y0+dy*yi),int(rect_width*sc), int(rect_height*sc)));
                        r->setZValue(10);
                        count++;
                    }
                }
            }
        } else if ((c.tagName().compare("Label-cd", Qt::CaseInsensitive)==0)or(c.tagName().compare("Label-round", Qt::CaseInsensitive)==0)) {
            double radius = unit_to_point(c.attribute("radius"));
            double mm_radius = point_to_unit(radius, "mm", 1);
            ui->label_size->setText(QString().number(mm_radius*2,'f', 2) + " " + tr("mm") + " " + tr("diameter"));
            QDomNodeList l = c.elementsByTagName("Layout");
            if (l.isEmpty ()) {

            };
            int k;
            for(k=0;k<l.count();k++) {
                QDomElement layout = l.at(k).toElement();
                double x0 = unit_to_point(layout.attribute("x0"))*sc+15;
                double y0 = unit_to_point(layout.attribute("y0"))*sc;
                double dx = unit_to_point(layout.attribute("dx"))*sc;
                double dy = unit_to_point(layout.attribute("dy"))*sc;
                int nx = layout.attribute("nx").toInt();
                int ny = layout.attribute("ny").toInt();
                int xi,yi;
                for(yi = 0;yi<ny;yi++) {
                    for(xi = 0;xi<nx;xi++) {
                        QGraphicsEllipseItem *r = scene->addEllipse(QRect(int(x0+xi*dx), int(y0+dy*yi),int(radius*2*sc), int(radius*2*sc)));
                        r->setZValue(10);
                        count++;
                    }
                }
            }
    }
    ui->format->setText(QString().number(count)+" labels at page");
    }
}

void DialogNewLabel::clear()
{
    ui->description->clear();
    ui->page_size->clear();
    ui->label_size->clear();
    ui->format->clear();
}

bool DialogNewLabel::isRotate()
{
    return ui->checkBoxRotate->isChecked();
}

QDomElement DialogNewLabel::getCurrentTemplate()
{
    return current_template;
}

void DialogNewLabel::cchanged(int index)
{
    clear();
    QString size = ui->paperCombo->itemData(index).toString();
    QDomElement l = templates->doc.firstChildElement(size);
    l1 = l.elementsByTagName("Template");
    int i;
    for(i=0;i<papers->list.count();i++) {
        QDomElement c = papers->list.at(i).toElement();
        if (size.compare(c.attribute("id"))==0) {
            current_paper = c;
            break;
        }
    }

    double width = unit_to_point(current_paper.attribute("width"));
    double height = unit_to_point(current_paper.attribute("height"));
    double ar = width/height;
    pr->setRect(QRect(0,0,int(ar*200),200));
    ui->templateCombo->clear();
    for(i=0;i<l1.count();i++) {
        QDomElement child = l1.at(i).toElement();
        QString id = child.attribute("name");
        ui->templateCombo->addItem(child.attribute("name"), QVariant(id));
    }
    current_template = l1.at(0).toElement();
    draw_template();
    return;
}

void DialogNewLabel::tchanged(int index)
{
    QString name = ui->templateCombo->itemData(index).toString();
    for(int i=0;i<l1.count();i++) {
        QDomElement child = l1.at(i).toElement();
        QString id = child.attribute("name");
        if(id.compare(name) == 0) {
            current_template = child;
            draw_template();
            break;
        }
    }
}
