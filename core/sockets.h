#ifndef SOCKETS_H
#define SOCKETS_H

#include <QtCore>

namespace graphshell {

class Box;
class GraphShell;
class Pipe;
class Socket;
class InputSocket;
class OutputSocket;
class DataType;

}

namespace graphshell {

//########## PIPE ##########
/*!
 * \brief The Pipe class represents a connection between two sockets. It will connect the given Input
 * and Output sockets upon creation and disconnect them upon deletion. It does not, however, actively
 * participate in any exchange of data or signals between the two.
*/
class Pipe : public QObject
{
    friend class GraphShell;
    Q_OBJECT
public:
    /*! \return the InputSocket of this Pipe */
    OutputSocket &getOutput() { return output; }
    /*! \return the InputSocket of this Pipe */
    InputSocket &getInput() { return input; }
private:
    ~Pipe();
    explicit Pipe(GraphShell &parent, InputSocket &input, OutputSocket &output);
    InputSocket &input;
    OutputSocket &output;
};


//########## SOCKET ##########
/*!
  \brief Represents an I/O endpoint of a Box.
*/
class Socket : public QObject
{
    friend class Box;
    Q_OBJECT
public:
    explicit Socket(DataType &type, QString name);
    /*! \return this Socket's parent Box */
    Box &getBox() { return *box; }
    /*! \return true if this Socket is connected, false otherwise */
    virtual bool isConnected() { return connectingPipe != nullptr; }
    /*! \return true if this is an InputSocket, false if it is an OutputSocket */
    virtual bool isInput() = 0;
    /*! \return this Socket's DataType */
    virtual DataType& getType() { return type; }
private:
    Box *box;
    DataType &type;
protected:
    virtual void setParent(Box &box);
    /*! \brief connects this Socket to via the given Pipe. What this method does is entirely up to the
     * subclass implementation.
     * \param pipe the pipe representing the newly created connection */
    virtual void connectSocket(Pipe &pipe) = 0;
    /*! \brief disconnects this Socket from its connectingPipe. What this method does is entirely up to the
     * subclass implementation. */
    virtual void disconnectSocket() = 0;
    /*! \brief Pointer to the current connection this Socket is part of, or nullptr if disconnected */
    Pipe *connectingPipe = nullptr;
signals:
    /*! \brief signal fired when this Socket is connected
     * \param socket this Socket
     * \note Interface monitoring a GraphShell for Socket connections should rather
     * listen to GraphShell::pipeAdded() */
    void socketConnected(Socket &socket);
    /*! \brief signal fired when this Socket is connected
     * \param socket this Socket
     * \note Interface monitoring a GraphShell for Socket connections should rather
     * listen to GraphShell::pipeRemoved() */
    void socketDisconnected(Socket &socket);
};

//########## INPUT SOCKET ##########
/*!
 * \brief abstract superclass of all input Socket's
 * \sa OutputSocket
 */
class InputSocket : public Socket {
    friend class Pipe;
    friend class Box;
    Q_OBJECT
public:
    /*! \return the currenly connected OutputSocket
     * \note Always test Socket::isConnected() beforehand, otherwise you might get
     * a null pointer dereferenced. */
    virtual OutputSocket& getConnected() { return connectingPipe->getOutput(); }
    /*! \param socket the OutputSocket to test for compatibility
     * \return true if this InputSocket may connect to the given OutputSocket */
    virtual bool canConnect(OutputSocket &socket) = 0;
    bool isInput() { return true; }
protected:
    explicit InputSocket(DataType &type, QString name);
    // Has to be here because derived classes won't have access to protected OutputSocket::disconnectSocket()
    void disconnectOutput();
    void connectOutput();
    virtual void setParent(Box &box);
};


//########## OUTPUT SOCKET ##########
/*!
 * \brief abstract superclass of all output Socket's
 * \sa InputSocket
 */
class OutputSocket : public Socket {
    friend class Box;
    friend class InputSocket;
    Q_OBJECT
public:
    /*! \return the currenly connected InputSocket
     * \note Always test Socket::isConnected() beforehand, otherwise you might get
     * a null pointer dereferenced. */
    virtual InputSocket &getConnected() { return connectingPipe->getInput(); }
    bool isInput() Q_DECL_OVERRIDE { return false; }
protected:
    explicit OutputSocket(DataType &type, QString name);
    virtual void setParent(Box &box) Q_DECL_OVERRIDE;
};


}

#include "graphshell.h"
#include "circularbuffer.h"

#endif // SOCKETS_H

