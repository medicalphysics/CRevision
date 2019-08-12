

#include "models.h"

#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QSqlRecord>


QuestionModel::QuestionModel(QObject* parent, QSqlDatabase db)
	:QSqlTableModel(parent, db)
{
	setEditStrategy(QSqlTableModel::OnManualSubmit);
	setTable("questions");
	select();
	readQuestions("resources/data/sampleQuestions.txt");
}

Question QuestionModel::question(int index)
{
	Question q;
	if (index < 0 || index >= rowCount())
		return q;
	auto r = record(index);
	q.questionGroup = r.field("question_group").value().toString();
	q.question = r.field("question").value().toString();
	q.comment = r.field("comment").value().toInt() > 0;

	auto answers = r.field("answers").value().toString();
	q.answers = answers.split(',');
	q.valid = true;
	return q;
}

void QuestionModel::readQuestions(const QString& filepath)
{
	QFile file(filepath);
	if (!file.open(QFile::ReadOnly |
		QFile::Text))
	{
		qDebug() << " Could not open the file for reading";
		file.close();
		return;
	}

	//deletes questions

	removeRows(0, rowCount());
	submitAll();

	QTextStream in(&file);
	while (!in.atEnd())
	{
		QStringList line = in.readLine().split(';');
		bool ignore = false;
		if (line.length() == 0)
			ignore = true;
		if (auto first_element = line.at(0).trimmed(); first_element.at(0) == '#')
			ignore = true;
		auto  element= this->record();
		
		if (line.count() < element.count() - 1)
			ignore = true;

		if (!ignore)
		{
			element.setValue("question_group", line.at(0).trimmed());
			element.setValue("question", line.at(1).trimmed());
			element.setValue("answers", line.at(2).trimmed());
			int comment = line.at(3).trimmed().toLower().at(0) != 'n';
			element.setValue("comment", comment);
			auto success = insertRecord(-1, element);
			if (!success)
				qDebug() << "Failed to insert question row";
			submitAll();
		}
	}
	file.close();	
}


AnswerModel::AnswerModel(QObject* parent, QSqlDatabase db)
	:QSqlTableModel(parent, db)
{
	setEditStrategy(QSqlTableModel::OnManualSubmit);
	setTable("answers");
	select();
}

CaseModel::CaseModel(QObject* parent, QSqlDatabase db)
	: QSqlTableModel(parent, db)
{
	setEditStrategy(QSqlTableModel::OnManualSubmit);
	setTable("cases");
	select();
	readCases("resources/data/sampleCases.txt");
}

void CaseModel::readCases(const QString& filepath)
{
	QFile file(filepath);
	if (!file.open(QFile::ReadOnly |
		QFile::Text))
	{
		qDebug() << " Could not open the file for reading";
		file.close();
		return;
	}

	QTextStream in(&file);
	while (!in.atEnd())
	{
		QStringList line = in.readLine().split(';');
		bool ignore = false;
		if (line.length() == 0)
			ignore = true;
		if (auto first_element = line.at(0).trimmed(); first_element.at(0) == '#')
			ignore = true;
		auto  element = this->record();

		if (line.count() < element.count() - 1)
			ignore = true;
		if (!ignore)
		{
			element.setValue("accession_name", line.at(0).trimmed());
			auto success = insertRecord(-1, element);
			if (!success)
				qDebug() << "Failed to insert case row";
			submitAll();
		}
	}
	file.close();
}