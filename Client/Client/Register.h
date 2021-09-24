#pragma once

#include <QWidget>
#include "ui_Register.h"

class Register : public QWidget
{
	Q_OBJECT

public:
	Register(QWidget *parent = Q_NULLPTR);
	~Register();

private:
	Ui::Register ui;
};
