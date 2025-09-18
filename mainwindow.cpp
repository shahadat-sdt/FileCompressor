#include "mainwindow.h"
#include "huffman.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Connect buttons
    connect(ui->compressButton, &QPushButton::clicked, this, &MainWindow::onCompressClicked);
    connect(ui->decompressButton, &QPushButton::clicked, this, &MainWindow::onDecompressClicked);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onCompressClicked() {
    QString inputFile = QFileDialog::getOpenFileName(this, "Select file to compress");
    if (inputFile.isEmpty()) return;

    QString outputFile = QFileDialog::getSaveFileName(this, "Save compressed file", "", "Huffman (*.huff)");
    if (outputFile.isEmpty()) return;

    std::string error;
    if (Huffman::compressFile(inputFile.toStdString(), outputFile.toStdString(), error)) {
        QFileInfo in(inputFile), out(outputFile);
        QMessageBox::information(this, "Success",
            QString("Compressed!\nOriginal size: %1 bytes\nCompressed size: %2 bytes")
            .arg(in.size())
            .arg(out.size()));
    } else {
        QMessageBox::critical(this, "Error", QString::fromStdString(error));
    }
}

void MainWindow::onDecompressClicked() {
    QString inputFile = QFileDialog::getOpenFileName(this, "Select file to decompress", "", "Huffman (*.huff)");
    if (inputFile.isEmpty()) return;

    QString outputFile = QFileDialog::getSaveFileName(this, "Save decompressed file");
    if (outputFile.isEmpty()) return;

    std::string error;
    if (Huffman::decompressFile(inputFile.toStdString(), outputFile.toStdString(), error)) {
        QFileInfo in(inputFile), out(outputFile);
        QMessageBox::information(this, "Success",
            QString("Decompressed!\nCompressed size: %1 bytes\nDecompressed size: %2 bytes")
            .arg(in.size())
            .arg(out.size()));
    } else {
        QMessageBox::critical(this, "Error", QString::fromStdString(error));
    }
}
