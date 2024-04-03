#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "classes.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    cout<<"start prog"<<"\n";
    int players=0;
    vector <string> names{"bigBob", "aBobs","Steve"};
    cout<<"";
    cin>>players;
    game aGame(names);
    aGame.play();
}

