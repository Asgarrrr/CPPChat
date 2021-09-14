/********************************************************************************
** Form generated from reading UI file 'Messagerie.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MESSAGERIE_H
#define UI_MESSAGERIE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MessagerieClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MessagerieClass)
    {
        if (MessagerieClass->objectName().isEmpty())
            MessagerieClass->setObjectName(QString::fromUtf8("MessagerieClass"));
        MessagerieClass->resize(600, 400);
        menuBar = new QMenuBar(MessagerieClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        MessagerieClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MessagerieClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MessagerieClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(MessagerieClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        MessagerieClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MessagerieClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MessagerieClass->setStatusBar(statusBar);

        retranslateUi(MessagerieClass);

        QMetaObject::connectSlotsByName(MessagerieClass);
    } // setupUi

    void retranslateUi(QMainWindow *MessagerieClass)
    {
        MessagerieClass->setWindowTitle(QCoreApplication::translate("MessagerieClass", "Messagerie", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MessagerieClass: public Ui_MessagerieClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MESSAGERIE_H
