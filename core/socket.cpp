#include "sockets.h"

namespace graphshell {

/*!
  \class Socket
  \brief Represents an I/O endpoint of a Box.
*/

/*!
 * \brief Creates a new Socket
 * \param parent The Box this Socket will belong to
 * \param type the DataType of the Socket
 * \param name The name of the Socket
 */
Socket::Socket(Box *parent, DataType *type, QString name) : QObject(parent)
{
    this->box = parent;
    this->type = type;
    setObjectName(name);
}

Socket::~Socket()
{
}

/*!
 * \brief Creates a new InputSocket
 * \param box The Box this Socket will belong to
 * \param type The DataType of the Socket
 * \param name The name of the Socket
 */
InputSocket::InputSocket(Box *box, DataType *type, QString name) :
    Socket(box, type, name)
{
    connect(this, &InputSocket::socketConnected,
            box, &Box::socketConnected);
    connect(this, &InputSocket::socketDisconnected,
            box, &Box::socketDisconnected);
}

InputSocket::~InputSocket()
{
}

/*!
 * \fn InputSocket::connectSocket(Pipe*)
 * \brief Connects the InputSocket to an OutputSocket via a Pipe. If already connected the socket
 * will disconnect first.
 * \param pipe The connecting pipe
 */

/*!
 * \fn InputSocket::disconnectSocket()
 * \brief Disconnects the InputSocket from its current connectingPipe if any. This should only be
 * callse by Pipe::~Pipe() and will call OutputSocket::disconnectSocket()
 */

/*!
 * \brief Calls the connected OutputSocket's - if any - OutputSocket::connectSocket(Pipe*).
 * This exists because OutputSocket::connectSocket(Pipe*) is protected, and InputSocket is
 * a friend class, but its subclasses aren't. Thus connectOutput() "relays" friendliness
 * to subclasses.
 * \sa InputSocket::disconnectOutput()
 */

void InputSocket::connectOutput() {
    if (connectingPipe != nullptr)
        connectingPipe->getOutput()->connectSocket(connectingPipe);
}

/*!
 * \brief Calls the connected OuputSocket's - if any - OutputSocket::disconnectSocket().
 * \sa InputSocket::connectOutput()
 */
void InputSocket::disconnectOutput() {
    if (connectingPipe != nullptr)
        connectingPipe->getOutput()->disconnectSocket();
}

/*!
 * \brief Creates a new OutputSocket
 * \param box The Box this Socket will belong to
 * \param type The DataType of the Socket
 * \param name The name of the Socket
 */
OutputSocket::OutputSocket(Box *box, DataType *type, QString name) :
    Socket(box, type, name)
{
    connect(this, &OutputSocket::socketConnected,
            box, &Box::socketConnected);
    connect(this, &OutputSocket::socketDisconnected,
            box, &Box::socketDisconnected);
}

OutputSocket::~OutputSocket()
{
}

}
