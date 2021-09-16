#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Messagerie.h"

class Messagerie : public QMainWindow
{
    Q_OBJECT

public:
    Messagerie(QWidget *parent = Q_NULLPTR);

private:
    Ui::MessagerieClass ui;
};
