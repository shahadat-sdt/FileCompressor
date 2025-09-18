#include "editorwindow.h"
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>


EditorWindow::EditorWindow(QWidget *parent, const QString &filePath)
    : QWidget(parent),
      currentFilePath(filePath)
{
    ui.setupUi(this);

    // ------------------------------
    // Make the editor window full screen
    this->showMaximized();

    // ------------------------------
    // Style the text editor and save button
    ui.textEdit->setStyleSheet("font-size: 14pt; padding: 5px;");
    ui.saveButton->setStyleSheet(
        "font-size: 12pt;"
        "padding: 8px 20px;"
        "background-color: #4CAF50;"
        "color: white;"
        "border-radius: 5px;"
    );
    // ------------------------------

    connect(ui.saveButton, &QPushButton::clicked, this, &EditorWindow::saveFile);

    if (!currentFilePath.isEmpty()) {
        loadFile(currentFilePath);
    }
}


EditorWindow::~EditorWindow() {}

void EditorWindow::loadFile(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    ui.textEdit->setPlainText(in.readAll());
    file.close();
}

void EditorWindow::saveFile()
{
    QString savePath = currentFilePath;

    // If no file path yet, open Save As dialog
    if (savePath.isEmpty()) {
        savePath = QFileDialog::getSaveFileName(
            this,
            "Save File",
            "untitled.txt",                    // default file name
            "Text Files (*.txt);;All Files (*)" // filter
        );
        if (savePath.isEmpty()) return; // user canceled
        currentFilePath = savePath;
    }

    QFile file(savePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Cannot save file.");
        return;
    }

    QTextStream out(&file);
    out << ui.textEdit->toPlainText();
    file.close();

    emit fileSaved(currentFilePath);
    this->close();
}
