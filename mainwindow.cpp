#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QString>
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
    if(names.size()<2){
        ui->status->setText("Wrong Names");
        return;
    }
    qDebug()<<names;
    mGame->reSetPlayers(names);
    std::string content = mGame->play();
    ui->status->setText(QString::fromUtf8(content.data(), content.size()));
}


void MainWindow::on_PB_AddNAme_clicked()
{
    QString name = ui->nameLE->text();
    names.push_back(name.toStdString());
    QString res = "";
    for(const auto& elem : names){
        res += elem + "\n";
    }
    ui->TB_Names->setText(res);
}


void MainWindow::on_PB_Delete_clicked()
{
    ui->TB_Names->clear();
    names.clear();
}

