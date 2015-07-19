#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "../sockets/textsockets.h"


namespace graphshell {
	namespace boxes {
        using namespace sockets;

        // ########## PRODUCER ############
        class TextProducerBox : public Box
        {
        protected:
            TextProducerBox(GraphShell *graph, TextOutputSocket *output = nullptr, QString outputName = "out");
            TextOutputSocket *output;
        };


        // ########## CONSUMER ############
        class TextConsumerBox : public Box
        {
        protected:
            TextConsumerBox(GraphShell *graph, TextInputSocket *input = nullptr, QString inputName = "in");
            TextInputSocket *input;
        };


        // ########## CONSUMER ############
        class TextProcessorBox : public TextConsumerBox, public TextProducerBox {
        protected:
            TextProcessorBox(GraphShell *graph) : TextConsumerBox(graph), TextProducerBox(graph) {}
        };

	}
}

#endif // TEXTBOX_H
