
#pragma once

#include <QSqlTableModel>
#include <QtSql>

#include <QDateTime>
#include <QString>
#include <QStringList>

class Question {
public:
    QString questionGroup;
    QStringList roles;
    QString question;
    QStringList answers;
    QStringList criteria;
    bool comment = true;
    bool valid = false;
};

class QuestionModel : public QSqlTableModel {
    Q_OBJECT
public:
    QuestionModel(QObject* parent = nullptr, QSqlDatabase db = QSqlDatabase());
    int questionCount() { return this->rowCount(); }
    Question question(int index);

protected:
    void readQuestions(const QString& filepath);
};

class Answer {
public:
    QString questionGroup;
    QString role;
    QStringList roles;
    QString question;
    QString answer;
    QStringList answers;
    QString comment;
    QString accessionName;
    QString username;
    QString date;
    QStringList criteria;
    QVector<int> criteriaSelected;
    bool isValid = false;
    Answer() { date = QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss"); }
    QString criteriaSelectedText(const QString sep = ",") const
    {
        QString txt;
        for (int i = 0; i < criteriaSelected.size() - 1; ++i) {
            txt += QString::number(criteriaSelected[i]) + sep;
        }
        if (criteriaSelected.size() > 0) {
            txt += QString::number(criteriaSelected[criteriaSelected.size() - 1]);
        }
        return txt;
    }
};

class AnswerModel : public QSqlTableModel {
    Q_OBJECT
public:
    AnswerModel(QObject* parent = nullptr, QSqlDatabase db = QSqlDatabase());
    void saveAnswer(const Answer& answer);
};

class CaseModel : public QSqlTableModel {
    Q_OBJECT
public:
    CaseModel(QObject* parent = nullptr, QSqlDatabase db = QSqlDatabase());

protected:
    void readCases(const QString& filepath);
};
