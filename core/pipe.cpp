#include "graphshell.h"

namespace graphshell {

/*!
 * \class Pipe::Pipe
 * \brief The Pipe class represents a connection between two sockets. It will connect the given Input
 * and Output sockets upon creation and disconnect them upon deletion. It does not, however, actively
 * participate in any exchange of data or signals between the two.
*/

/*!
 * \brief Creates a Pipe object to represent the connection between input and output. This will call
 * InputSocket::connectSocket(Pipe*)
 * \param parent
 * \param input
 * \param output
 */
Pipe::Pipe(GraphShell *parent, InputSocket *input, OutputSocket *output) : QObject(parent)
{
    this->input = input;
    this->output = output;
    input->connectSocket(this);
}

/*!
 * \brief Deletes the Pipe and disconnects the corresponding sockets
 */
Pipe::~Pipe()
{
    input->disconnectSocket();
}
}

