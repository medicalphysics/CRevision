#pragma once

#include "models.h"

#include <QCheckBox>
#include <QLabel>
#include <QRadioButton>
#include <QString>
#include <QStringList>
#include <QTextEdit>
#include <QVector>
#include <QWidget>

class QuestionWidget : public QWidget {
    Q_OBJECT
public:
    QuestionWidget(const QString& questionGroup,
        const QStringList& roles,
        const QString& question,
        const QStringList& answers,
        const QStringList& criteria,
        bool comment = true,
        QWidget* parent = nullptr);

    Answer answer() const;
    const QString& questionGroup() const { return m_questionGroup; }
    bool hasAnswer() const;
    void clearQuestion();
    void setRole(const QString& role);
    void setCase(const QString& ccase);
    void setUserName(const QString& uname);
    void setQuestionNumber(int n);
    bool validRole() const;
signals:
    void answerChanged(void);
    void cleared(void);

protected:
    void buttonClicked();
    void updateComment();
    void criteriaChanged(int);

private:
    bool m_hasComment = true;
    QTextEdit* m_commentEdit = nullptr;
    QLabel* m_questionLabel = nullptr;
    QString m_questionGroup = "";
    QString m_question = "";
    QString m_comment = "";
    QString m_answer = "";
    QString m_role;
    QStringList m_roles;
    QString m_case;
    QString m_userName;
    QStringList m_answers;
    QStringList m_criteria;
    QVector<int> m_criteriaSelected;
    int m_questionNumber = 1;
    bool m_hasAnswer = false;
    QVector<QRadioButton*> m_buttons;
    QVector<QCheckBox*> m_checkBoxes;
};
