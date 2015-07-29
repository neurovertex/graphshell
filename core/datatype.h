#ifndef DATATYPE_H
#define DATATYPE_H

namespace graphshell {
class DataType;
}

#include "graphshell.h"

namespace graphshell {

/*!
 * \brief Describes a type of data for sockets to tell if they can connect to one another.
 * Also implements a minimal hierarchy system, deduced from their name. A DataType name is
 * of the form /foo/, /foo/bar/ ... where the latter is a subtype of the former.
 * Subtypes imply that a consumer of a given type can accept any subtype. For example, a
 * plain text (/text/) can accept a xml stream producer (/text/xml/), though it won't work
 * the other way around.
 */
class DataType : public QObject
{
    Q_OBJECT
public:
    bool canAccept(DataType &t);
    bool subtypeOf(DataType& type);
    /*! \return true if a type with the given name already exists in the registry */
    static bool hasType(QString type) { return typeRegistry.contains(validate(type)); }
    static DataType &getType(QString name);
private:
    static QString validate(const QString& in);
    explicit DataType(QString name);
    static QHash<QString, DataType*> typeRegistry;
signals:

public slots:
};

}

#endif // DATATYPE_H
