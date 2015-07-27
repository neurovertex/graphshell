#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "../sockets/textsockets.h"


namespace graphshell {
namespace boxes {
using namespace sockets;

// ########## PROCESSOR ############
class TextProcessorBox : public Box
{
    Q_OBJECT
protected:
    TextInputSocket *input;
    TextOutputSocket *output;
    TextProcessorBox(QString typeName, QString *inname=new QString("in"), QString *outname=new QString("out"));
};

// ########## READER ############
class TextReaderBox : public TextProcessorBox
{
    Q_OBJECT
public:
    TextReaderBox(QIODevice *in, QString outputname = "out");
private:
    QIODevice *in;
protected slots:
    void onData();
};



// ########## CONSUMER ############
class TextPrinterBox : public TextProcessorBox
{
    Q_OBJECT
public:
    TextPrinterBox(QTextStream *out);
protected:
    virtual void run() Q_DECL_OVERRIDE;
private:
    QTextStream *out;
protected slots:
    void onData();
};

}
}

#endif // TEXTBOX_H
