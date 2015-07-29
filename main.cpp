#include "main.h"
#include "core/boxes/textboxes.h"

using namespace graphshell;
using namespace graphshell::gui;
using namespace graphshell::boxes;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qsrand(QDateTime::currentDateTime().toTime_t());
    MainWindow w;
    w.show();


    QFile file("ipsum.txt");
    if (! file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Couldn't open file : "<< file.errorString();
        qFatal("Error opening file");
    }

    GraphShell *shell = GraphManager::getInstance().getGraph("newshell");

    Box *reader = new TextReaderBox(&file);
    reader->setAutoStart(true);
    shell->addBox(*reader);
    reader->setObjectName("Reader");

    return a.exec();
}
