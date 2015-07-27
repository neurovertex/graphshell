#include "graphshell.h"

namespace graphshell {

GraphManager *GraphManager::instance = new GraphManager();

GraphManager::GraphManager() : QObject()
{}

/*!
  \fn GraphManager::getInstance()
  \brief returns the singleton instance of GraphManager
*/

/*!
 * \brief Creates a new GraphShell with the given name. If there already is
 * one with this name, nullptr is returned.
 * \param name name of the new GraphShell
 * \return A new Graphshell with the given name, or null if there already was one.
 */
GraphShell * GraphManager::newGraph(QString name) {
    if (graphs.contains(name))
        return nullptr;
    GraphShell *graph = new GraphShell(this, name);
    graphs.insert(name, graph);
    return graph;
}

}
