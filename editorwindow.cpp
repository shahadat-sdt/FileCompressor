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
    if (savePath.isEmpty()) {
        savePath = QFileDialog::getSaveFileName(this, "Save File");
        if (savePath.isEmpty()) return;
        currentFilePath = savePath;
    }

    QFile file(savePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return;

    QTextStream out(&file);
    out << ui.textEdit->toPlainText();
    file.close();

    emit fileSaved(currentFilePath);

    // Close this editor window and return to the previous window
    this->close();
}
