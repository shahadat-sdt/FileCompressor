//
// Created by devel on 9/21/2025.
//
#include "advancedDetailsWindow.h"
#include "ui_advancedDetailsWindow.h"
#include <QTableWidgetItem>
#include <QDebug>

AdvancedDetailsWindow::AdvancedDetailsWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AdvancedDetailsWindow)
{
    ui->setupUi(this);
}

AdvancedDetailsWindow::~AdvancedDetailsWindow()
{
    delete ui;
}

void AdvancedDetailsWindow::setFrequencyMap(const QMap<QString, int> &freqMap)
{
    frequencyMap = freqMap;

    // Fill table initially unsorted
    QVector<QPair<QString,int>> vec;
    for(auto it = frequencyMap.begin(); it != frequencyMap.end(); ++it) {
        vec.append(qMakePair(it.key(), it.value()));
    }

    updateTable(vec);
}

void AdvancedDetailsWindow::updateTable(const QVector<QPair<QString,int>> &vec)
{
    ui->frequencyTable->setRowCount(vec.size());
    for(int i = 0; i < vec.size(); i++){
        ui->frequencyTable->setItem(i, 0, new QTableWidgetItem(vec[i].first));
        ui->frequencyTable->setItem(i, 1, new QTableWidgetItem(QString::number(vec[i].second)));
    }
}

// Sorting algorithms
void AdvancedDetailsWindow::insertionSort(QVector<QPair<QString,int>> &vec)
{
    for(int i=1;i<vec.size();i++){
        auto key = vec[i];
        int j = i-1;
        while(j>=0 && vec[j].second < key.second){ // Descending
            vec[j+1] = vec[j];
            j--;
        }
        vec[j+1] = key;
    }
}

void AdvancedDetailsWindow::selectionSort(QVector<QPair<QString,int>> &vec)
{
    for(int i=0;i<vec.size()-1;i++){
        int maxIdx = i;
        for(int j=i+1;j<vec.size();j++){
            if(vec[j].second > vec[maxIdx].second)
                maxIdx = j;
        }
        if(maxIdx != i)
            std::swap(vec[i], vec[maxIdx]);
    }
}

void AdvancedDetailsWindow::merge(QVector<QPair<QString,int>> &vec, int left, int mid, int right)
{
    QVector<QPair<QString,int>> temp;
    int i=left, j=mid+1;
    while(i<=mid && j<=right){
        if(vec[i].second >= vec[j].second) temp.append(vec[i++]);
        else temp.append(vec[j++]);
    }
    while(i<=mid) temp.append(vec[i++]);
    while(j<=right) temp.append(vec[j++]);

    for(int k=0;k<temp.size();k++)
        vec[left+k] = temp[k];
}

void AdvancedDetailsWindow::mergeSort(QVector<QPair<QString,int>> &vec, int left, int right)
{
    if(left>=right) return;
    int mid = (left+right)/2;
    mergeSort(vec, left, mid);
    mergeSort(vec, mid+1, right);
    merge(vec, left, mid, right);
}

void AdvancedDetailsWindow::on_sortButton_clicked()
{
    QVector<QPair<QString,int>> vec;
    for(auto it = frequencyMap.begin(); it != frequencyMap.end(); ++it)
        vec.append(qMakePair(it.key(), it.value()));

    QString algo = ui->sortingComboBox->currentText();
    if(algo == "Insertion Sort") insertionSort(vec);
    else if(algo == "Selection Sort") selectionSort(vec);
    else if(algo == "Merge Sort") mergeSort(vec, 0, vec.size()-1);

    updateTable(vec);
}
