#ifndef CONTROLSOCKETS_H
#define CONTROLSOCKETS_H
#include "../sockets.h"

namespace graphshell {
namespace stdsockets {

// ############# INPUT ############
class InputSignalSocket : public InputSocket
{
    Q_OBJECT
public:
    InputSignalSocket(Box *box, DataType *type);
    bool canConnect(OutputSocket *socket) Q_DECL_OVERRIDE;
    const QVariant *getValue();
protected:
    void disconnectSocket() Q_DECL_OVERRIDE;
    void connectSocket(Pipe *pipe) Q_DECL_OVERRIDE;
public slots:
    void receiveValue(const QVariant *value);
signals:
    void valueReceived(InputSignalSocket *source, const QVariant *value);
};



// ############# OUTPUT ############
class OutputSignalSocket : public OutputSocket
{
    Q_OBJECT
public:
    OutputSignalSocket(Box *box, DataType *type, bool latch=true, QVariant *defval = nullptr);
    const QVariant *getValue() { return &lastVal; }
private:
    bool latch;
    QVariant lastVal;
    void send(const QVariant *value);
protected:
    void disconnectSocket() Q_DECL_OVERRIDE;
    void connectSocket(Pipe *pipe) Q_DECL_OVERRIDE;
signals:
    void newValue(const QVariant *value);
};

}
}
#endif // CONTROLSOCKETS_H
