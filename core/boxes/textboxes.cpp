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
        addSocket(*output, DATA);
    }
    if (inname != nullptr) {
        this->input = new TextInputSocket(*inname);
        addSocket(*input, DATA);
    }
}


// ########## READER ############
TextReaderBox::TextReaderBox(QIODevice *in, QString outputname) :
    TextProcessorBox("TextReaderBox", nullptr, &outputname)
{
    this->in = in;
}

void TextReaderBox::run()
{
    connect(in, &QIODevice::readyRead,
            [=](){ this->exit(0); });
    char *buf = new char[1024];
    while (!in->atEnd()) {
        in->waitForReadyRead(100);
        while (in->bytesAvailable() > 0) {
            auto size = in->read(buf, qMin((qint64)1024, in->bytesAvailable()));
            if (output->isConnected())
                output->getDevice()->write(buf, size);
        }
        if (!in->atEnd())
            exec();
    }
    delete buf;
    output->getDevice()->setEndOfStream();
    qDebug() << this <<": Finished reading input device";
    stopBox();
}


// ########## PRINTER ############
TextPrinterBox::TextPrinterBox(QIODevice &out) :
    TextProcessorBox("TextPrinterBox"), out(out)
{
    qDebug() << this << " created with device "<< ((QFile&)out).fileName();
    connect(&this->input->getDevice(), &QIODevice::readyRead,
            this, &TextPrinterBox::onData);
}

void TextPrinterBox::onData()
{
    QIODevice &dev = input->getDevice();
    if (!out.isOpen())
        qFatal("Output device isn't open");
    while (dev.bytesAvailable() > 0) {
        QByteArray data = dev.read(dev.bytesAvailable());
        out.write(data);
        if (output->isConnected())
            output->getDevice()->write(data);
    }
    if (input->getDevice().atEnd())
        stopBox();
}

void TextPrinterBox::startBox()
{
    if (!out.isOpen())
        if (!out.open(QIODevice::WriteOnly | QIODevice::Text))
            return;
    TextProcessorBox::startBox();
}

void TextPrinterBox::stopBox()
{
    TextProcessorBox::stopBox();
}

void TextPrinterBox::run()
{
    exec();
    out.waitForBytesWritten(-1);
    out.close();
}


}
}
