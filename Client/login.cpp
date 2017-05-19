#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    Ok = false;

    QIcon icon("../Client/data/32x32/Client.png");
    this->setWindowIcon(icon);
}

Login::~Login()
{
    delete ui;
}

void Login::on_LogInButton_clicked()
{
    check();
}

void Login::closeEvent(QCloseEvent *event)
{
    if(!Ok){
        emit FinishNoName();
    }
    QMainWindow::closeEvent(event);
}

void Login::on_NameEdit_returnPressed()
{
    check();
}

void Login::on_RegisterButton_clicked()
{
    check();
}

void Login::check()
{
    if(ui->NameEdit->text() != "" && ui->PassEdit->text() != ""){
        emit Name(ui->NameEdit->text());
        Ok = true;
        this->close();
    }else{
        QMessageBox::warning(this, "Error", "You must introduce a name and password", QMessageBox::Ok);
    }
}

void Login::on_PassEdit_returnPressed()
{
    check();
}
