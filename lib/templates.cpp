
#include "templates.h"
#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <QApplication>
#include <qdebug.h>

Templates::Templates()
	: QObject()
{
	QStringList dirs;
#ifdef Q_WS_WIN32
    dirs << "./" << QDir::homePath() + "/.qlabels/" << "./templates/" << QApplication::applicationDirPath()  + "/templates/";;
#else
	dirs << "./" << "/usr/share/qlabels/templates/" << "/usr/local/share/qlabels/templates/" << QDir::homePath() + "/.qlabels/" << "./templates/";
#endif
	for (int j = 0; j < dirs.size(); ++j) {
		QDir dir(dirs.at(j));
		QStringList flist = dir.entryList(QStringList("*.xml"));
		QDomNodeList list;
		doc.createElement("Templates");
		for (int i = 0; i < flist.size(); ++i) {
			QFile file(dirs.at(j)+flist.at(i));
		
			QString errorStr;
			QDomDocument domDocument;
			domDocument.setContent(&file, true);
			QDomElement root = domDocument.documentElement();
			if ((root.tagName() == "labels-templates")||(root.tagName() == "Glabels-templates")) {
				list = root.elementsByTagName("Template");
				while(list.count()) {
					QString size = list.at(0).toElement().attribute("size");
					QDomNodeList list1 = doc.elementsByTagName(size);
					QDomElement n;
					if(list1.count()==0) {
						n = doc.createElement(size);
						doc.insertBefore(n,doc.firstChild());
					} else {
						n = list1.at(0).toElement();
					}
					n.insertBefore(list.at(0),n.firstChild());
				}
			}
			file.close();
		}
	}
}
