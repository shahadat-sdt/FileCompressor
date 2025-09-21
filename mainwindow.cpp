#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>
#include <QTextStream>
#include <QDebug>

#include "editorwindow.h"
#include "huffman.h"   // your Huffman class

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Disable initially
    ui->editFileButton->setEnabled(false);
    ui->compressButton->setEnabled(false);
    ui->decompressButton->setEnabled(false);
    ui->viewCodeButton->setEnabled(false);

    // Connect
    connect(ui->pickFileButton, &QPushButton::clicked, this, &MainWindow::pickFile);
    connect(ui->createFileButton, &QPushButton::clicked, this, &MainWindow::createFile);
    connect(ui->editFileButton, &QPushButton::clicked, this, &MainWindow::editFile);
    connect(ui->compressButton, &QPushButton::clicked, this, &MainWindow::compressFile);
    connect(ui->decompressButton, &QPushButton::clicked, this, &MainWindow::decompressFile);
    connect(ui->viewCodeButton, &QPushButton::clicked, this, &MainWindow::showCompressedCode);

    // Apply OS theme background
    this->setStyleSheet("QMainWindow { background-color: palette(window); }");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::pickFile()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Select a text file", "", "Text Files (*.txt);;All Files (*)");
    if (filePath.isEmpty()) return;

    currentFilePath = filePath;
    displayFileInfo(filePath);

    ui->editFileButton->setEnabled(true);
    ui->compressButton->setEnabled(true);
    ui->decompressButton->setEnabled(true);
}

// Create a new text file
void MainWindow::createFile()
{
    // Hide main window
    this->hide();

    // Create editor window for a new file
    EditorWindow *editorWindow = new EditorWindow(nullptr);
    editorWindow->setAttribute(Qt::WA_DeleteOnClose); // Auto-delete when closed
    editorWindow->setWindowTitle("New File");
    editorWindow->show();

    // Restore main window after editor closes
    connect(editorWindow, &EditorWindow::destroyed, this, [this](){
        this->show();
    });

    // Update UI when file is saved
    connect(editorWindow, &EditorWindow::fileSaved, this, [this](const QString &savedFilePath){
        currentFilePath = savedFilePath;
        displayFileInfo(savedFilePath);
        ui->editFileButton->setEnabled(true);
        ui->compressButton->setEnabled(true);
        ui->decompressButton->setEnabled(true);
    });
}

// Edit the current file
void MainWindow::editFile()
{
    if (currentFilePath.isEmpty()) {
        QMessageBox::warning(this, "No file", "Please select a file first.");
        return;
    }

    // Hide main window
    this->hide();

    EditorWindow *editorWindow = new EditorWindow(nullptr, currentFilePath);
    editorWindow->setAttribute(Qt::WA_DeleteOnClose);
    editorWindow->show();

    connect(editorWindow, &EditorWindow::destroyed, this, [this](){
        this->show();
    });

    connect(editorWindow, &EditorWindow::fileSaved, this, [this](const QString &path){
        displayFileInfo(path);
    });
}

void MainWindow::compressFile()
{
    if (currentFilePath.isEmpty()) return;

    QString outPath = QFileDialog::getSaveFileName(this, "Save Compressed File", "", "Compressed Files (*.huff)");
    if (outPath.isEmpty()) return;

    std::string err;
    if (Huffman::compressFile(currentFilePath.toStdString(), outPath.toStdString(), err)) {
        lastCompressedPath = outPath;

        QFileInfo inInfo(currentFilePath), outInfo(outPath);
        updateCompressionInfo(QString::number(inInfo.size()), QString::number(outInfo.size()));

        ui->viewCodeButton->setEnabled(true);
        QMessageBox::information(this, "Success", "File compressed successfully.");
    } else {
        QMessageBox::warning(this, "Error", QString::fromStdString(err));
    }
}

void MainWindow::decompressFile()
{
    if (currentFilePath.isEmpty()) return;

    QString outPath = QFileDialog::getSaveFileName(this, "Save Decompressed File", "", "Text Files (*.txt)");
    if (outPath.isEmpty()) return;

    std::string err;
    if (Huffman::decompressFile(currentFilePath.toStdString(), outPath.toStdString(), err)) {
        QMessageBox::information(this, "Success", "File decompressed successfully.");
    } else {
        QMessageBox::warning(this, "Error", QString::fromStdString(err));
    }
}

void MainWindow::showCompressedCode()
{
    if (lastCompressedPath.isEmpty()) {
        QMessageBox::information(this, "No Code", "No compressed file available.");
        return;
    }

    QFile file(lastCompressedPath);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Error", "Cannot open compressed file.");
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    QString hex = data.toHex(' ').toUpper();
    QMessageBox::information(this, "Compressed Data", hex.left(1000) + (hex.size() > 1000 ? " ..." : ""));
}

void MainWindow::displayFileInfo(const QString &filePath)
{
    QFileInfo info(filePath);
    ui->fileNameValue->setText(info.fileName());
    ui->filePathValue->setText(info.absoluteFilePath());
    ui->fileSizeValue->setText(QString::number(info.size()) + " bytes");
    ui->fileStatusValue->setText(info.exists() ? "File ready" : "File missing");
}

void MainWindow::updateCompressionInfo(const QString &original, const QString &compressed)
{
    ui->originalSizeValue->setText(original + " bytes");
    ui->compressedSizeValue->setText(compressed + " bytes");

    bool ok1, ok2;
    double o = original.toDouble(&ok1);
    double c = compressed.toDouble(&ok2);

    if (ok1 && ok2 && o > 0) {
        double ratio = (1.0 - (c / o)) * 100.0;
        ui->ratioValue->setText(QString::number(ratio, 'f', 2) + " %");
    } else {
        ui->ratioValue->setText("---");
    }
}
