#include "controlsockets.h"


namespace graphshell {
namespace stdsockets {

// ########### INPUT ############

InputSignalSocket::InputSignalSocket(Box *box, DataType *type) : InputSocket(box, type) {
    if (!type->getName().startsWith("signal"))
        throw new QString("InputSignalSocket can only have type 'signal' or a subtype of ");
}

bool InputSignalSocket::canConnect(OutputSocket *socket) {
    return socket->getType().getName().startsWith("signal");
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
    emit socketConnected(pipe);
}

void InputSignalSocket::disconnectSocket() {
    disconnectOutput();
    connectingPipe = nullptr;
}

const QVariant *InputSignalSocket::getValue() {
    return isConnected() ? static_cast<OutputSignalSocket*>(connectingPipe->getOutput())->getValue() : nullptr;
}

// ########### OUTPUT ############

OutputSignalSocket::OutputSignalSocket(Box *box, DataType *type, bool latch, QVariant *defval) : OutputSocket(box, type) {
    if (!latch && defval == nullptr)
        throw new QString("Must define a value for non-latching signal sockets.");
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
    emit socketConnected(pipe);
}

void OutputSignalSocket::disconnectSocket() {
    disconnect(this, SIGNAL(newValue(const QVariant*)),
               static_cast<InputSignalSocket*>(connectingPipe->getInput()), SLOT(receiveValue(QVariant*)));
    connectingPipe = nullptr;
}


}
}

