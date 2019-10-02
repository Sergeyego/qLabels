
#ifndef PAPERS_H
#define PAPERS_H
#include <QObject>
#include <QDomDocument>
class Papers;

class Papers : public QObject
{
	Q_OBJECT
public:
	Papers();
	QDomDocument domDocument;
	QDomNodeList list;	
};

#endif
