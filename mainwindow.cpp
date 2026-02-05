#include "mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>

void MainWindow::setupUi()
{
    label         = new QLabel("Progress: Not started");
    dateTimeLabel = new QLabel(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
    startButton   = new QPushButton("Start");
    stopButton    = new QPushButton("Stop");
    progressBar   = new QProgressBar;
    progressBar->setRange(0, 100);
    progressBar->setValue(0);
    stopButton->setEnabled(false);

    QHBoxLayout* dateTimeLayout = new QHBoxLayout;
    dateTimeLayout->addStretch();
    dateTimeLayout->addWidget(dateTimeLabel);

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(startButton);
    buttonLayout->addWidget(stopButton);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(dateTimeLayout);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(label);
    mainLayout->addWidget(progressBar);

    QWidget* central = new QWidget;
    central->setLayout(mainLayout);
    setCentralWidget(central);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateDateTime);
    timer->start(1000);

    connect(startButton, &QPushButton::clicked, this, &MainWindow::onStartClicked);
    connect(stopButton, &QPushButton::clicked, this, &MainWindow::onStopClicked);
}

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), worker(nullptr)
{
    setupUi();
}

MainWindow::~MainWindow()
{
    if (worker) {
        worker->stop();
        worker->wait();
        delete worker;
    }
}

void MainWindow::onStartClicked()
{
    startButton->setEnabled(false);
    stopButton->setEnabled(true);
    progressBar->setValue(0);
    label->setText("Progress: Started");
    worker = new Worker(this);
    connect(worker, &Worker::progressChanged, this, &MainWindow::onProgressChanged);
    connect(worker, &Worker::finished, this, &MainWindow::onWorkerFinished);
    worker->start(); // 新しいスレッドでWorker::run()が実行される
}

void MainWindow::onStopClicked()
{
    if (worker)
        worker->stop();
    // Stop時はUIをロックし、リセットはonWorkerFinishedで行う
    startButton->setEnabled(false);
    stopButton->setEnabled(false);
}

void MainWindow::onProgressChanged(int value, const QString& message)
{
    progressBar->setValue(value);
    label->setText("Progress: " + message);
}

void MainWindow::updateDateTime()
{
    dateTimeLabel->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
}

void MainWindow::onWorkerFinished()
{
    // Worker終了後にUIをリセット
    startButton->setEnabled(true);
    stopButton->setEnabled(false);
    progressBar->setValue(0);
    label->setText("Progress: Not started");
    if (worker) {
        worker->deleteLater();
        worker = nullptr;
    }
}
