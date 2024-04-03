#pragma once

#include "classes.h"
#include <QMainWindow>
#include <memory>

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
    void on_pushButton_clicked();

    void on_PB_AddNAme_clicked();

    void on_PB_Delete_clicked();

private:
    Ui::MainWindow *ui;
    std::vector<std::string> names{};
    std::shared_ptr<game> mGame = std::make_shared<game>(names);
    //TODO need to be abstract and polymorph.
    //std::shared_ptr<abstrGame> mGame = dynamic_pointer_cast<abstrGame>(make_shared<game>(game(names)));
    //game mGame = game(names);
};

