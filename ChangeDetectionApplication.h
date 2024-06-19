#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ChangeDetectionApplication.h"
#include <QPushButton>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QLabel>
#include <QPixmap>
#include <QDebug>
#include <QEvent>
#include <string>
#include <QStringLiteral>
#include <QProcess>
#include <iostream>
#include <QFileInfo>
#include <QDir>

class ChangeDetectionApplication : public QMainWindow
{
    Q_OBJECT

public:
    ChangeDetectionApplication(QWidget *parent = nullptr);
    ~ChangeDetectionApplication();

private slots:
    void openFileDialog();
    void openFileDialogForLeftWidget();
    void openFileDialogForCenterWidget();
    void performChangeDetection();

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;

private:
    Ui::ChangeDetectionApplicationClass ui;

    QLabel* leftWidget;
    QLabel* centerWidget;
    QLabel* rightWidget;
    QLabel* clickedWidget;

    QLabel* leftLabel;
    QLabel* centerLabel;
    QLabel* rightLabel;

    QPushButton* leftButton;
    QPushButton* centerButton;
    QPushButton* rightButton;

    QPixmap beforeImage;
    QPixmap afterImage;
    QPixmap resultImage;

    QString beforeImagePath;
    QString afterImagePath;
    QString resultImagePath;

    QProcess* process;
};
