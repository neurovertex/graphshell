#include "textsockets.h"

namespace graphshell {
namespace sockets {

// ############# INPUT ############
TextInputSocket::TextInputSocket(Box *box, QString name, size_t buffersize) :
    InputSocket(box, DataType::getType("/text/"), name), buffer(this, buffersize)
{
}


TextInputSocket::TextInputSocket(Box *box, DataType *type, QString name, size_t buffersize) :
    InputSocket(box, type, name), buffer(this, buffersize)
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
TextOutputSocket::TextOutputSocket(Box *box, QString name) :
    OutputSocket(box, DataType::getType("/text/"), name), buffer(nullptr)
{
}


TextOutputSocket::TextOutputSocket(Box *box, DataType *type, QString name) :
    OutputSocket(box, type, name), buffer(nullptr)
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
