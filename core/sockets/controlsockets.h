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
    InputSignalSocket(DataType &type, QString name);
    bool canConnect(OutputSocket &socket) Q_DECL_OVERRIDE;
    const QVariant *getValue();
protected:
    void disconnectSocket() Q_DECL_OVERRIDE;
    void connectSocket(Pipe &pipe) Q_DECL_OVERRIDE;
public slots:
    void receiveValue(const QVariant &value);
signals:
    void valueReceived(InputSignalSocket &source, const QVariant &value);
};



// ############# OUTPUT ############
class OutputSignalSocket : public OutputSocket
{
    Q_OBJECT
public:
    OutputSignalSocket(DataType &type, QString name, bool latch=true, QVariant *defval = nullptr);
    const QVariant &getValue() { return lastVal; }
private:
    bool latch;
    QVariant lastVal;
    static const QVariant VOIDSIGNAL;
protected:
    void disconnectSocket() Q_DECL_OVERRIDE;
    void connectSocket(Pipe &pipe) Q_DECL_OVERRIDE;
public slots:
    void send(const QVariant &value);
    void sendVoid() {send(VOIDSIGNAL);}
signals:
    void newValue(const QVariant &value = VOIDSIGNAL);
};

}
}
#endif // CONTROLSOCKETS_H
