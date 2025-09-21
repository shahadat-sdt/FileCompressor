#include <QApplication>
#include "mainwindow.h"

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    MainWindow w;
    QApplication::setStyle("Fusion"); // optional, for consistent theming

    qApp->setStyleSheet(
        "QPushButton:disabled {"
        "   background-color: lightgray;"
        "   color: gray;"
        "   border: 1px solid gray;"
        "}"
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


    w.resize(800, 600);
    w.show();
    return app.exec();
}
