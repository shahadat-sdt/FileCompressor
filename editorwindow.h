#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class EditorWindow; }
QT_END_NAMESPACE

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
    Ui::EditorWindow *ui;
    QString currentFilePath;

    void loadFile(const QString &filePath);
};
