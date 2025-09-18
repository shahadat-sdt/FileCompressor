#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>
#include <QTextStream>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Disable edit/compress/decompress buttons initially
    ui->editFileButton->setEnabled(false);
    ui->compressButton->setEnabled(false);
    ui->decompressButton->setEnabled(false);

    connect(ui->pickFileButton, &QPushButton::clicked, this, &MainWindow::pickFile);
    connect(ui->createFileButton, &QPushButton::clicked, this, &MainWindow::createFile);
    connect(ui->editFileButton, &QPushButton::clicked, this, &MainWindow::editFile);
    connect(ui->compressButton, &QPushButton::clicked, this, &MainWindow::compressFile);
    connect(ui->decompressButton, &QPushButton::clicked, this, &MainWindow::decompressFile);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Pick an existing text file
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
    EditorWindow *editor = new EditorWindow(this);
    connect(editor, &EditorWindow::fileSaved, this, [=](const QString &savedFilePath){
        currentFilePath = savedFilePath;
        displayFileInfo(savedFilePath);
        ui->editFileButton->setEnabled(true);
        ui->compressButton->setEnabled(true);
        ui->decompressButton->setEnabled(true);
    });
    editor->setWindowTitle("New File");
    editor->show();
}

// Edit the current file
void MainWindow::editFile()
{
    if (currentFilePath.isEmpty()) return;

    EditorWindow *editor = new EditorWindow(this, currentFilePath);
    connect(editor, &EditorWindow::fileSaved, this, [=](const QString &savedFilePath){
        currentFilePath = savedFilePath;
        displayFileInfo(savedFilePath);
    });
    editor->setWindowTitle("Edit File");
    editor->show();
}

// Display file information in the UI
void MainWindow::displayFileInfo(const QString &filePath)
{
    QFileInfo info(filePath);
    ui->fileNameValue->setText(info.fileName());
    ui->filePathValue->setText(info.absoluteFilePath());
    ui->fileSizeValue->setText(QString::number(info.size()) + " bytes");
    ui->fileStatusValue->setText(info.exists() ? "File ready" : "File missing");
}

// Dummy compress function (to connect later to Huffman)
void MainWindow::compressFile()
{
    if (currentFilePath.isEmpty()) return;
    QMessageBox::information(this, "Compress", "Compress functionality will be implemented here.");
}

// Dummy decompress function (to connect later to Huffman)
void MainWindow::decompressFile()
{
    if (currentFilePath.isEmpty()) return;
    QMessageBox::information(this, "Decompress", "Decompress functionality will be implemented here.");
}
