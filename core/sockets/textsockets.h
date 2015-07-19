#ifndef TEXTSOCKETS_H
#define TEXTSOCKETS_H

#include "../sockets.h"

namespace graphshell {
namespace sockets {

// ############# INPUT ############
class TextInputSocket : public InputSocket {
    Q_OBJECT
public:
    explicit TextInputSocket(Box *box, size_t buffersize=4096);
    virtual bool canConnect(OutputSocket *socket);
protected:
    virtual void disconnectSocket();
    virtual void connectSocket(Pipe *pipe);
    explicit TextInputSocket(Box *box, DataType *type, size_t buffersize=4096);
private:
    CircularBuffer buffer;
};


// ############# OUTPUT ############
class TextOutputSocket : public OutputSocket {
    friend class TextInputSocket;
    Q_OBJECT
public:
    explicit TextOutputSocket(Box *box);
protected:
    virtual void disconnectSocket();
    virtual void connectSocket(Pipe *pipe);
    explicit TextOutputSocket(Box *box, DataType *type);
private:
    CircularBuffer *buffer;
};

}
}

#endif
