#include "graphshell.h"

namespace graphshell {

	GraphShell::GraphShell(QObject *parent, QString name) : QObject(parent)
	{
		setObjectName(name);
    }

}
