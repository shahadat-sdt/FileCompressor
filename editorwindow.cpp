#include "editorwindow.h"
#include "ui_editorwindow.h"

#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>

EditorWindow::EditorWindow(QWidget *parent, const QString &filePath)
    : QWidget(nullptr), // top-level window
      ui(new Ui::EditorWindow),
      currentFilePath(filePath)
{
    ui->setupUi(this);

    // Modern Save button style
    ui->saveButton->setStyleSheet(
        "QPushButton {"
        "font-size: 12pt;"
        "padding: 8px 20px;"
        "background-color: #0078D7;"
        "color: white;"
        "border: none;"
        "border-radius: 4px;"
        "}"
        "QPushButton:hover { background-color: #005A9E; }"
        "QPushButton:pressed { background-color: #004578; }"
    );

    connect(ui->saveButton, &QPushButton::clicked, this, &EditorWindow::saveFile);

    if (!currentFilePath.isEmpty()) {
        loadFile(currentFilePath);
    }

    // Make window resizeable with layouts
    this->setMinimumSize(400, 300);
    this->resize(800, 600);
    this->setWindowFlags(Qt::Window);
}

EditorWindow::~EditorWindow()
{
    delete ui;
}

void EditorWindow::loadFile(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QTextStream in(&file);
    ui->textEdit->setPlainText(in.readAll());
    file.close();
}

void EditorWindow::saveFile()
{
    QString savePath = currentFilePath;
    if (savePath.isEmpty()) {
        savePath = QFileDialog::getSaveFileName(
            this,
            "Save File",
            "untitled.txt",   // placeholder name
            "Text Files (*.txt)"
        );
        if (savePath.isEmpty()) return;
        currentFilePath = savePath;
    }

    QFile file(savePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return;

    QTextStream out(&file);
    out << ui->textEdit->toPlainText();
    file.close();

    emit fileSaved(currentFilePath);

    // Close editor window and return to main window
    this->close();
}
