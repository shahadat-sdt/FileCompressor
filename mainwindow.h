#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "advancedDetailsWindow.h"

QT_BEGIN_NAMESPACE

namespace Ui {
    class MainWindow;
}

QT_END_NAMESPACE

class MainWindow : public QMainWindow {
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

    void showCompressedCode();

    void openAdvancedDetails();

private:
    AdvancedDetailsWindow *advancedWindow;
    // Use the correct class name
    Ui::MainWindow *ui;
    QString currentFilePath;
    QString lastCompressedPath;
    void showCompressedDataFull(const QString &hexData);

    void displayFileInfo(const QString &filePath);

    void updateCompressionInfo(const QString &original, const QString &compressed);
};

#endif // MAINWINDOW_H
