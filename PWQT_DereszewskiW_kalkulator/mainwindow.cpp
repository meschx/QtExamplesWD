#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QPlainTextEdit>
#include <QCheckBox>
#include <QRegularExpression>
#include <cmath>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , currentValue(0)
    , waitingForOperand(false)
    , useRadians(false)
{
    ui->setupUi(this);

    QList<QPushButton*> digitButtons = findChildren<QPushButton*>(QRegularExpression("pushButton_[0-9]"));
    for (QPushButton* button : digitButtons)
        connect(button, &QPushButton::clicked, this, &MainWindow::onDigitClicked);

    connect(ui->buttonSqrt, &QPushButton::clicked, this, &MainWindow::onUnaryOperationClicked);
    connect(ui->buttonReciprocal, &QPushButton::clicked, this, &MainWindow::onUnaryOperationClicked);
    connect(ui->buttonSquare, &QPushButton::clicked, this, &MainWindow::onUnaryOperationClicked);
    connect(ui->buttonLn, &QPushButton::clicked, this, &MainWindow::onUnaryOperationClicked);
    connect(ui->buttonSin, &QPushButton::clicked, this, &MainWindow::onUnaryOperationClicked);
    connect(ui->buttonCos, &QPushButton::clicked, this, &MainWindow::onUnaryOperationClicked);
    connect(ui->buttonAdd, &QPushButton::clicked, this, &MainWindow::onBinaryOperationClicked);
    connect(ui->buttonSubtract, &QPushButton::clicked, this, &MainWindow::onBinaryOperationClicked);
    connect(ui->buttonMultiply, &QPushButton::clicked, this, &MainWindow::onBinaryOperationClicked);
    connect(ui->buttonDivide, &QPushButton::clicked, this, &MainWindow::onBinaryOperationClicked);
    connect(ui->buttonEquals, &QPushButton::clicked, this, &MainWindow::onEqualsClicked);
    connect(ui->buttonClear, &QPushButton::clicked, this, &MainWindow::onClearClicked);
    connect(ui->buttonClearEntry, &QPushButton::clicked, this, &MainWindow::onClearEntryClicked);
    connect(ui->buttonPoint, &QPushButton::clicked, this, &MainWindow::onPointClicked);
    connect(ui->buttonChangeSign, &QPushButton::clicked, this, &MainWindow::onChangeSignClicked);
    connect(ui->checkBoxRadians, &QCheckBox::toggled, this, &MainWindow::onUnitChanged);
    connect(ui->checkBoxAdvancedFunctions, &QCheckBox::toggled, this, &MainWindow::onAdvancedFunctionsToggled);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onDigitClicked()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (!button)
        return;

    int digitValue = button->text().toInt();
    if (ui->display->toPlainText() == "0" && digitValue == 0.0)
        return;

    if (waitingForOperand) {
        ui->display->setPlainText(QString::number(digitValue));
        waitingForOperand = false;
    } else
        ui->display->setPlainText(ui->display->toPlainText() + QString::number(digitValue));
    qDebug() << "Digit clicked: " << digitValue;
}

void MainWindow::onUnaryOperationClicked()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (!button)
        return;

    QString operation = button->text();
    double operand = ui->display->toPlainText().toDouble();
    double result = 0.0;

    if (operation == "sqrt") {
        if (operand < 0.0) {
            abortOperation();
            return;
        }
        result = std::sqrt(operand);
    } else if (operation == "1/x") {
        if (operand == 0.0) {
            abortOperation();
            return;
        }
        result = 1.0 / operand;
    } else if (operation == "x^2")
        result = operand * operand;
      else if (operation == "ln") {
        if (operand <= 0.0) {
            abortOperation();
            return;
        }
        result = std::log(operand);
    } else if (operation == "sin") {
        if (!useRadians)
            operand = qDegreesToRadians(operand);
        result = std::sin(operand);
        if (std::fmod(operand, M_PI) == 0)
            result = 0;
    } else if (operation == "cos") {
        if (!useRadians)
            operand = qDegreesToRadians(operand);
        result = std::cos(operand);
        if (std::fmod(operand + M_PI_2, M_PI) == 0)
            result = 0;
    }

    ui->display->setPlainText(QString::number(result));
    waitingForOperand = true;
}

void MainWindow::onBinaryOperationClicked()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (!button)
        return;

    QString operation = button->text();
    double operand = ui->display->toPlainText().toDouble();

    if (!pendingOperation.isEmpty()) {
        if (!calculate(operand, pendingOperation)) {
            abortOperation();
            return;
        }
        ui->display->setPlainText(QString::number(currentValue));
    } else
        currentValue = operand;

    pendingOperation = operation;
    waitingForOperand = true;
    qDebug() << "Binary operation clicked: " << operation;
}

void MainWindow::onEqualsClicked()
{
    double operand = ui->display->toPlainText().toDouble();
    qDebug() << "Equals clicked";
    if (!pendingOperation.isEmpty()) {
        if (!calculate(operand, pendingOperation)) {
            abortOperation();
            return;
        }
        ui->display->setPlainText(QString::number(currentValue));
        pendingOperation.clear();
    }

    waitingForOperand = true;
}

void MainWindow::onClearClicked()
{
    currentValue = 0.0;
    pendingOperation.clear();
    ui->display->setPlainText("");
    waitingForOperand = false;
    qDebug() << "Clear clicked";
}

void MainWindow::onClearEntryClicked()
{
    ui->display->setPlainText("");
    waitingForOperand = true;
    qDebug() << "Clear entry clicked";
}

void MainWindow::onPointClicked()
{
    if (waitingForOperand) {
        ui->display->setPlainText("0");
        waitingForOperand = false;
    }

    if (!ui->display->toPlainText().contains("."))
        ui->display->setPlainText(ui->display->toPlainText() + ".");
    qDebug() << "Point clicked";
}

void MainWindow::onChangeSignClicked()
{
    QString text = ui->display->toPlainText();
    double value = text.toDouble();

    if (value > 0.0)
        text.prepend("-");
    else if (value < 0.0)
        text.remove(0, 1);

    ui->display->setPlainText(text);
    qDebug() << "Change sign clicked";
}

void MainWindow::onUnitChanged(bool checked)
{
    useRadians = checked;
    qDebug() << "Use radians: " << useRadians;
}

void MainWindow::onAdvancedFunctionsToggled(bool checked)
{
    ui->checkBoxRadians->setVisible(checked);
    ui->buttonSqrt->setVisible(checked);
    ui->buttonReciprocal->setVisible(checked);
    ui->buttonSquare->setVisible(checked);
    ui->buttonLn->setVisible(checked);
    ui->buttonSin->setVisible(checked);
    ui->buttonCos->setVisible(checked);
    qDebug() << "Advanced functions: " << checked;
}

void MainWindow::abortOperation()
{
    ui->display->setPlainText("Error");
    currentValue = 0.0;
    pendingOperation.clear();
    waitingForOperand = true;
    qDebug() << "Operation aborted";
}

bool MainWindow::calculate(double rightOperand, const QString &operation)
{
    if (operation == "+")
        currentValue += rightOperand;
    else if (operation == "-")
        currentValue -= rightOperand;
    else if (operation == "*")
        currentValue *= rightOperand;
    else if (operation == "/") {
        if (rightOperand == 0.0)
            return false;
        currentValue /= rightOperand;
    }
    qDebug() << "Calculate: " << currentValue;
    return true;
}
