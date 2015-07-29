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
public:
    TextInputSocket *getTextInput() { return input; }
    TextOutputSocket *getTextOutput() { return output; }
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
protected:
    void run();
private:
    QIODevice *in;
protected slots:
};



// ########## CONSUMER ############
class TextPrinterBox : public TextProcessorBox
{
    Q_OBJECT
public:
    TextPrinterBox(QIODevice &out);
protected:
    virtual void run() Q_DECL_OVERRIDE;
private:
    QIODevice &out;
protected slots:
    void onData();
    void startBox() Q_DECL_OVERRIDE;
    void stopBox() Q_DECL_OVERRIDE;
};

}
}

#endif // TEXTBOX_H
