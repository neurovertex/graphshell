#include "controlsockets.h"


namespace graphshell {
namespace sockets {

// ########### INPUT ############

InputSignalSocket::InputSignalSocket(DataType *type, QString name) :
    InputSocket(type, name)
{
    if (!type->subtypeOf(DataType::getType("/signal/")))
        qFatal("InputSignalSocket can only have type 'signal' or a subtype of ");
}

bool InputSignalSocket::canConnect(OutputSocket *socket) {
    return socket->getType()->subtypeOf(DataType::getType("/signal/"));
}

void InputSignalSocket::receiveValue(const QVariant *value) {
    emit valueReceived(this, value);
}

void InputSignalSocket::connectSocket(Pipe *pipe)
{
    if (isConnected())
        disconnectSocket();
    connectingPipe = pipe;
    connectOutput();
    emit socketConnected(this);
}

void InputSignalSocket::disconnectSocket() {
    emit socketDisconnected(this);
    disconnectOutput();
    connectingPipe = nullptr;
}

const QVariant *InputSignalSocket::getValue() {
    return isConnected() ? static_cast<OutputSignalSocket*>(connectingPipe->getOutput())->getValue() : nullptr;
}

// ########### OUTPUT ############

OutputSignalSocket::OutputSignalSocket(DataType *type, QString name, bool latch, QVariant *defval) :
    OutputSocket(type, name)
{
    if (!latch && defval == nullptr)
        qFatal("Must define a value for non-latching signal sockets.");
    this->lastVal = *defval;
    this->latch = latch;
}

void OutputSignalSocket::send(const QVariant *value) {
    if (latch)
        this->lastVal = *value;
    emit newValue(value);
}

void OutputSignalSocket::connectSocket(Pipe *pipe) {
    if (isConnected())
        disconnectSocket();
    connectingPipe = pipe;
    connect(this,	SIGNAL(newValue(const QVariant*)),
            static_cast<InputSignalSocket*>(pipe->getInput()), SLOT(receiveValue(QVariant*)));
    emit socketConnected(this);
}

void OutputSignalSocket::disconnectSocket() {
    emit socketDisconnected(this);
    disconnect(this, SIGNAL(newValue(const QVariant*)),
               static_cast<InputSignalSocket*>(connectingPipe->getInput()), SLOT(receiveValue(QVariant*)));
    connectingPipe = nullptr;
}


}
}

