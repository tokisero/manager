#ifndef VALIDATEDSQLTABLEMODEL_H
#define VALIDATEDSQLTABLEMODEL_H

#include <QSqlTableModel>
#include "ManagerException.h"

class ValidatedSqlTableModel : public QSqlTableModel {
    Q_OBJECT

public:
    explicit ValidatedSqlTableModel(QObject* parent = nullptr, QSqlDatabase db = QSqlDatabase());
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
    bool submitAll();

private:
    bool validateData(const QModelIndex& index, const QVariant& value) const;
};

#endif
