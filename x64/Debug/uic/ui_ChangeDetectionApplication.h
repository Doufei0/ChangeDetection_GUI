/********************************************************************************
** Form generated from reading UI file 'ChangeDetectionApplication.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHANGEDETECTIONAPPLICATION_H
#define UI_CHANGEDETECTIONAPPLICATION_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ChangeDetectionApplicationClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ChangeDetectionApplicationClass)
    {
        if (ChangeDetectionApplicationClass->objectName().isEmpty())
            ChangeDetectionApplicationClass->setObjectName("ChangeDetectionApplicationClass");
        ChangeDetectionApplicationClass->resize(600, 400);
        menuBar = new QMenuBar(ChangeDetectionApplicationClass);
        menuBar->setObjectName("menuBar");
        ChangeDetectionApplicationClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(ChangeDetectionApplicationClass);
        mainToolBar->setObjectName("mainToolBar");
        ChangeDetectionApplicationClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(ChangeDetectionApplicationClass);
        centralWidget->setObjectName("centralWidget");
        ChangeDetectionApplicationClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(ChangeDetectionApplicationClass);
        statusBar->setObjectName("statusBar");
        ChangeDetectionApplicationClass->setStatusBar(statusBar);

        retranslateUi(ChangeDetectionApplicationClass);

        QMetaObject::connectSlotsByName(ChangeDetectionApplicationClass);
    } // setupUi

    void retranslateUi(QMainWindow *ChangeDetectionApplicationClass)
    {
        ChangeDetectionApplicationClass->setWindowTitle(QCoreApplication::translate("ChangeDetectionApplicationClass", "ChangeDetectionApplication", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ChangeDetectionApplicationClass: public Ui_ChangeDetectionApplicationClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHANGEDETECTIONAPPLICATION_H
