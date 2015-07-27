#include "textsockets.h"

namespace graphshell {
namespace sockets {

// ############# INPUT ############
TextInputSocket::TextInputSocket(QString name, size_t buffersize) :
    InputSocket(DataType::getType("/text/"), name), buffer(this, buffersize)
{
}


TextInputSocket::TextInputSocket(DataType *type, QString name, size_t buffersize) :
    InputSocket(type, name), buffer(this, buffersize)
{
}

bool TextInputSocket::canConnect(OutputSocket *socket)
{
    return socket->getType()->subtypeOf(getType());
}

void TextInputSocket::disconnectSocket()
{
    emit socketDisconnected(this);
    disconnectOutput();
    connectingPipe = nullptr;
}

void TextInputSocket::connectSocket(Pipe *pipe)
{
    if (isConnected())
        disconnectSocket();
    connectingPipe = pipe;
    emit socketConnected(this);
}

// ############# OUTPUT ############
TextOutputSocket::TextOutputSocket(QString name) :
    OutputSocket(DataType::getType("/text/"), name), buffer(nullptr)
{
}


TextOutputSocket::TextOutputSocket(DataType *type, QString name) :
    OutputSocket(type, name), buffer(nullptr)
{
}

void TextOutputSocket::connectSocket(Pipe *pipe) {
    connectingPipe = pipe;
    buffer = static_cast<CircularBuffer*>(static_cast<TextInputSocket*>(pipe->getInput())->getDevice());
    emit socketConnected(this);
}

void TextOutputSocket::disconnectSocket() {
    emit socketDisconnected(this);
    connectingPipe = nullptr;
    buffer = nullptr;
}

}
}
