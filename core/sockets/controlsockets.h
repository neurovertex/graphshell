#ifndef CONTROLSOCKETS_H
#define CONTROLSOCKETS_H
#include "../sockets.h"

namespace graphshell {
namespace sockets {

// ############# INPUT ############
class InputSignalSocket : public InputSocket
{
    Q_OBJECT
public:
    InputSignalSocket(Box *box, DataType *type, QString name);
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
    OutputSignalSocket(Box *box, DataType *type, QString name, bool latch=true, QVariant *defval = nullptr);
    const QVariant *getValue() { return &lastVal; }
private:
    bool latch;
    QVariant lastVal;
protected:
    void disconnectSocket() Q_DECL_OVERRIDE;
    void connectSocket(Pipe *pipe) Q_DECL_OVERRIDE;
public slots:
    void send(const QVariant *value);
    void sendVoid() {send(new QVariant());}
signals:
    void newValue(const QVariant *value = new QVariant());
};

}
}
#endif // CONTROLSOCKETS_H
