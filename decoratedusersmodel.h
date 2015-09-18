#ifndef DECORATEDUSERSMODEL_H
#define DECORATEDUSERSMODEL_H

#include <QAbstractListModel>
#include <QLightDM/UsersModel>

class LoginForm;
class DecoratedUsersModel : public QAbstractListModel
{
   Q_OBJECT

public:
    DecoratedUsersModel(LoginForm& loginForm, QObject* parent = 0);

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

private:
    QLightDM::UsersModel usersModel;
    LoginForm& loginForm;
};

#endif // DECORATEDUSERSMODEL_H
