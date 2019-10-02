
#include "papers.h"
#include <QtGlobal>
#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <qdebug.h>
#include <QApplication>
Papers::Papers()
	: QObject()
{
    QStringList dirs;
#ifdef Q_WS_WIN32
    dirs << "./" << QDir::homePath() + "/.qlabels/" << "./templates/" << QApplication::applicationDirPath()  + "/templates/";;
#else
    dirs << "./" << "/usr/share/qlabels/templates/" << "/usr/local/share/qlabels/templates/" << QDir::homePath() + "/.qlabels/" << "./templates/";
#endif
    for (int i = 0; i < dirs.size(); ++i) {
	QFile file(dirs.at(i)+QString("paper-sizes.xml"));
	QString errorStr;
	int errorLine;
	int errorColumn;
	if (domDocument.setContent(&file, true, &errorStr, &errorLine,	&errorColumn)) {
		QDomElement root = domDocument.firstChildElement("paper-sizes");
		list = root.elementsByTagName ("Paper-size");
	} else {
//	qDebug(tr("Parse error at line %1, column %2:\n%3"), errorLine,errorColumn,errorStr);
	}
    }
}
