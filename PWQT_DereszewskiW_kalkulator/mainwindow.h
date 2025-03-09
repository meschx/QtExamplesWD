#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QRegularExpression>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onDigitClicked();
    void onUnaryOperationClicked();
    void onBinaryOperationClicked();
    void onEqualsClicked();
    void onClearClicked();
    void onClearEntryClicked();
    void onPointClicked();
    void onChangeSignClicked();
    void onUnitChanged(bool checked);
    void onAdvancedFunctionsToggled(bool checked);

private:
    Ui::MainWindow *ui;
    double currentValue;
    QString pendingOperation;
    bool waitingForOperand;
    bool useRadians;

    void abortOperation();
    bool calculate(double rightOperand, const QString &operation);
};

#endif // MAINWINDOW_H
