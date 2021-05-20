#include "questioncollectionwidget.h"
#include "questionwidget.h"

#include <QFont>
#include <QGridLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QString>
#include <QVBoxLayout>

QuestionCollectionWidget::QuestionCollectionWidget(QuestionModel* questionModel, AnswerModel* answerModel, QWidget* parent)
    : QWidget(parent)
    , m_questionModel(questionModel)
    , m_answerModel(answerModel)
{

    auto mainLayout = new QVBoxLayout(this);

    for (int i = 0; i < m_questionModel->questionCount(); ++i) {
        auto q = m_questionModel->question(i);
        if (q.valid) {
            auto w = new QuestionWidget(q.questionGroup, q.roles, q.question, q.answers, q.criteria, q.comment, this);
            m_questionWidgets.append(w);
            connect(w, &QuestionWidget::answerChanged, this, &QuestionCollectionWidget::answerChanged);
        }
    }

    const int maxColumns = 1;

    for (auto w : m_questionWidgets) {
        auto group = w->questionGroup();

        if (!m_questionGroupBoxes.contains(group)) {
            auto box = new QGroupBox(group);
            m_questionGroupBoxes[group] = box;

            QFont font = box->font();
            font.setPointSize(font.pointSize() + 4);
            font.setBold(true);
            box->setFont(font);

            auto glayout = new QGridLayout(this);
            box->setLayout(glayout);
            mainLayout->addWidget(box);
            glayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
        }
        auto box = m_questionGroupBoxes[group];
        auto glayout = static_cast<QGridLayout*>(box->layout());
        const int n_elements = glayout->count();

        const int row = n_elements / maxColumns;
        const int column = n_elements % maxColumns;
        glayout->addWidget(w, row, column);
    }

    //buttons
    auto saveNextButton = new QPushButton(tr("Lagre og neste"), this);
    connect(saveNextButton, &QPushButton::clicked, this, &QuestionCollectionWidget::saveAnswers);
    connect(this, &QuestionCollectionWidget::answerReady, saveNextButton, &QPushButton::setEnabled);
    auto discardNextButton = new QPushButton(tr("Forkast og neste"), this);
    connect(discardNextButton, &QPushButton::clicked, this, &QuestionCollectionWidget::nextCase);
    auto buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(saveNextButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(discardNextButton);
    mainLayout->addLayout(buttonLayout);

    mainLayout->addStretch();

    auto vStrechLayout = new QHBoxLayout(this);
    vStrechLayout->setContentsMargins(0, 0, 0, 0);
    vStrechLayout->addLayout(mainLayout);
    auto spacerItem = new QSpacerItem(1, 1, QSizePolicy::Maximum, QSizePolicy::Maximum);
    vStrechLayout->addSpacerItem(spacerItem);
    setLayout(vStrechLayout);
}

void QuestionCollectionWidget::clearAllQuestions()
{
    for (auto w : m_questionWidgets) {
        w->clearQuestion();
    }
    emit answerReady(false);
}

void QuestionCollectionWidget::setRole(const QString& role)
{
    m_currentRole = role;

    auto ibeg = m_questionGroupBoxes.begin();
    while (ibeg != m_questionGroupBoxes.end()) {
        ibeg.value()->setVisible(false);
        ibeg++;
    }

    int question_number = 1;
    for (auto w : m_questionWidgets) {
        w->setRole(m_currentRole);        
        if (w->validRole()) {
            w->setQuestionNumber(question_number++);
            const auto& groupname = w->questionGroup();
            if (m_questionGroupBoxes.contains(groupname)) {
                m_questionGroupBoxes[groupname]->setVisible(true);                
            }            
        }
    }

    this->setVisible(!m_currentRole.isEmpty());
}

void QuestionCollectionWidget::setUserName(const QString& uname)
{
    m_currentUserName = uname;
    for (auto w : m_questionWidgets) {
        w->setUserName(m_currentUserName);
    }
}
void QuestionCollectionWidget::setCase(const QString& ccase)
{
    m_currentCase = ccase;
    for (auto w : m_questionWidgets) {
        w->setCase(m_currentCase);
    }
}

void QuestionCollectionWidget::answerChanged(void)
{
    bool allAnswered = true;
    for (auto w : m_questionWidgets) {
        allAnswered = (w->hasAnswer() || !(w->validRole())) && allAnswered;
    }
    emit answerReady(allAnswered);
}

void QuestionCollectionWidget::saveAnswers()
{
    for (auto w : m_questionWidgets) {
        const auto ans = w->answer();
        m_answerModel->saveAnswer(ans);
    }
    emit nextCase();
}
