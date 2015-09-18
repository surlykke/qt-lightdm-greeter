#include <QDebug>
#include "decoratedusersmodel.h"
#include "loginform.h"

DecoratedUsersModel::DecoratedUsersModel(LoginForm &loginForm, QObject *parent)
    : QAbstractListModel(parent),
      usersModel(),
      loginForm(loginForm)
{

}

int DecoratedUsersModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }
    else {

        return usersModel.rowCount(parent) + 1;
    }
}

QVariant DecoratedUsersModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid() && index.row() == rowCount(QModelIndex()) - 1 && index.column() == 0) {
        if (role == Qt::DisplayRole) {
            return tr("other...");
        }
        else if (role = QLightDM::UsersModel::NameRole) {
            return loginForm.otherUserName();
        }
        else {
            return QVariant();
        }
    }
    else {
        return usersModel.data(index, role);
    }
}

