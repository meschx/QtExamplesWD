#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlError>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    modelBooks(new QSqlQueryModel(this)),
    modelPersons(new QSqlQueryModel(this)),
    modelRentals(new QSqlQueryModel(this))
{
    ui->setupUi(this);

    // Jezeli baza istnieje, to ja otwiera
    // Jezeli nie istnieje, to baza jest tworzona na podstawie podanej pozniej instrukcji
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("bibl.db");

    if (!db.open()) {
        qDebug() << "Problem z otworzeniem bazy" << db.lastError().text();
    } else {
        qDebug() << "Baza otworzona";
    }

    QSqlQuery query;

    QString createTableBooks = "CREATE TABLE IF NOT EXISTS books ("
                               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                               "title TEXT, "
                               "author TEXT)";

    QString createTablePersons = "CREATE TABLE IF NOT EXISTS persons ("
                                 "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                 "name TEXT, "
                                 "surname TEXT)";

    QString createTableRentals = "CREATE TABLE IF NOT EXISTS rentals ("
                                 "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                 "book_id INTEGER, "
                                 "person_id INTEGER, "
                                 "rental_date TEXT, "
                                 "FOREIGN KEY(book_id) REFERENCES books(id), "
                                 "FOREIGN KEY(person_id) REFERENCES persons(id))";

    if (!query.exec(createTableBooks)) {
        qDebug() << "Błąd tworzenia tabeli books:" << query.lastError().text();
    }
    if (!query.exec(createTablePersons)) {
        qDebug() << "Błąd tworzenia tabeli persons:" << query.lastError().text();
    }
    if (!query.exec(createTableRentals)) {
        qDebug() << "Błąd tworzenia tabeli rentals:" << query.lastError().text();
    }

    modelBooks->setQuery("SELECT id, title, author FROM books");
    modelBooks->setHeaderData(1, Qt::Horizontal, "Tytuł");
    modelBooks->setHeaderData(2, Qt::Horizontal, "Autor");

    modelPersons->setQuery("SELECT id, name, surname FROM persons");
    modelPersons->setHeaderData(1, Qt::Horizontal, "Imię");
    modelPersons->setHeaderData(2, Qt::Horizontal, "Nazwisko");

    modelRentals->setQuery("SELECT rentals.id, books.title, persons.name || ' ' || persons.surname, rentals.rental_date "
                       "FROM rentals "
                       "JOIN books ON rentals.book_id = books.id "
                       "JOIN persons ON rentals.person_id = persons.id");
    modelRentals->setHeaderData(1, Qt::Horizontal, "Tytuł książki");
    modelRentals->setHeaderData(2, Qt::Horizontal, "Imię i nazwisko");
    modelRentals->setHeaderData(3, Qt::Horizontal, "Data wyp.");

    qDebug() << "Liczba wierszy w modelBooks:" << modelBooks->rowCount();
    qDebug() << "Liczba wierszy w modelPersons:" << modelPersons->rowCount();
    qDebug() << "Liczba wierszy w modelRentals:" << modelRentals->rowCount();

    ui->booksTableView->setModel(modelBooks);
    ui->personsTableView->setModel(modelPersons);
    ui->rentalsTableView->setModel(modelRentals);

    ui->booksTableView->reset();
    ui->personsTableView->reset();
    ui->rentalsTableView->reset();

    // connect(ui->deleteBookButton, &QPushButton::clicked, this, &MainWindow::on_deleteBookButton_clicked);
    // connect(ui->deletePersonButton, &QPushButton::clicked, this, &MainWindow::on_deletePersonButton_clicked);
    // connect(ui->deleteRentButton, &QPushButton::clicked, this, &MainWindow::on_deleteRentButton_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_addPersonButton_clicked()
{
    AddPersonDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        modelPersons->setQuery("SELECT id, name, surname FROM persons");
        qDebug() << "Liczba wierszy w modelPersons po dodaniu:" << modelPersons->rowCount();
    }
}

void MainWindow::on_addBookButton_clicked()
{
    AddBookDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        modelBooks->setQuery("SELECT id, title, author FROM books");
        qDebug() << "Liczba wierszy w modelBooks po dodaniu:" << modelBooks->rowCount();
    }
}

void MainWindow::on_addRentButton_clicked()
{
    AddRentDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        modelRentals->setQuery("SELECT rentals.id, books.title, persons.name || ' ' || persons.surname, rentals.rental_date "
                               "FROM rentals "
                               "JOIN books ON rentals.book_id = books.id "
                               "JOIN persons ON rentals.person_id = persons.id");
        modelRentals->setHeaderData(1, Qt::Horizontal, "Tytuł książki");
        modelRentals->setHeaderData(2, Qt::Horizontal, "Imię i nazwisko");
        modelRentals->setHeaderData(3, Qt::Horizontal, "Data wyp.");
        qDebug() << "Liczba wierszy w modelRentals po dodaniu:" << modelRentals->rowCount();
    }
}

void MainWindow::on_deleteBookButton_clicked()
{
    QModelIndex currentIndex = ui->booksTableView->currentIndex();
    if (!currentIndex.isValid()) {
        QMessageBox::warning(this, "Błąd", "Proszę zaznaczyć wiersz do usunięcia.");
        return;
    }

    int id = modelBooks->data(modelBooks->index(currentIndex.row(), 0)).toInt();
    QSqlQuery query;

    query.prepare("SELECT COUNT(*) FROM rentals WHERE book_id = :id");
    query.bindValue(":id", id);
    query.exec();
    if (query.next() && query.value(0).toInt() > 0) {
        QMessageBox::warning(this, "Błąd", "Nie można usunąć książki, ponieważ jest wypożyczona.");
        return;
    }

    query.prepare("DELETE FROM books WHERE id = :id");
    query.bindValue(":id", id);
    if (!query.exec()) {
        QMessageBox::warning(this, "Błąd", "Nie udało się usunąć książki.");
        return;
    }

    modelBooks->setQuery("SELECT id, title, author FROM books");
    qDebug() << "Liczba wierszy w modelBooks po usunięciu:" << modelBooks->rowCount();
}

void MainWindow::on_deletePersonButton_clicked()
{
    QModelIndex currentIndex = ui->personsTableView->currentIndex();
    if (!currentIndex.isValid()) {
        QMessageBox::warning(this, "Błąd", "Proszę zaznaczyć wiersz do usunięcia.");
        return;
    }

    int id = modelPersons->data(modelPersons->index(currentIndex.row(), 0)).toInt();
    QSqlQuery query;

    query.prepare("SELECT COUNT(*) FROM rentals WHERE person_id = :id");
    query.bindValue(":id", id);
    query.exec();
    if (query.next() && query.value(0).toInt() > 0) {
        QMessageBox::warning(this, "Błąd", "Nie można usunąć osoby, ponieważ ma wypożyczone książki.");
        return;
    }

    query.prepare("DELETE FROM persons WHERE id = :id");
    query.bindValue(":id", id);
    if (!query.exec()) {
        QMessageBox::warning(this, "Błąd", "Nie udało się usunąć osoby.");
        return;
    }

    modelPersons->setQuery("SELECT id, name, surname FROM persons");
    qDebug() << "Liczba wierszy w modelPersons po usunięciu:" << modelPersons->rowCount();
}

void MainWindow::on_deleteRentButton_clicked()
{
    QModelIndex currentIndex = ui->rentalsTableView->currentIndex();
    if (!currentIndex.isValid()) {
        QMessageBox::warning(this, "Błąd", "Proszę zaznaczyć wiersz do usunięcia.");
        return;
    }

    int id = modelRentals->data(modelRentals->index(currentIndex.row(), 0)).toInt();
    QSqlQuery query;

    query.prepare("DELETE FROM rentals WHERE id = :id");
    query.bindValue(":id", id);
    if (!query.exec()) {
        QMessageBox::warning(this, "Błąd", "Nie udało się usunąć wypożyczenia.");
        return;
    }

    modelRentals->setQuery("SELECT rentals.id, books.title, persons.name || ' ' || persons.surname, rentals.rental_date "
                           "FROM rentals "
                           "JOIN books ON rentals.book_id = books.id "
                           "JOIN persons ON rentals.person_id = persons.id");
    qDebug() << "Liczba wierszy w modelRentals po usunięciu:" << modelRentals->rowCount();
}
