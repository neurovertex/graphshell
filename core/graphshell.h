#ifndef GRAPHSHELL_H
#define GRAPHSHELL_H
#include <QtCore>

namespace graphshell {
class GraphShell;
class GraphManager;
class Pipe;
class Box;
}

#include "datatype.h"
#include "sockets.h"

namespace graphshell {


//########## GRAPHSHELL ##########
/*!
 * \brief Represents a graph. Has a list of existing pipes and boxes, controls the execution by
 * starting and stopping the boxes when relevant.
 */

class GraphShell : public QObject
{
    friend class GraphManager;
    Q_OBJECT
public:
    Pipe &connectSockets(OutputSocket &out, InputSocket &in);
    bool disconnectSockets(Pipe &pipe);
    void addBox(Box &box);
    bool removeBox(Box &box);
    /*!
     * \return the boxes of this GraphShell
     */
    const QList<Box*> &getBoxes() {return boxes;}
    /*!
     * \return the pipes of this GraphShell
     */
    const QList<Pipe*> &getPipes() {return pipes;}
private:
    explicit GraphShell(GraphManager &parent, QString name);
    QList<Box*> boxes;
    QList<Pipe*> pipes;
signals:
    /*! \brief signal fired when a box is added to the graph
     * \param box the newly added box */
    void boxAdded(Box &box);
    /*! \brief signal fired when a box is removed from the graph
     * \param box the removed box */
    void boxRemoved(Box &box);
    /*! \brief signal fired when a pipe is added to the graph
     * \param pipe the newly added pipe */
    void pipeAdded(Pipe &pipe);
    /*! \brief signal fired when a pipe is removed from the graph
     *  \param pipe the removed pipe */
    void pipeRemoved(Pipe &pipe);
public slots:
    void start();
    void stop();
    void terminate();
};

//########## GRAPHMANAGER ##########
/*!
 * \brief Singleton class that manages the creation/loading of GraphShell objects
 */
class GraphManager : public QObject
{
    Q_OBJECT
public:
    /*! \brief returns the singleton instance of GraphManager */
    static GraphManager &getInstance() { return instance; }
    GraphShell &newGraph(QString name, bool *created = nullptr);
    /*! \return the corresponding GraphShell if exiting, nullptr if there is non with the given name. */
    GraphShell *getGraph(QString name) { return graphs.value(name); }
    /*! \return a const map corresponding to all registered GraphShell's indexed by name */
    const QHash<QString, GraphShell*> &getGraphs() { return graphs; }
private:
    static GraphManager &instance;
    QHash<QString, GraphShell*> graphs;
    explicit GraphManager();
signals:
    /*! \brief signal fired when a new graph is created
     * \param graph the newly created graph */
    void graphAdded(GraphShell& graph);
    /*! \brief signal fired when a new graph is removed
     * \param graph the newly removed graph */
    void graphRemoved(GraphShell& graph);
public slots:

};



//########## BOX ##########
/*!
 * \brief Base class for the nodes of the execution graph
 * \ingroup Core
 * \ingroup Boxes
 *
 * This class is the basis of all processing in GraphShell. It has I/O sockets for
 * exchanging information with other boxes. Sockets are divided into 2 categories :
 * "Data" Input/Output sockets are for the process of the box to consume/process/produce
 * "Control" Input/Output sockets are "meta" to the box : they control its execution
 * and give information about it to other boxes. Although this is a purely conceptual
 * difference, in practice boxes may  be redefined to use either as desired.
 */
class Box : public QThread
{
    friend class GraphShell;
    Q_OBJECT
    /*! \property Box::autostart
     * \brief States if this box should be started when its parent Graph is started */
    Q_PROPERTY(bool autostart READ autostarts)
    /*! \property Box::typeName
     * \brief a constant string designating the type of the Box */
    Q_PROPERTY(QString typeName READ getTypeName)
public:
    /*! \return a map of the Box's Data InputSocket's indexed by name */
    virtual const QHash<QString, InputSocket*> &getDataInput() { return dataInput; }
    /*! \return a map of the Box's Data OutputSocket's indexed by name */
    virtual const QHash<QString, OutputSocket*> &getDataOutput() { return dataOutput; }
    /*! \return a map of the Box's Control InputSocket's indexed by name */
    virtual const QHash<QString, InputSocket*> &getControlInput() { return controlInput; }
    /*! \return a map of the Box's Control OutputSocket's indexed by name */
    virtual const QHash<QString, OutputSocket*> &getControlOutput() { return controlOutput; }
    /*! \return the parent GraphShell this Box belongs to */
    GraphShell *getShell() { return this->shell; }

    /*! \return Box::autostart */
    bool autostarts() const {return autostart;}
    /*! \sa Box::autostarts() */
    void setAutoStart(bool autostart) {this->autostart = autostart;}
    /*! \return Box::typeName */
    QString getTypeName() const { return typeName; }

    /*! \brief input flag for a Socket. In most cases - but not all - the Socket is passed
     * along with the flags, and the in/output-ness can be tested direclty (Socket::isInput()),
     * thus ignoring the flag. */
    static const unsigned int INPUT = 0x1;
    /*! \brief output flag for a Socket. In most cases - but not all - the Socket is passed
     * along with the flags, and the in/output-ness can be tested direclty (Socket::isInput()),
     * thus ignoring the flag. */
    static const unsigned int OUTPUT = 0x2;
    /*! \brief data flag for a Socket */
    static const unsigned int DATA = 0x4;
    /*! \brief control flag for a Socket */
    static const unsigned int CONTROL = 0x8;
private:
    QString typeName;
    GraphShell *shell;
    bool autostart;
    QHash<QString, InputSocket*>	dataInput;
    QHash<QString, OutputSocket*> dataOutput;
    QHash<QString, InputSocket*>	controlInput;
    QHash<QString, OutputSocket*> controlOutput;
    void setParent(GraphShell &shell);
    void addInputSocket(InputSocket &socket, unsigned int flags = DATA);
    void addOutputSocket(OutputSocket &socket, unsigned int flags = DATA);
    bool removeInputSocket(InputSocket &socket, unsigned int flags);
    bool removeOutputSocket(OutputSocket &socket, unsigned int flags);
protected:
    /*! \brief Adds a socket to the box according to flags. Template function so you can
     * write SomethingSocket &sock = addSocket(new SomethingSocket(), flags) without needing a cast.
     * \param sock the Socket to add
     * \param flags sets whether the socket is data (Box::DATA) or control (Box::CONTROL).
     * Box::INPUT and Box::OUTPUT flags are ignored and deduced from Socket::isInput()
     * \return socket */
    template <class T>
    T &addSocket(T &sock, unsigned int flags = DATA);
    bool removeSocket(Socket &sock, unsigned int flags = DATA);
    explicit Box(QString typeName, bool autostart = false);
    ~Box();
signals:
    /*! \brief signal fired when a new Socket is added to the Box
     * \param socket The newly-added socket
     * \param flags Tells whether this socket is Data or Control */
    void socketAdded(Socket &socket, unsigned int flags);
    /*! \brief signal fired when a Socket is removed from the box. The socket is deleted
     * immediately after this so this signal *must* be connected with Qt::BlockingQueuedConnection
     * across threads, or Qt::DirectConnection in the same thread.
     * \param socket the removed socket */
    void socketRemoved(Socket &socket);
public slots:
    /*! \brief Starts the box. By default, this starts the thread this box implements. */
    virtual void startBox();
    /*! \brief Starts the box. By default, this tells the thread this box implements to exit.*/
    virtual void stopBox();
protected slots:
    /*! \brief Virtual slot called when a socket of the box connects to another.
     * Default implementation does nothing */
    virtual void socketConnected(Socket&) {}
    /*! \brief Virtual slot called when a socket of the box disconnects from another.
     * Default implementation does nothing */
    virtual void socketDisconnected(Socket&) {}
};


template <class T>
T &Box::addSocket(T &sock, unsigned int flags)
{
    Socket &socket = (Socket&)sock;
    if (socket.isInput())
        addInputSocket((InputSocket&)socket, flags | INPUT);
    else
        addOutputSocket((OutputSocket&)socket, flags | OUTPUT);
    connect(&sock, &OutputSocket::socketConnected,
            this, &Box::socketConnected);
    connect(&sock, &OutputSocket::socketDisconnected,
            this, &Box::socketDisconnected);
    return sock;
}

}

#endif // GRAPHSHELL_H
