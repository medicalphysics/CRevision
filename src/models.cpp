

#include "models.h"

DataModel::DataModel(QObject* parent, QSqlDatabase db)
	:QSqlTableModel(parent, db)
{

}