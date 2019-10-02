
#ifndef TEMPLATES_H
#define TEMPLATES_H
#include <QObject>
#include <QDomDocument>
class Templates;

class Templates : public QObject
{
	Q_OBJECT
public:
	Templates();
	QDomDocument doc;
	
};

#endif
