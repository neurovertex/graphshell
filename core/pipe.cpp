#include "graphshell.h"

namespace graphshell {
	Pipe::Pipe(Box *parent, InputSocket *input, OutputSocket *output) : QObject(parent)
	{
		this->input = input;
		this->output = output;
		input->connectSocket(this);
	}

	Pipe::~Pipe()
	{
		input->disconnectSocket();
	}
}

