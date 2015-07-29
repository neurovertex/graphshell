#include "graphshell.h"

namespace graphshell {

GraphManager &GraphManager::instance = *new GraphManager();

GraphManager::GraphManager() : QObject()
{}

/*!
 * \brief Creates a new GraphShell with the given name or returns an already existing one.
 * \param name name of the desired GraphShell
 * \param created if not null, will be set to true or false according to whether
 * a new Graph has been created by the call
 * \return a GraphShell with the given name
 */
GraphShell &GraphManager::newGraph(QString name, bool *created) {
    if (created != nullptr)
        *created = false;
    if (!graphs.contains(name)) {
        graphs[name] = new GraphShell(*this, name);
        if (created != nullptr)
            *created = true;
    }
    return *graphs[name];
}


}
