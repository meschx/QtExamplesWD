#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QThreadPool>
#include <QElapsedTimer>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void on_drawButton_clicked();
    void on_clearButton_clicked();

private:
    Ui::MainWindow *ui;
    QImage image;
    int numThreads;
    QLabel *imageLabel;
    void drawMandelbrot();
};

class MandelbrotTask : public QRunnable
{
public:
    MandelbrotTask(QImage *image, int startRow, int endRow);
    void run() override;

private:
    QImage *image;
    int startRow;
    int endRow;
};

#endif // MAINWINDOW_H
