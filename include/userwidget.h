

#pragma once

#include <QLineEdit>
#include <QString>
#include <QWidget>

class UserWidget : public QWidget {
    Q_OBJECT
public:
    UserWidget(QWidget* parent = nullptr);

signals:
    void usernameChanged(const QString& username);

protected:
    void findUsername();

private:
    QLineEdit* m_nameEdit = nullptr;
};
