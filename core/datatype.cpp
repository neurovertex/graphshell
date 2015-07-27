#include "datatype.h"

namespace graphshell {

QHash<QString, DataType *> *DataType::typeRegistry = new QHash<QString, DataType *>();

/*!
 * \brief Creates a new DataType from a name.
 * \param name The Name of the DataType.
 */
DataType::DataType(QString name) : QObject()
{
    this->name = name;
}

/*!
 * \brief Implements equality test between two DataTypes
 * \param a First DataType
 * \param b Second DataType
 * \return True if a and b designate the same DataType. False otherwise
 */
bool operator==(DataType& a, DataType& b) {
    return a.getName() == b.getName();
}

/*!
 * \brief Tests whether this a subtype of the given DataType.
 * For now This simply tests whether this's name stats with the given type's name.
 * A type is its own sub/superType.
 * \param type Pointer to the DataType to test for parenthood
 * \return true if type IS the fath- ... I mean supertype
 */
bool DataType::subtypeOf(DataType* type) {
    return name.startsWith(type->getName());
}

/*!
 * \brief Returns a DataType object corresponding to
 * \param name a valid name for the type.
 * A valid name is one that only contains lowercase alphabetic chars (a-z) and slashes (/).
 * It must start and end with a slash, and two slashes must be separated by at least one letter.
 * It will be made valid if it is not but a warning will be printed.
 * \return a DataType* corresponding to the given name.
 * \throws QString if the name cannot be made valid (if it contains non-alphabetic characters
 * or multiple subsequent slashes.
 */
DataType *DataType::getType(QString name)
{
    static QRegExp validName("^/?([a-z]|[a-z]/)+$", Qt::CaseInsensitive);
    if (!validName.exactMatch(name)) {
        QString error = "Invalid DataType name : "+ name;
        throw error.toStdString().c_str();
    }

    QString normalized = name.toLower();

    if (!normalized.startsWith('/'))
        normalized = '/' + normalized;
    if (!normalized.endsWith('/'))
        normalized = normalized + '/';

    if (normalized != name) {
        qWarning() << "Warning : passed invalid datatype name : "+ name +". Normalizing to "+ normalized;
        name = name.toLower();
    }
    DataType *type = typeRegistry->value(normalized, nullptr);

    if (type == nullptr) {
        type = new DataType(normalized);
        typeRegistry->insert(normalized, type);
    }
    return type;
}


}
