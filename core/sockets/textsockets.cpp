#include "textsockets.h"

namespace graphshell {
namespace sockets {

// ############# INPUT ############
TextInputSocket::TextInputSocket(Box *box, size_t buffersize) :
    InputSocket(box, DataType::getType("text")), buffer(this, buffersize)
{

}


TextInputSocket::TextInputSocket(Box *box, DataType *type, size_t buffersize) :
    InputSocket(box, type), buffer(this, buffersize)
{

}

bool TextInputSocket::canConnect(OutputSocket *socket)
{
    return socket->getType().subtypeOf(getType());
}

void TextInputSocket::disconnectSocket()
{
    disconnectOutput();
    connectingPipe = nullptr;
}

void TextInputSocket::connectSocket(Pipe *pipe)
{
    if (isConnected())
        disconnectSocket();
    connectingPipe = pipe;
}

// ############# OUTPUT ############
TextOutputSocket::TextOutputSocket(Box *box) :
    OutputSocket(box, DataType::getType("text")), buffer(nullptr)
{
}


TextOutputSocket::TextOutputSocket(Box *box, DataType *type) :
    OutputSocket(box, type), buffer(nullptr)
{
}

void TextOutputSocket::connectSocket(Pipe *pipe) {
    connectingPipe = pipe;
}

void TextOutputSocket::disconnectSocket() {
    connectingPipe = nullptr;
}

}
}
