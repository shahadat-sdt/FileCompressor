#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>
#include <QDebug>

#include "huffman.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Disable edit/compress/decompress buttons initially
    ui->editFileButton->setEnabled(false);
    ui->compressButton->setEnabled(false);
    ui->decompressButton->setEnabled(false);

    // Modern flat buttons
    // QString btnStyle =
    //     "QPushButton {"
    //     "font-size: 12pt;"
    //     "padding: 8px 20px;"
    //     "background-color: #0078D7;"
    //     "color: white;"
    //     "border: none;"
    //     "border-radius: 4px;"
    //     "}"
    //     "QPushButton:hover { background-color: #005A9E; }"
    //     "QPushButton:pressed { background-color: #004578; }"
    // ;
    //
    // ui->pickFileButton->setStyleSheet(btnStyle);
    // ui->createFileButton->setStyleSheet(btnStyle);
    // ui->editFileButton->setStyleSheet(btnStyle);
    // ui->compressButton->setStyleSheet(btnStyle);
    // ui->decompressButton->setStyleSheet(btnStyle);

    // Connect buttons
    connect(ui->pickFileButton, &QPushButton::clicked, this, &MainWindow::pickFile);
    connect(ui->createFileButton, &QPushButton::clicked, this, &MainWindow::createFile);
    connect(ui->editFileButton, &QPushButton::clicked, this, &MainWindow::editFile);
    connect(ui->compressButton, &QPushButton::clicked, this, &MainWindow::compressFile);
    connect(ui->decompressButton, &QPushButton::clicked, this, &MainWindow::decompressFile);

    // Apply OS theme (dark/light)
    this->setStyleSheet("QMainWindow { background-color: palette(window); }");
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

// Display file information in the UI
void MainWindow::displayFileInfo(const QString &filePath)
{
    QFileInfo info(filePath);
    ui->fileNameValue->setText(info.fileName());
    ui->filePathValue->setText(info.absoluteFilePath());
    ui->fileSizeValue->setText(QString::number(info.size()) + " bytes");
    ui->fileStatusValue->setText(info.exists() ? "File ready" : "File missing");
}

// Compress using Huffman
void MainWindow::compressFile()
{
    if (currentFilePath.isEmpty()) return;

    QString outFile = QFileDialog::getSaveFileName(this, "Save Compressed File", currentFilePath + ".huff", "Huffman Files (*.huff)");
    if (outFile.isEmpty()) return;

    std::string err;
    bool ok = Huffman::compressFile(currentFilePath.toStdString(), outFile.toStdString(), err);
    if (ok) QMessageBox::information(this, "Compress", "File compressed successfully!");
    else QMessageBox::warning(this, "Compress Failed", QString::fromStdString(err));
}

// Decompress using Huffman
void MainWindow::decompressFile()
{
    if (currentFilePath.isEmpty()) return;

    QString outFile = QFileDialog::getSaveFileName(this, "Save Decompressed File", "decompressed.txt", "Text Files (*.txt)");
    if (outFile.isEmpty()) return;

    std::string err;
    bool ok = Huffman::decompressFile(currentFilePath.toStdString(), outFile.toStdString(), err);
    if (ok) QMessageBox::information(this, "Decompress", "File decompressed successfully!");
    else QMessageBox::warning(this, "Decompress Failed", QString::fromStdString(err));
}