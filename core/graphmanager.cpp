#include "graphshell.h"

namespace graphshell {

    GraphManager::GraphManager(QObject *parent) : QObject(parent)
    {

    }

    GraphShell * GraphManager::newGraph(QString name) {
        GraphShell *graph = new GraphShell(this, name);
        graphs << graph;
        return graph;
    }

}
