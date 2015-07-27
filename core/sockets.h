#ifndef SOCKETS_H
#define SOCKETS_H

namespace graphshell {

class Socket;
class InputSocket;
class OutputSocket;
class CircularBuffer;
class CircularIODevice;

}

#include "graphshell.h"
#include "circularbuffer.h"

namespace graphshell {

//########## PIPE ##########
class InputSocket;
class OutputSocket;
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
    Q_OBJECT
public:
    explicit Socket(Box *parent, DataType *type, QString name);
    ~Socket();
    Box *getBox() { return box; }
    virtual bool isConnected() { return connectingPipe != nullptr; }
    virtual DataType* getType() { return type; }
private:
    Box *box;
    DataType *type;
protected:
    virtual void disconnectSocket() = 0;
    Pipe *connectingPipe = nullptr;
public slots:
};

//########## INPUT SOCKET ##########
class OutputSocket;
class InputSocket : public Socket {
    Q_OBJECT
    friend class Pipe;
public:
    virtual OutputSocket* getConnected() { return isConnected() ? connectingPipe->getOutput() : nullptr; }
    virtual bool canConnect(OutputSocket *socket) = 0;
protected:
    explicit InputSocket(Box *box, DataType *type, QString name);
    ~InputSocket();
    // Has to be here because derived classes won't have access to protected OutputSocket::disconnectSocket()
    void disconnectOutput();
    void connectOutput();
    virtual void disconnectSocket() = 0;
    virtual void connectSocket(Pipe *pipe) = 0;
signals:
    void socketConnected(InputSocket*);
    void socketDisconnected(InputSocket*);
};


//########## OUTPUT SOCKET ##########
class OutputSocket : public Socket {
    friend class CircularBuffer;
    Q_OBJECT
    friend class InputSocket;
public:
    virtual InputSocket* getConnected() { return isConnected() ? connectingPipe->getInput() : nullptr; }
protected:
    explicit OutputSocket(Box *box, DataType *type, QString name);
    ~OutputSocket();
    virtual void disconnectSocket() = 0;
    virtual void connectSocket(Pipe *pipe) = 0;
signals:
    void socketConnected(OutputSocket*);
    void socketDisconnected(OutputSocket*);
};


}

#endif // SOCKETS_H

