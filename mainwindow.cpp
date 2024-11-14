#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "dc_driver.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initDriver();
}

MainWindow::~MainWindow()
{
    exitDriver();
    delete ui;
}

void MainWindow::on_RunButton_clicked()
{
    QString targetSpeed = ui->SpeedInput->toPlainText();
    QString targetAngle = ui->AngleInput->toPlainText();

    unsigned int speed  = targetSpeed.toInt();
    unsigned int angle  = targetAngle.toInt();
    qInfo() << "Speed: " << speed;
    qInfo() << "Angle: " << angle;
    setTargetSpeed(speed);
    setTargetAngle(angle);
}

