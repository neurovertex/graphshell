#include "sockets.h"

namespace graphshell {

	Socket::Socket(Box *parent, DataType *type) : QObject(parent)
	{
		this->box = parent;
		this->type = type;
	}

	void InputSocket::disconnectOutput() {
		if (connectingPipe != nullptr)
			connectingPipe->getOutput()->disconnectSocket();
	}

	void InputSocket::connectOutput() {
		if (connectingPipe != nullptr)
			connectingPipe->getOutput()->connectSocket(connectingPipe);
	}

}
