#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    paperSize["US-Letter"]=QPrinter::Letter;
    paperSize["US-Legal"]=QPrinter::Legal;
    paperSize["US-Executive"]=QPrinter::Executive;
    paperSize["A0"]=QPrinter::A0;
    paperSize["A1"]=QPrinter::A1;
    paperSize["A2"]=QPrinter::A2;
    paperSize["A3"]=QPrinter::A3;
    paperSize["A4"]=QPrinter::A4;
    paperSize["A5"]=QPrinter::A5;
    paperSize["A6"]=QPrinter::A6;
    paperSize["A7"]=QPrinter::A7;
    paperSize["A8"]=QPrinter::A8;
    paperSize["A9"]=QPrinter::A9;
    paperSize["A10"]=QPrinter::Custom;
    paperSize["B0"]=QPrinter::B0;
    paperSize["B1"]=QPrinter::B1;
    paperSize["B2"]=QPrinter::B2;
    paperSize["B3"]=QPrinter::B3;
    paperSize["B4"]=QPrinter::B4;
    paperSize["B5"]=QPrinter::B5;
    paperSize["B6"]=QPrinter::B6;
    paperSize["B7"]=QPrinter::B7;
    paperSize["B8"]=QPrinter::B8;
    paperSize["B9"]=QPrinter::B9;
    paperSize["B10"]=QPrinter::B10;

    setLock(true);
    scene = new QGraphicsScene(this);
    scene->setSceneRect(QRect(0, 0, 300, 300));

    currentScale=1;
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setBackgroundBrush(QBrush(Qt::gray));

    loadSettings();

    connect(ui->actionDelete,SIGNAL(triggered(bool)),this,SLOT(delItem()));
    connect(ui->actionNew,SIGNAL(triggered(bool)),this,SLOT(newLbl()));
    connect(ui->actionSave,SIGNAL(triggered(bool)),this,SLOT(saveLbl()));
    connect(ui->actionOpen,SIGNAL(triggered(bool)),this,SLOT(openLbl()));
    connect(ui->actionRectangle,SIGNAL(triggered(bool)),this,SLOT(addRect()));
    connect(ui->actionBarcode,SIGNAL(triggered(bool)),this,SLOT(addBarcode()));
    connect(ui->actionImage,SIGNAL(triggered(bool)),this,SLOT(addImage()));
    connect(ui->actionText,SIGNAL(triggered(bool)),this,SLOT(addText()));
    connect(ui->actionEllipse,SIGNAL(triggered(bool)),this,SLOT(addEllipse()));
    connect(ui->actionLine,SIGNAL(triggered(bool)),this,SLOT(addLine()));
    connect(ui->actionQuit,SIGNAL(triggered(bool)),qApp,SLOT(quit()));
    connect(ui->actionPrint,SIGNAL(triggered(bool)),this,SLOT(print()));
    connect(ui->actionPrint_preview,SIGNAL(triggered(bool)),this,SLOT(printPreview()));
    connect(ui->actionExport_PDF,SIGNAL(triggered(bool)),this,SLOT(exportPdf()));
    connect(ui->actionZoom_in,SIGNAL(triggered(bool)),this,SLOT(zoomIn()));
    connect(ui->actionZoom_out,SIGNAL(triggered(bool)),this,SLOT(zoomOut()));
    connect(ui->actionZoom_fit,SIGNAL(triggered(bool)),this,SLOT(zoomFit()));
    connect(ui->actionAnti_aliasing,SIGNAL(triggered(bool)),this,SLOT(setAntiAliasing(bool)));
    connect(scene,SIGNAL(selectionChanged()),this,SLOT(setTabs()));
    connect(ui->listWidget,SIGNAL(currentRowChanged(int)),this,SLOT(selectItem(int)));
    connect(ui->listWidget,SIGNAL(itemChanged(QListWidgetItem*)),this,SLOT(blockItem(QListWidgetItem*)));
}

MainWindow::~MainWindow()
{
    saveSettings();
    QList <QGraphicsItem *> items = scene->items();
    for (int i=0; i<items.size(); i++){
            delete items.at(i);
    }
    delete ui;
}

void MainWindow::addItem(QGraphicsItem *i)
{
    ItemBase *it = dynamic_cast<ItemBase *>(i);
    if (it){
        ui->stackedWidget->addWidget(it->tabWidget);
        scene->addItem(i);
    }
    updList();
}

void MainWindow::loadFile(QString filename)
{
    clear();
    QFile file(filename);
    QString errorStr;
    int errorLine;
    int errorColumn;
    QDomDocument domDocument;
    if (!domDocument.setContent(&file, true, &errorStr, &errorLine,	&errorColumn)) {
        return;
    }
    QDomElement root = domDocument.firstChildElement("label");
    QDomNodeList list = root.elementsByTagName ("Template");
    QString c = list.at(0).toElement().attribute("description");
    Papers *papers = new Papers();
    QString size = list.at(0).toElement().attribute("size");
    for(int i=0;i<papers->list.count();i++) {
        QDomElement child = papers->list.at(i).toElement();
        QString id = child.attribute("id");
        if(id.compare(size, Qt::CaseInsensitive)==0) {
            //current_paper = child;
            break;
        }
    }

    current_template = list.at(0).toElement();
    list = root.elementsByTagName ("rotate");
    is_rotated = list.at(0).toElement().attribute("value").toInt();
    drawTemplate();
    list = root.elementsByTagName ("items");
    list = list.at(0).childNodes();
    QList<QString> items_c;
    items_c << ""<< ""<< ""<< "rectangle"<< "ellipse" << "barcode"<< "line" << "image"<< "text";
    for(int i=0;i<list.count();i++) {
        QDomNode item = list.at(i);
        switch(items_c.indexOf(item.nodeName())) {
        case 3: {
            addItem(new RectItem(item.toElement(),scene));
        } break;
        case 4: {
            addItem(new EllipseItem(item.toElement(),scene));
        } break;
        case 5: {
            addItem(new BarcodeItem(item.toElement(),scene));
        } break;
        case 6: {
            addItem(new LineItem(item.toElement(),scene));
        } break;
        case 7: {
            addItem(new ImageItem(item.toElement(),scene));
        } break;
        case 8: {
            addItem(new TextItem(item.toElement(),scene));
        } break;
        }
    }
    zoomFit();
    setLock(false);
}

void MainWindow::addRect()
{
    addItem(new RectItem(NULL,scene));
}

void MainWindow::addBarcode()
{
    addItem(new BarcodeItem(NULL,scene));
}

void MainWindow::addImage()
{
    addItem(new ImageItem(NULL,scene));
}

void MainWindow::addText()
{
    addItem(new TextItem(NULL,scene));
}

void MainWindow::addEllipse()
{
    addItem(new EllipseItem(NULL,scene));
}

void MainWindow::addLine()
{
    addItem(new LineItem(NULL,scene));
}

void MainWindow::zoomIn()
{
    ui->graphicsView->scale(1.6, 1.6);
    currentScale*=1.6;
}

void MainWindow::zoomOut()
{
    ui->graphicsView->scale(0.625,0.625);
    currentScale*=0.625;
}

void MainWindow::zoomFit()
{
    if (scene->width()!=0 && scene->height()!=0 && currentScale!=0){
        qreal w=ui->graphicsView->width();
        qreal h=ui->graphicsView->height();
        qreal sc = (w-60.0)/scene->width();
        qreal h1 = (h-60.0)/scene->height();
        if(h1<sc) sc = h1;
        ui->graphicsView->scale(sc/currentScale,sc/currentScale);
        currentScale*=(sc/currentScale);
    }
}

void MainWindow::setTabs()
{
    QList<QGraphicsItem *> l=scene->selectedItems();
    if (l.size()>0){
        ItemBase *it = dynamic_cast<ItemBase *>(l.at(l.size()-1));
        if (it){
            ui->stackedWidget->setCurrentWidget(it->tabWidget);
            QListWidgetItem *i =map.key(l.at(l.size()-1));
            if (i) {
                ui->listWidget->blockSignals(true);
                ui->listWidget->setCurrentItem(i);
                ui->listWidget->blockSignals(false);
            }
        }
    } else {
        ui->stackedWidget->setCurrentWidget(ui->pageDefault);
    }
}

void MainWindow::delItem()
{
    QList <QGraphicsItem *> items = scene->selectedItems();
    for (int i=0; i<items.size(); i++){
            delete items.at(i);
    }
    updList();
}

void MainWindow::newLbl()
{
    clear();
    DialogNewLabel l;
    if (l.exec()==QDialog::Accepted){
        is_rotated = l.isRotate();
        current_template=l.getCurrentTemplate();
        drawTemplate();
        zoomFit();
        setLock(false);
    }
}

void MainWindow::saveLbl()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Label"),	QDir::homePath(), tr("labels files (*.lbl);;All Files (*)"));
    if (fileName.isEmpty())
        return;
    if (QFileInfo(fileName).suffix().isEmpty())
        fileName.append(".lbl");
    QDomDocument doc;
    QDomElement root = doc.createElement("label");
    doc.appendChild(root);
    root.appendChild(current_template);
    QDomElement rotate = doc.createElement("rotate");
    rotate.setAttribute("value", is_rotated);
    root.appendChild(rotate);
    QDomElement items = doc.createElement("items");
    root.appendChild(items);

    QList<QGraphicsItem *> l=scene->items();
    for (int i=l.size()-1; i>0; i--){
        ItemBase *it = dynamic_cast<ItemBase *>(l.at(i));
        if (it){
            QDomElement e = it->toXml(doc);
            items.appendChild(e);
        }
    }

    QFile file(fileName);
    if ( file.open( QIODevice::WriteOnly ) ) {
        QTextStream stream( &file );
        doc.save(stream,1);
        file.close();
    }
}

void MainWindow::openLbl()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Label"), QDir::homePath(), tr("All Files (*);;labels files (*.lbl)"));
    if (!fileName.isEmpty()){
        loadFile(fileName);
    }
}

void MainWindow::setAntiAliasing(bool b)
{
    ui->graphicsView->setRenderHint(QPainter::Antialiasing,b);
}

void MainWindow::printPreview()
{
    QPrinter p;
    p.setPaperSize(paperSize.value(current_template.attribute("size"),QPrinter::A4));
    QPrintPreviewDialog d(&p);
    connect(&d,SIGNAL(paintRequested(QPrinter*)),this,SLOT(printLbl(QPrinter*)));
    d.exec();
}

void MainWindow::print()
{
    QPrinter p;
    p.setPaperSize(paperSize.value(current_template.attribute("size"),QPrinter::A4));
    p.setOutputFormat(QPrinter::NativeFormat);
    QPrintDialog printDialog(&p, this);
    if (printDialog.exec() == QDialog::Accepted) {
        printLbl(&p);
    }
}

void MainWindow::exportPdf()
{
    QString filename;
    filename = QFileDialog::getSaveFileName(this,tr("Save PDF"),QDir::homePath()+"/label.pdf", "*.pdf");
    if (!filename.isEmpty()) {
        if (QFileInfo(filename).suffix().isEmpty())
            filename.append(".pdf");
        QPrinter p;
        p.setOutputFormat(QPrinter::PdfFormat);
        p.setOutputFileName(filename);
        p.setResolution(QPrinter::HighResolution);
        p.setColorMode(QPrinter::Color);
        p.setFullPage(true);
        p.setPaperSize(paperSize.value(current_template.attribute("size"),QPrinter::A4));
        printLbl(&p);
    }
}

void MainWindow::printLbl(QPrinter *p)
{
    scene->clearSelection();
    double max_scale=1;
    p->setResolution(int(72*max_scale));
    p->setFullPage(true);
    p->setColorMode(QPrinter::Color);
    QPainter pp(p);
    if(ui->actionAnti_aliasing->isChecked()) {
        pp.setRenderHints(QPainter::Antialiasing);
    }
    QDomNodeList items = current_template.childNodes();
    int i;
    for(i=0;i<items.count();i++) {
        QDomElement c = items.at(i).toElement();
        QString tagname = c.tagName();
        double rect_width;
        double rect_height;
        double radius;
        double hole;
        if(tagname.compare("Label-rectangle", Qt::CaseInsensitive)==0) {
            rect_width = unit_to_point(c.attribute("width"), max_scale);
            rect_height = unit_to_point(c.attribute("height"), max_scale);
        } else if (tagname.compare("Label-cd", Qt::CaseInsensitive)==0) {
            radius = unit_to_point(c.attribute("radius"), max_scale);
            hole = unit_to_point(c.attribute("hole"), max_scale);
            rect_width = radius*2;
            rect_height = rect_width;
        }
        QDomNodeList l = c.elementsByTagName("Layout");
        int k;
        for(k=0;k<l.count();k++) {
            QDomElement layout = l.at(k).toElement();
            double x0 = unit_to_point(layout.attribute("x0"),max_scale);
            double y0 = unit_to_point(layout.attribute("y0"),max_scale);
            double dx = unit_to_point(layout.attribute("dx"),max_scale);
            double dy = unit_to_point(layout.attribute("dy"),max_scale);
            int xi,yi,j;
            int nx = layout.attribute("nx").toInt();
            int ny = layout.attribute("ny").toInt();

            /*if (tagname.compare("Label-cd", Qt::CaseInsensitive)==0) {
                QPainterPath clipPath;
                clipPath.addEllipse(0,0, rect_width, rect_height);
                clipPath.addEllipse(radius-hole,radius-hole, hole*2, hole*2);
                pp.setClipPath(clipPath);
            }*/

            for(yi = 0;yi<ny;yi++) {
                for(xi = 0;xi<nx;xi++) {
                    if (is_rotated){
                        pp.rotate(90);
                        QRectF r(y0+dy*yi, -(x0+dx*xi)-rect_width, rect_height, rect_width);
                        scene->render(&pp,r);
                        pp.rotate(-90);
                    } else {
                        QRectF r(x0+dx*xi, y0+dy*yi, rect_width, rect_height);
                        scene->render(&pp,r);
                    }
                    if (ui->actionPrint_outlines->isChecked()) {
                        if (tagname.compare("Label-cd", Qt::CaseInsensitive)==0) {
                            //pp.drawEllipse(int(x0+dx*xi), int(y0+dy*yi), int(rect_width), int(rect_height));
                            //pp.drawEllipse(int(x0+dx*xi+radius-hole), int(y0+dy*yi+radius-hole), int(hole*2), int(hole*2));
                        } else {
                            pp.drawRect(QRectF(x0+dx*xi, y0+dy*yi, rect_width, rect_height));
                        }
                    } else if(ui->actionPrint_crop_marks->isChecked()) {
                        float pl[8] = {0,0, rect_width,0, rect_width, rect_height, 0,rect_height};
                        for(j=0;j<4;j++) {
                            pp.drawLine(int(x0-10*max_scale+dx*xi+pl[j*2]), int(y0+dy*yi+pl[j*2+1]), int(x0+10*max_scale+dx*xi+pl[j*2]), int(y0+dy*yi+pl[j*2+1]));
                            pp.drawLine(int(x0+dx*xi+pl[j*2]), int(y0-10*max_scale+dy*yi+pl[j*2+1]), int(x0+dx*xi+pl[j*2]), int(y0+10*max_scale+dy*yi+pl[j*2+1]));
                        }
                    }
                }
            }
        }
    }
}

void MainWindow::setLock(bool l)
{
    ui->actionSave->setDisabled(l);
    ui->actionPrint->setDisabled(l);
    ui->actionPrint_preview->setDisabled(l);
    ui->actionExport_PDF->setDisabled(l);
    ui->actionLine->setDisabled(l);
    ui->actionRectangle->setDisabled(l);
    ui->actionEllipse->setDisabled(l);
    ui->actionImage->setDisabled(l);
    ui->actionText->setDisabled(l);
    ui->actionBarcode->setDisabled(l);
}

void MainWindow::updList()
{
    ui->listWidget->clear();
    map.clear();
    QList <QGraphicsItem *> l=scene->items();
    int j=0;
    for (int i=l.size()-1; i>0; i--){
        ItemBase *it = dynamic_cast<ItemBase *>(l.at(i));
        if (it){
            int x1,y1,x2,y2;
            it->getPosition(x1,y1,x2,y2);
            QListWidgetItem *t = new QListWidgetItem(ui->listWidget);
            bool b=l.at(i)->flags().testFlag(QGraphicsItem::ItemIsSelectable) && (l.at(i)->flags().testFlag(QGraphicsItem::ItemIsMovable));
            map[t]=l.at(i);
            t->setCheckState(b? Qt::Checked : Qt::Unchecked);
            t->setData(Qt::DisplayRole,it->getName()+"-"+QString::number(j));
            j++;
        }
    }
}

void MainWindow::selectItem(int p)
{
    QGraphicsItem *i = map[ui->listWidget->item(p)];
    if (i) {
        scene->clearSelection();
        i->setSelected(true);
    }
}

void MainWindow::blockItem(QListWidgetItem *i)
{
    QGraphicsItem *it = map[i];
    if (it) {
        if (i->checkState()==Qt::Checked){
            it->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
        } else {
            it->setFlags(QGraphicsItem::ItemIsSelectable);
        }
    }
}

void MainWindow::drawTemplate()
{
    QDomNodeList items = current_template.childNodes();
    if(items.isEmpty()) return;
    QDomElement label = items.at(0).toElement();
    double template_width;
    double template_height;
    if(label.tagName().compare("Label-rectangle", Qt::CaseInsensitive)==0) {
        double template_width = unit_to_point(label.attribute("width"));
        double template_height = unit_to_point(label.attribute("height"));
        if(is_rotated) {
            double tmp = template_width;
            template_width = template_height;
            template_height = tmp;
        }
        scene->setSceneRect(0,0,template_width, template_height);
        QAbstractGraphicsShapeItem *i = scene->addRect( QRectF(-3, -3, template_width+6, template_height+6));
        i->setBrush( QColor(255,255,255) );
        i->setPen( QPen(QColor(172,214,230), 2) );
        i->setZValue(0);
        QDomNodeList items0 = label.childNodes();
        for(int i0=0;i0<items0.count();i0++) {
            QDomElement item = items0.at(i0).toElement();
            if(item.tagName().compare("Markup-line", Qt::CaseInsensitive)==0) {
                double x1 = unit_to_point(item.attribute("x1"));
                double y1 = unit_to_point(item.attribute("y1"));
                double x2 = unit_to_point(item.attribute("x2"));
                double y2 = unit_to_point(item.attribute("y2"));
                if(is_rotated) {
                    double tmp = x1;
                    x1 = y1;
                    y1 = tmp;
                    tmp = x2;
                    x2 = y2;
                    y2 = tmp;
                }
                QGraphicsLineItem *i1 = scene->addLine( QLine(x1, y1, x2,y2));
                i1->setPen( QPen(QColor(197,0,0), 0) );
                i1->setZValue(2);
            }
        }
    } else if(label.tagName().compare("Label-cd", Qt::CaseInsensitive)==0) {
        double radius = unit_to_point(label.attribute("radius"));
        double hole = unit_to_point(label.attribute("hole"));
        template_width = radius*2;
        template_height = radius*2;
        scene->setSceneRect(-2,-2,template_width+4, template_height+4);
        QAbstractGraphicsShapeItem *i = scene->addEllipse( QRectF(0, 0, template_width, template_height));
        i->setBrush( QColor(255,255,255) );
        i->setPen( QPen(QColor(172,214,230), 2) );
        i->setZValue(0);
        QAbstractGraphicsShapeItem *hl = scene->addEllipse( QRectF(radius-hole, radius-hole, hole*2, hole*2));
        hl->setPen( QPen(QColor(172,214,230), 2) );
    }
}

void MainWindow::clear()
{
    setLock(true);
    current_template= QDomElement();
    scene->clear();
    updList();
}

void MainWindow::loadSettings()
{
    QSettings settings("labels", "labels");
    this->restoreGeometry(settings.value("main_geometry").toByteArray());
    this->ui->splitter->restoreState(settings.value("main_splitter_width").toByteArray());
    ui->actionAnti_aliasing->setChecked(settings.value("antialiasing").toBool());
    ui->actionPrint_crop_marks->setChecked(settings.value("print_crop_marks").toBool());
    ui->actionPrint_outlines->setChecked(settings.value("print_outlines").toBool());
}

void MainWindow::saveSettings()
{
    QSettings settings("labels", "labels");
    settings.setValue("main_geometry", this->saveGeometry());
    settings.setValue("main_splitter_width",ui->splitter->saveState());
    settings.setValue("antialiasing",ui->actionAnti_aliasing->isChecked());
    settings.setValue("print_crop_marks",ui->actionPrint_crop_marks->isChecked());
    settings.setValue("print_outlines",ui->actionPrint_outlines->isChecked());
}
