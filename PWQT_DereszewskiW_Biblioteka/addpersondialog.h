#ifndef ADDPERSONDIALOG_H
#define ADDPERSONDIALOG_H

#include <QDialog>
#include <QSqlQuery>

namespace Ui {
class AddPersonDialog;
}

class AddPersonDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddPersonDialog(QWidget *parent = nullptr);
    ~AddPersonDialog();

private slots:
    void on_addButton_clicked();

private:
    Ui::AddPersonDialog *ui;
};

#endif // ADDPERSONDIALOG_H
