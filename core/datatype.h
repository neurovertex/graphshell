#ifndef DATATYPE_H
#define DATATYPE_H

namespace graphshell {
class DataType;
}

#include "graphshell.h"

namespace graphshell {

	class DataType : public QObject
	{
		Q_OBJECT
	public:
        bool canAccept(DataType &t);
        QString& getName() { return name; }
        bool subtypeOf(DataType* type);
		static bool hasType(QString type) { return typeRegistry->contains(type); }
		static DataType * getType(QString name);
    private:
		explicit DataType(QString name);
		QString name;
        static QHash<QString, DataType*> *typeRegistry;
	signals:

	public slots:
	};

}

#endif // DATATYPE_H
