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
    Q_OBJECT
public:
    explicit Pipe(Box *parent, InputSocket *input, OutputSocket *output);
    ~Pipe();
    InputSocket *getInput() { return input; }
    OutputSocket *getOutput() { return output; }
private:
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
    explicit Socket(Box *parent, DataType *type);
    Box *getBox() { return box; }
    virtual bool isConnected() { return connectingPipe != nullptr; }
    virtual DataType& getType() { return *type; }
private:
    Box *box;
    DataType *type;
protected:
    virtual void disconnectSocket() = 0;
    Pipe *connectingPipe = nullptr;
signals:
    void socketConnected(Pipe*);
    void socketDisconnected(Pipe*);
public slots:
};

//########## INPUT SOCKET ##########
class OutputSocket;
class InputSocket : public Socket {
    Q_OBJECT
    friend class Pipe;
public:
    explicit InputSocket(Box *box, DataType *type) : Socket(box, type) {}
    virtual OutputSocket* getConnected() { return isConnected() ? connectingPipe->getOutput() : nullptr; }
    virtual bool canConnect(OutputSocket *socket) = 0;
protected:
    // Has to be here because derived classes won't have access to protected OutputSocket::disconnectSocket()
    void disconnectOutput();
    void connectOutput();
    virtual void disconnectSocket() = 0;
    virtual void connectSocket(Pipe *pipe) = 0;
};


//########## OUTPUT SOCKET ##########
class OutputSocket : public Socket {
    friend class CircularBuffer;
    Q_OBJECT
    friend class InputSocket;
public:
    explicit OutputSocket(Box *box, DataType *type) : Socket(box, type) {}
    virtual InputSocket* getConnected() { return isConnected() ? connectingPipe->getInput() : nullptr; }
protected:
    virtual void disconnectSocket() = 0;
    virtual void connectSocket(Pipe *pipe) = 0;
private:
};


}

#endif // SOCKETS_H

