#include "datatype.h"

namespace graphshell {

	QMap<QString, DataType *> *DataType::typeRegistry = new QMap<QString, DataType *>();
	QObject *DataType::parent = nullptr;

	DataType::DataType(QString name) : QObject()
	{
		this->name = name;
	}

	bool operator==(DataType& a, DataType& b) {
		return a.getName() == b.getName();
	}

    bool DataType::subtypeOf(DataType& type) {
		return name.startsWith(type.getName());
	}

	void DataType::initializeRegistry(QObject *parent)
	{
		if (parent != nullptr)
			return;
		DataType::parent = parent;
	}

	DataType *DataType::getType(QString name)
	{
		static QRegExp validName("^[a-z][a-z/]+[a-z]$", Qt::CaseInsensitive);
		if (!validName.exactMatch(name)) {
			throw new QString("Invalid datatype name");
		}
		if (name.toLower() != name) {
			qWarning() << "Warning : passed non-lowercase datatype name. Converting to lowercase.";
			name = name.toLower();
		}
		DataType *type = typeRegistry->value(name, nullptr);

		if (type == nullptr) {
			type = new DataType(name);
			typeRegistry->insert(name, type);
		}
		return type;
	}


}
