#pragma once

#include <QWidget>
#include "ui_editorwindow.h" // generated from editorwindow.ui

class EditorWindow : public QWidget
{
    Q_OBJECT

public:
    explicit EditorWindow(QWidget *parent = nullptr, const QString &filePath = "");
    ~EditorWindow();

    signals:
        void fileSaved(const QString &filePath);

private slots:
    void saveFile();

private:
    Ui::EditorWindow ui; // NOT a pointer
    QString currentFilePath;

    void loadFile(const QString &filePath);
};
