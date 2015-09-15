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

#include "logindata.h"

namespace Ui {
class LoginForm;
}

class LoginForm : public QWidget
{
    Q_OBJECT

public:
    explicit LoginForm(QWidget *parent = 0);
    ~LoginForm();

    virtual void setFocus(Qt::FocusReason reason);

public slots:
    void userComboCurrentIndexChanged();
    void otherUserEditingFinished(); 
    void loginClicked();

    void onPrompt(QString prompt, QLightDM::Greeter::PromptType promptType);

    void authenticationComplete();

private:
    void setupAppearence();
    void fillUserAndSessionCombos();
    void setupConnections();
    void initializeControls();
    void setSessionCombo(int session_index);
    void setUser(QString user);
    QMenu* buildLeaveMenu();


    Ui::LoginForm *ui;
    QLightDM::Greeter m_Greeter; 
    LoginData m_LoginData;
    QLightDM::PowerInterface power;

    int m_otherUserComboIndex;
    QString m_user;
};

#endif // LOGINFORM_H
