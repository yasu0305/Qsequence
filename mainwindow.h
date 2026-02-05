#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "worker.h"
#include <QDateTime>
#include <QLabel>
#include <QMainWindow>
#include <QProgressBar>
#include <QPushButton>
#include <QTimer>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onStartClicked();
    void onStopClicked();
    void onProgressChanged(int value, const QString& message);
    void updateDateTime();
    void onWorkerFinished();

private:
    void          setupUi();
    QLabel*       label;
    QLabel*       dateTimeLabel;
    QPushButton*  startButton;
    QPushButton*  stopButton;
    QProgressBar* progressBar;
    Worker*       worker;
    QTimer*       timer;
};

#endif // MAINWINDOW_H
