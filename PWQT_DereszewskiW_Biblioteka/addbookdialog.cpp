#include "addbookdialog.h"
#include "ui_addbookdialog.h"

AddBookDialog::AddBookDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddBookDialog)
{
    ui->setupUi(this);
}

AddBookDialog::~AddBookDialog()
{
    delete ui;
}

void AddBookDialog::on_addButton_clicked()
{
    qDebug() << "on_addButton_clicked called";
    
    QString title = ui->titleLineEdit->text();
    QString author = ui->authorLineEdit->text();

    QSqlQuery query;
    query.prepare("INSERT INTO books (title, author) VALUES (:title, :author)");
    query.bindValue(":title", title);
    query.bindValue(":author", author);
    query.exec();

    accept();
}
