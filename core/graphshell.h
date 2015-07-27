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
    Pipe *connect(InputSocket *in, OutputSocket *out);
    bool disconnect(Pipe *pipe);
    void addBox(Box *box);
    bool removeBox(Box *box);
private:
    explicit GraphShell(QObject *parent, QString name);
    QList<Box*> boxes;
    QList<Pipe*> pipes;
signals:
    void boxAdded(Box* graph);
    void boxRemoved(Box* graph);
    void pipeAdded(Pipe *pipe);
    void pipeRemoved(Pipe *pipe);
public slots:
    void start();
    void stop();
};

//########## GRAPHMANAGER ##########
/*!
 * \class GraphManager
 * \brief Singleton class that manages the creation/loading of GraphShell objects
 */
class GraphManager : public QObject
{
    Q_OBJECT
public:
    static GraphManager *getInstance() { return instance; }
    GraphShell * newGraph(QString name);
    GraphShell *getGraph(QString name) { return graphs.value(name); }
    const QHash<QString, GraphShell*> *getGraphs() { return &graphs; }
private:
    static GraphManager *instance;
    QHash<QString, GraphShell*> graphs;
    explicit GraphManager();
signals:
    void graphAdded(GraphShell*);
    void graphRemoved(GraphShell*);
public slots:

};



//########## BOX ##########
class Box : public QThread
{
    friend class GraphShell;
    Q_OBJECT
    Q_PROPERTY(bool autostart READ autostarts)
    Q_PROPERTY(QString typeName READ getTypeName)
public:
    virtual const QHash<QString, InputSocket*> getDataInput() { return dataInput; }
    virtual const QHash<QString, OutputSocket*> getDataOutput() { return dataOutput; }
    virtual const QHash<QString, InputSocket*> getControlInput() { return controlInput; }
    virtual const QHash<QString, OutputSocket*> getControlOutput() { return controlOutput; }
    GraphShell *getShell() { return this->shell; }
    bool autostarts() const {return autostart;}
    QString getTypeName() const { return typeName; }
    virtual void setName(QString name) { setObjectName(name); }
    static const int INPUT = 0x1;
    static const int OUTPUT = 0x2;
    static const int DATA = 0x4;
    static const int CONTROL = 0x8;
private:
    QString typeName;
    GraphShell *shell;
    bool autostart;
    QHash<QString, InputSocket*>	dataInput;
    QHash<QString, OutputSocket*> dataOutput;
    QHash<QString, InputSocket*>	controlInput;
    QHash<QString, OutputSocket*> controlOutput;
    void setParent(GraphShell *shell);
    void addInputSocket(InputSocket *socket, int flags = DATA);
    void addOutputSocket(OutputSocket *socket, int flags = DATA);
protected:
    template <class T>
    T *addSocket(T *sock, int flags = DATA)
    {
        if (((Socket*)sock)->isInput())
            addInputSocket((InputSocket*)sock, (flags | INPUT) & ~OUTPUT);
        else
            addOutputSocket((OutputSocket*)sock, (flags | OUTPUT) & ~INPUT);
        return sock;
    }

    bool removeSocket(InputSocket *socket, int flags);
    bool removeSocket(OutputSocket *socket, int flags);

    explicit Box(QString typeName, bool autostart = false);
    ~Box();
    void setAutoStart(bool autostart) {this->autostart = autostart;}
signals:
    void socketAdded(Socket *, int flags);
    void socketRemoved(Socket *);
public slots:
    virtual void socketConnected(Socket*) {}
    virtual void socketDisconnected(Socket*) {}
    virtual void startBox();
    virtual void stopBox();
};

}

#endif // GRAPHSHELL_H
