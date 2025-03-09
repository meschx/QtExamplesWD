#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QThreadPool>
#include <QElapsedTimer>
#include <QDebug>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , numThreads(1)
{
    ui->setupUi(this);
    imageLabel = new QLabel(this);
    imageLabel->setGeometry(0, 50, 1000, 600);
    connect(ui->drawButton, &QPushButton::clicked, this, &MainWindow::on_drawButton_clicked);
    connect(ui->clearButton, &QPushButton::clicked, this, &MainWindow::on_clearButton_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.drawImage(0, 0, image);
}

void MainWindow::on_drawButton_clicked()
{
    numThreads = ui->threadsSpinBox->value();
    qDebug() << "Liczba wątków: " << numThreads;
    drawMandelbrot();
}

void MainWindow::on_clearButton_clicked()
{
    qDebug() << "Czyszczenie";
    image.fill(Qt::gray);
    imageLabel->setPixmap(QPixmap::fromImage(image));
    update();
}

void MainWindow::drawMandelbrot()
{
    int width = imageLabel->width();
    int height = imageLabel->height();
    image = QImage(width, height, QImage::Format_RGB32);
    image.fill(Qt::white);
    
    QElapsedTimer timer;
    timer.start();

    QThreadPool pool;
    pool.setMaxThreadCount(numThreads);

    int rowsPerTask = height / numThreads;
    for (int i = 0; i < numThreads; i++) {
        int startRow = i * rowsPerTask;
        int endRow = (i == numThreads - 1) ? height : startRow + rowsPerTask;
        pool.start(new MandelbrotTask(&image, startRow, endRow));
    }
    pool.waitForDone();

    qint64 elapsed = timer.elapsed();
    ui->timeLabel->setText(QString("Czas rysowania: %1 ms").arg(elapsed));

    imageLabel->setPixmap(QPixmap::fromImage(image));
    update();
}

MandelbrotTask::MandelbrotTask(QImage *image, int startRow, int endRow)
    : image(image), startRow(startRow), endRow(endRow)
{
}

void MandelbrotTask::run()
{
    int width = image->width();
    int height = image->height();
    double minX = -2.0;
    double maxX = 2.0;
    double minY = -2.0;
    double maxY = 2.0;

    for (int y = startRow; y < endRow; y++) {
        for (int x = 0; x < width; x++) {
            double zx = minX + (x / (double)width) * (maxX - minX);
            double zy = minY + (y / (double)height) * (maxY - minY);
            double cx = zx;
            double cy = zy;
            int iteration = 0;
            const int maxIteration = 50000  ;
            while (zx * zx + zy * zy < 4.0 && iteration < maxIteration) {
                double tmp = zx * zx - zy * zy + cx;
                zy = 2.0 * zx * zy + cy;
                zx = tmp;
                ++iteration;
            }
            int color;
            if (iteration == maxIteration) {
                color = qRgb(0, 0, 0);
            } else {
                double t = sqrt((double)iteration / maxIteration);
                int r = (int)(255 * pow(t, 0.3));
                int g = (int)(255 * pow(t, 0.6));
                int b = (int)(255 * pow(1 - t, 0.8));
                color = qRgb(r, g, b);
            }

            image->setPixel(x, y, color);
        }
    }
}
