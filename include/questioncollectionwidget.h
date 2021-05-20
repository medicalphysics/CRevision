#pragma once

#include <QGroupBox>
#include <QMap>
#include <QVector>
#include <QWidget>

#include "models.h"
#include "questionwidget.h"

class QuestionCollectionWidget : public QWidget {
    Q_OBJECT
public:
    QuestionCollectionWidget(QuestionModel* questionModel, AnswerModel* answerModel, QWidget* parent = nullptr);

    void clearAllQuestions();
    void setRole(const QString& role);
    void setUserName(const QString& uname);
    void setCase(const QString& ccase);
signals:
    void answerReady(bool ready);
    void nextCase();

protected:
    void saveAnswers();
    void answerChanged(void);

private:
    QuestionModel* m_questionModel = nullptr;
    AnswerModel* m_answerModel = nullptr;
    QVector<QuestionWidget*> m_questionWidgets;
    QMap<QString, QGroupBox*> m_questionGroupBoxes;
    QString m_currentRole;
    QString m_currentUserName;
    QString m_currentCase;
};
