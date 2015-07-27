#ifndef TEXTSOCKETS_H
#define TEXTSOCKETS_H

#include "../sockets.h"

namespace graphshell {
namespace sockets {

// ############# INPUT ############
class TextInputSocket : public InputSocket {
    Q_OBJECT
public:
    explicit TextInputSocket(QString name, size_t buffersize=4096);
    virtual bool canConnect(OutputSocket *socket);
    QIODevice *getDevice() { return &buffer; }
protected:
    virtual void disconnectSocket();
    virtual void connectSocket(Pipe *pipe);
    explicit TextInputSocket(DataType *type, QString name, size_t buffersize=4096);
private:
    CircularBuffer buffer;
};


// ############# OUTPUT ############
class TextOutputSocket : public OutputSocket {
    friend class TextInputSocket;
    Q_OBJECT
public:
    explicit TextOutputSocket(QString name);
    QIODevice *getDevice() { return buffer; }
protected:
    explicit TextOutputSocket(DataType *type, QString name);
    virtual void disconnectSocket();
    virtual void connectSocket(Pipe *pipe);
private:
    CircularBuffer *buffer;
};

}
}

#endif
