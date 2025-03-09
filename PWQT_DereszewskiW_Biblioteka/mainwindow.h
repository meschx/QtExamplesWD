#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include "addbookdialog.h"
#include "addpersondialog.h"
#include "addrentdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_addPersonButton_clicked();
    void on_addBookButton_clicked();
    void on_addRentButton_clicked();
    void on_deleteBookButton_clicked();
    void on_deletePersonButton_clicked();
    void on_deleteRentButton_clicked();

private:
    Ui::MainWindow *ui;
    QSqlQueryModel *modelBooks;
    QSqlQueryModel *modelPersons;
    QSqlQueryModel *modelRentals;
};

#endif // MAINWINDOW_H
