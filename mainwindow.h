#pragma once

#include <QMainWindow>
#include "editorwindow.h"  // <<<<<< Include this

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void pickFile();
    void createFile();
    void editFile();
    void compressFile();
    void decompressFile();

private:
    Ui::MainWindow *ui;
    QString currentFilePath;

    void displayFileInfo(const QString &filePath);
};
