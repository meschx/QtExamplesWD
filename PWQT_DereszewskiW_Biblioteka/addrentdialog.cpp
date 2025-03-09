#include "addrentdialog.h"
#include "ui_addrentdialog.h"
#include <QDate>

AddRentDialog::AddRentDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddRentDialog),
    personModel(new QSqlQueryModel(this)),
    bookModel(new QSqlQueryModel(this))
{
    ui->setupUi(this);

    ui->dateEdit->setDate(QDate::currentDate());

    personModel->setQuery("SELECT id, name || ' ' || surname FROM persons");
    ui->personComboBox->setModel(personModel);
    ui->personComboBox->setModelColumn(1);

    bookModel->setQuery("SELECT id, title FROM books");
    ui->bookComboBox->setModel(bookModel);
    ui->bookComboBox->setModelColumn(1);
}

AddRentDialog::~AddRentDialog()
{
    delete ui;
}

void AddRentDialog::on_addButton_clicked()
{
    qDebug() << "on_addButton_clicked called";

    int personId = ui->personComboBox->model()->data(ui->personComboBox->model()->index(ui->personComboBox->currentIndex(), 0)).toInt();
    int bookId = ui->bookComboBox->model()->data(ui->bookComboBox->model()->index(ui->bookComboBox->currentIndex(), 0)).toInt();
    QString rentalDate = ui->dateEdit->date().toString("yyyy-MM-dd");

    QSqlQuery query;
    query.prepare("INSERT INTO rentals (person_id, book_id, rental_date) VALUES (:person_id, :book_id, :rental_date)");
    query.bindValue(":person_id", personId);
    query.bindValue(":book_id", bookId);
    query.bindValue(":rental_date", rentalDate);
    query.exec();

    accept();
}
