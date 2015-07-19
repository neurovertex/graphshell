#include "textboxes.h"

namespace graphshell {
namespace boxes {
using namespace sockets;

// ########## PRODUCER ############
TextProducerBox::TextProducerBox(GraphShell *graph, TextOutputSocket *output, QString outputName) : Box(graph)
{
    this->output = (output != nullptr) ? output : new TextOutputSocket(this);
    dataOutput.insert(outputName, output);
}

// ########## CONSUMER ############
TextConsumerBox::TextConsumerBox(GraphShell *graph, TextInputSocket *input, QString inputName) : Box(graph)
{
    this->input = (input != nullptr) ? input : new TextInputSocket(this);
    dataInput.insert(inputName, input);
}


}
}
