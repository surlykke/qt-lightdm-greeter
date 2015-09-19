/*
* Copyright (c) 2012-2015 Christian Surlykke
*
* This file is part of qt-lightdm-greeter 
* It is distributed under the LGPL 2.1 or later license.
* Please refer to the LICENSE file for a copy of the license.
*/
#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QWidget>
#include <QProcess>
#include <QDialog>
#include <QKeyEvent>
#include <QGraphicsOpacityEffect>
#include <QMenu>

#include <QLightDM/Power>
#include <QLightDM/Greeter>
#include <QLightDM/SessionsModel>

#include "decoratedusersmodel.h"

//#include "logindata.h"

namespace Ui
{
class LoginForm;
}

class LoginForm : public QWidget
{
    Q_OBJECT

friend class DecoratedUsersModel;

public:
    explicit LoginForm(QWidget *parent = 0);
    ~LoginForm();
    virtual void setFocus(Qt::FocusReason reason);

public slots:
    void userChanged();
    void loginClicked();
    void onPrompt(QString prompt, QLightDM::Greeter::PromptType promptType);
    void authenticationComplete();

protected:
    virtual void paintEvent(QPaintEvent* event);

private:
    void initialize();
    void addLeaveEntry(QString iconName, QString text, const char *slot);

    QString currentUser();
    QString currentSession();

    Ui::LoginForm *ui;

    QLightDM::Greeter m_Greeter;
    QLightDM::PowerInterface power;
    DecoratedUsersModel usersModel;
    QLightDM::SessionsModel sessionsModel;
};

#endif // LOGINFORM_H
