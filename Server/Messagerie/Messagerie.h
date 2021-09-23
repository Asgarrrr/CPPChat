#pragma once

#include <QtWidgets/QMainWindow>

class Messagerie : public QMainWindow
{
    Q_OBJECT

public:
    Messagerie(QWidget *parent = Q_NULLPTR);

private:
    Ui::MessagerieClass ui;
};
