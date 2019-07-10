
#pragma once

#include <QtSql>
#include <QSqlTableModel>



class DataModel :public QSqlTableModel
{
	DataModel(QObject* parent=nullptr, QSqlDatabase db = QSqlDatabase());
};