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

    // Modern flat button style
    QString btnStyle =
        "QPushButton {"
        "font-size: 12pt;"
        "padding: 8px 20px;"
        "background-color: #0078D7;"
        "color: white;"
        "border: none;"
        "border-radius: 4px;"
        "}"
        "QPushButton:hover { background-color: #005A9E; }"
        "QPushButton:pressed { background-color: #004578; }";

    ui->pickFileButton->setStyleSheet(btnStyle);
    ui->createFileButton->setStyleSheet(btnStyle);
    ui->editFileButton->setStyleSheet(btnStyle);
    ui->compressButton->setStyleSheet(btnStyle);
    ui->decompressButton->setStyleSheet(btnStyle);

    // Optional: add spacing between layouts
    ui->centralwidget->layout()->setSpacing(10);

    // Disable edit/compress/decompress buttons initially
    ui->editFileButton->setEnabled(false);
    ui->compressButton->setEnabled(false);
    ui->decompressButton->setEnabled(false);

    // Connect buttons to slots
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
    if (currentFilePath.isEmpty()) {
        QMessageBox::warning(this, "No file", "Please select a file first.");
        return;
    }

    EditorWindow *editorWindow = new EditorWindow(this, currentFilePath);

    connect(editorWindow, &EditorWindow::fileSaved, this, [this](const QString &path){
        displayFileInfo(path);
    });

    editorWindow->show();
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

// Dummy compress function
void MainWindow::compressFile()
{
    if (currentFilePath.isEmpty()) return;
    QMessageBox::information(this, "Compress", "Compress functionality will be implemented here.");
}

// Dummy decompress function
void MainWindow::decompressFile()
{
    if (currentFilePath.isEmpty()) return;
    QMessageBox::information(this, "Decompress", "Decompress functionality will be implemented here.");
}
