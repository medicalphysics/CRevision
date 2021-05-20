
#include "questionwidget.h"

#include <QFont>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>

QChar letterFromInt(int n)
{
    auto A = 'A';
    return QChar(A + n);
}

QuestionWidget::QuestionWidget(const QString& questionGroup, const QStringList& roles, const QString& question, const QStringList& answers, const QStringList& criteria, bool comment, QWidget* parent)
    : QWidget(parent)
    , m_hasComment(comment)
    , m_questionGroup(questionGroup)
    , m_question(question)
    , m_roles(roles)
    , m_answers(answers)
    , m_criteria(criteria)
{
    auto mainLayout = new QVBoxLayout(this);
    //question

    auto quest_crit_layout = new QHBoxLayout;
    mainLayout->addLayout(quest_crit_layout);

    auto quest_layout = new QVBoxLayout;
    quest_crit_layout->addLayout(quest_layout);
    auto crit_layout = new QVBoxLayout;
    quest_crit_layout->addLayout(crit_layout);
    quest_crit_layout->addStretch();

    m_questionLabel = new QLabel(m_question, this);
    auto sizePolicy = QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    m_questionLabel->setSizePolicy(sizePolicy);
    quest_layout->addWidget(m_questionLabel);

    //answers
    auto quest_groupBox = new QGroupBox();

    QFont font = quest_groupBox->font();
    font.setPointSize(font.pointSize());
    font.setBold(true);
    quest_groupBox->setFont(font);
    m_questionLabel->setFont(font);

    quest_groupBox->setSizePolicy(sizePolicy);
    auto groupBoxLayout = new QVBoxLayout(quest_groupBox);
    groupBoxLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    for (const QString& a : m_answers) {
        QRadioButton* r = new QRadioButton(a, quest_groupBox);
        groupBoxLayout->addWidget(r);
        r->setSizePolicy(sizePolicy);
        connect(r, &QRadioButton::toggled, this, &QuestionWidget::buttonClicked);
        m_buttons.append(r);
    }
    quest_groupBox->setLayout(groupBoxLayout);
    quest_groupBox->setFlat(true); // no borders
    quest_layout->addWidget(quest_groupBox);

    if (m_criteria.size() > 0) {
        auto critLabel = new QLabel(tr("Kriterier"), this);
        critLabel->setFont(font);
        critLabel->setSizePolicy(sizePolicy);
        crit_layout->addWidget(critLabel);

        auto crit_groupBox = new QGroupBox();
        crit_groupBox->setFont(font);
        auto critBoxLayout = new QVBoxLayout;
        crit_groupBox->setLayout(critBoxLayout);
        critBoxLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);

        int crit_number = 0;
        for (const auto& crit : m_criteria) {
            QString crit_prefix = letterFromInt(crit_number++) + QString(": ");
            auto cw = new QCheckBox(crit_prefix + crit, this);
            m_checkBoxes.append(cw);
            critBoxLayout->addWidget(cw);
            cw->setFont(font);
            connect(cw, &QCheckBox::stateChanged, this, &QuestionWidget::criteriaChanged);
        }
        crit_layout->addWidget(crit_groupBox);
        crit_groupBox->setFlat(true); // no borders
    }
    crit_layout->addStretch();
    quest_layout->addStretch();

    if (m_hasComment) {
        m_commentEdit = new QTextEdit(this);
        m_commentEdit->setPlaceholderText(tr("Kommentar"));
        m_commentEdit->setFont(font);
        connect(m_commentEdit, &QTextEdit::textChanged, this, &QuestionWidget::updateComment);
        mainLayout->addWidget(m_commentEdit);
    }
    this->setLayout(mainLayout);
}

Answer QuestionWidget::answer() const
{
    Answer ans;
    ans.accessionName = m_case;
    ans.answer = m_answer;
    ans.answers = m_answers;
    ans.comment = m_comment;
    ans.isValid = m_hasAnswer && validRole();
    ans.question = m_question;
    ans.questionGroup = m_questionGroup;
    ans.role = m_role;
    ans.roles = m_roles;
    ans.username = m_userName;
    ans.criteria = m_criteria;

    ans.criteriaSelected = m_criteriaSelected;
    return ans;
}

void QuestionWidget::clearQuestion()
{
    for (auto r : m_buttons) {
        r->setAutoExclusive(false);
        r->setChecked(false);
        r->setAutoExclusive(true);
    }
    m_hasAnswer = false;
    m_comment = "";
    if (m_commentEdit) {
        m_commentEdit->clear();
    }
    for (auto r : m_checkBoxes) {
        r->setCheckState(Qt::CheckState::Unchecked);
    }
    m_criteriaSelected.clear();

    emit cleared();
}

void QuestionWidget::setRole(const QString& role)
{
    m_role = role;
    setHidden(!validRole());
    clearQuestion();
}

void QuestionWidget::setCase(const QString& ccase)
{
    m_case = ccase;
    clearQuestion();
}
void QuestionWidget::setUserName(const QString& uname)
{
    m_userName = uname;
    clearQuestion();
}

void QuestionWidget::setQuestionNumber(int n)
{
    m_questionNumber = n;
    const QString number_prefix = QString::number(m_questionNumber) + QString(": ");
    m_questionLabel->setText(number_prefix + m_question);
}

bool QuestionWidget::hasAnswer() const
{
    return m_hasAnswer;
}

void QuestionWidget::buttonClicked()
{
    for (auto r : m_buttons) {
        if (r->isChecked()) {
            m_answer = r->text();
            m_hasAnswer = true;
            emit answerChanged();
        }
    }
}
void QuestionWidget::criteriaChanged(int)
{
    m_criteriaSelected.clear();
    for (int i = 0; i < m_checkBoxes.size(); ++i) {
        bool checked = m_checkBoxes[i]->checkState() == Qt::CheckState::Checked;
        if (checked) {
            m_criteriaSelected.append(i);
        }
    }
    emit answerChanged();
}

void QuestionWidget::updateComment()
{
    m_comment = m_commentEdit->toPlainText();
    emit answerChanged();
}

bool QuestionWidget::validRole() const
{
    return m_roles.contains(m_role) || m_roles.isEmpty();
}
