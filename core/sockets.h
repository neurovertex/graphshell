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
class Pipe : public QObject
{
    friend class GraphShell;
    Q_OBJECT
public:
    ~Pipe();
    InputSocket *getInput() { return input; }
    OutputSocket *getOutput() { return output; }
private:
    explicit Pipe(GraphShell *parent, InputSocket *input, OutputSocket *output);
    InputSocket *input;
    OutputSocket *output;
signals:

public slots:
};


//########## SOCKET ##########
class Socket : public QObject
{
    friend class Box;
    Q_OBJECT
public:
    explicit Socket(DataType *type, QString name);
    ~Socket();
    Box *getBox() { return box; }
    virtual bool isConnected() { return connectingPipe != nullptr; }
    virtual bool isInput() = 0;
    virtual DataType* getType() { return type; }
private:
    Box *box;
    DataType *type;
protected:
    virtual void setParent(Box * box);
    virtual void disconnectSocket() = 0;
    Pipe *connectingPipe = nullptr;
public slots:
};

//########## INPUT SOCKET ##########
class InputSocket : public Socket {
    friend class Pipe;
    friend class Box;
    Q_OBJECT
public:
    virtual OutputSocket* getConnected() { return isConnected() ? connectingPipe->getOutput() : nullptr; }
    virtual bool canConnect(OutputSocket *socket) = 0;
    bool isInput() { return false; }
protected:
    explicit InputSocket(DataType *type, QString name);
    ~InputSocket();
    // Has to be here because derived classes won't have access to protected OutputSocket::disconnectSocket()
    void disconnectOutput();
    void connectOutput();
    virtual void disconnectSocket() = 0;
    virtual void connectSocket(Pipe *pipe) = 0;
    virtual void setParent(Box *box);
private:
signals:
    void socketConnected(InputSocket*);
    void socketDisconnected(InputSocket*);
};


//########## OUTPUT SOCKET ##########
class OutputSocket : public Socket {
    friend class Box;
    friend class InputSocket;
    Q_OBJECT
public:
    virtual InputSocket* getConnected() { return isConnected() ? connectingPipe->getInput() : nullptr; }
    bool isInput() Q_DECL_OVERRIDE { return true; }
protected:
    explicit OutputSocket(DataType *type, QString name);
    ~OutputSocket();
    virtual void disconnectSocket() = 0;
    virtual void connectSocket(Pipe *pipe) = 0;
    virtual void setParent(Box *box) Q_DECL_OVERRIDE;
signals:
    void socketConnected(OutputSocket*);
    void socketDisconnected(OutputSocket*);
};


}

#include "graphshell.h"
#include "circularbuffer.h"

#endif // SOCKETS_H

