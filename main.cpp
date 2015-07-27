#include "main.h"
#include "core/boxes/textboxes.h"

using namespace graphshell;
using namespace graphshell::gui;
using namespace graphshell::boxes;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();


    QFile file("ipsum.txt");
    if (! file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Couldn't open file : "<< file.errorString();
        qFatal("Error opening file");
    }

    GraphShell *shell = GraphManager::getInstance()->getGraph("newshell");
    Box *box = new TextReaderBox(&file);
    shell->addBox(box);
    box->setName("Reader box");

    return a.exec();
}
