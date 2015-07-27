#include "textboxes.h"

namespace graphshell {
namespace boxes {
using namespace sockets;

// ########## PRODUCER ############
TextProcessorBox::TextProcessorBox(QString typeName, QString *inname, QString *outname) :
    Box(typeName), input(nullptr), output(nullptr)
{
    if (outname != nullptr) {
        this->output = new TextOutputSocket(*outname);
        addSocket(output, DATA);
    }
    if (inname != nullptr) {
        this->input = new TextInputSocket(*inname);
        addSocket(input, DATA);
    }
}


// ########## READER ############
TextReaderBox::TextReaderBox(QIODevice *in, QString outputname) :
    TextProcessorBox("TextReaderBox", nullptr, &outputname)
{
    this->in = in;
    connect(in, &QIODevice::readyRead, this, &TextReaderBox::onData);
}

void TextReaderBox::onData()
{
    while (in->bytesAvailable() > 0) {
        QByteArray data = in->read(in->bytesAvailable());
        if (output->isConnected())
            output->getDevice()->write(data);
    }
}



// ########## PRINTER ############
TextPrinterBox::TextPrinterBox(QTextStream *out) :
    TextProcessorBox("TextPrinterBox"), out(out)
{
    connect(this->input->getDevice(), &QIODevice::readyRead,
            this, &TextPrinterBox::onData);

}

void TextPrinterBox::onData()
{
    QIODevice *dev = input->getDevice();
    while (dev->bytesAvailable() > 0) {
        QByteArray data = dev->read(dev->bytesAvailable());
        *out << data;
        if (output->isConnected())
            output->getDevice()->write(data);
    }

}

void TextPrinterBox::run()
{
    exec();
}


}
}
