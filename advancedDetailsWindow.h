#ifndef ADVANCEDDETAILSWINDOW_H
#define ADVANCEDDETAILSWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QString>
#include <QVector>
#include <QPair>

QT_BEGIN_NAMESPACE
namespace Ui { class AdvancedDetailsWindow; }
QT_END_NAMESPACE

class AdvancedDetailsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AdvancedDetailsWindow(QWidget *parent = nullptr);
    ~AdvancedDetailsWindow();

    // Set the frequency map (from MainWindow)
    void setFrequencyMap(const QMap<QString, int> &freqMap);

private slots:
    void on_sortButton_clicked();

private:
    Ui::AdvancedDetailsWindow *ui;
    QMap<QString, int> frequencyMap;

    // Sorting functions
    void insertionSort(QVector<QPair<QString,int>> &vec);
    void selectionSort(QVector<QPair<QString,int>> &vec);
    void mergeSort(QVector<QPair<QString,int>> &vec, int left, int right);
    void merge(QVector<QPair<QString,int>> &vec, int left, int mid, int right);

    void updateTable(const QVector<QPair<QString,int>> &vec);
};

#endif // ADVANCEDDETAILSWINDOW_H
