#include "addpersondialog.h"
#include "ui_addpersondialog.h"

AddPersonDialog::AddPersonDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddPersonDialog)
{
    ui->setupUi(this);
}

AddPersonDialog::~AddPersonDialog()
{
    delete ui;
}

void AddPersonDialog::on_addButton_clicked()
{
    qDebug() << "on_addButton_clicked called";

    QString name = ui->nameLineEdit->text();
    QString surname = ui->surnameLineEdit->text();

    QSqlQuery query;
    query.prepare("INSERT INTO persons (name, surname) VALUES (:name, :surname)");
    query.bindValue(":name", name);
    query.bindValue(":surname", surname);
    query.exec();

    accept();
}