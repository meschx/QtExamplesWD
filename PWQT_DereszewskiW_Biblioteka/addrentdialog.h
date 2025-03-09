#ifndef ADDRENTDIALOG_H
#define ADDRENTDIALOG_H

#include <QDialog>
#include <QSqlQuery>
#include <QSqlQueryModel>

namespace Ui {
class AddRentDialog;
}

class AddRentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddRentDialog(QWidget *parent = nullptr);
    ~AddRentDialog();

private slots:
    void on_addButton_clicked();

private:
    Ui::AddRentDialog *ui;
    QSqlQueryModel *personModel;
    QSqlQueryModel *bookModel;
};

#endif // ADDRENTDIALOG_H
