#ifndef GRAPHSHELL_H
#define GRAPHSHELL_H
#include <QtCore>

namespace graphshell {
class GraphShell;
class Pipe;
class Box;
}

#include "datatype.h"
#include "sockets.h"

namespace graphshell {


//########## GRAPHSHELL ##########
class GraphShell : public QObject
{
    friend class GraphManager;
    Q_OBJECT
public:
private:
    explicit GraphShell(QObject *parent, QString name);
    QList<Box*> boxes;
    QList<Pipe*> pipes;
signals:

public slots:

};

//########## GRAPHMANAGER ##########
class GraphManager : public QObject
{
    Q_OBJECT
public:
    explicit GraphManager(QObject *parent);
    GraphShell * newGraph(QString name);
    const QList<GraphShell*> getGraphs() { return graphs; }
private:
    QList<GraphShell*> graphs;
signals:
    void graphAdded(GraphShell*);
    void graphRemoved(GraphShell*);
public slots:

};



//########## BOX ##########
class Box : public QThread
{
    Q_OBJECT
public:
	virtual const QMap<QString, InputSocket*> getDataInput() { return dataInput; }
	virtual const QMap<QString, OutputSocket*> getDataOutput() { return dataOutput; }
	virtual const QMap<QString, InputSocket*> getControlInput() { return controlInput; }
	virtual const QMap<QString, OutputSocket*> getControlOutput() { return controlOutput; }
	virtual void run() Q_DECL_OVERRIDE = 0;
private:
protected:
	QMap<QString, InputSocket*>	dataInput;
	QMap<QString, OutputSocket*> dataOutput;
	QMap<QString, InputSocket*>	controlInput;
	QMap<QString, OutputSocket*> controlOutput;
	explicit Box(GraphShell *graph);
signals:

public slots:
};

}

#endif // GRAPHSHELL_H
