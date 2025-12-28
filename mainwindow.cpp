#include "mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>

Worker::Worker(QObject* parent) : QThread(parent)
{
}

void Worker::stop()
{
    m_stop = true;
}

void Worker::run()
{
    // 各処理名と進捗値を配列で管理しループで処理
    // 状態マシンで各処理を管理し、進捗%を自動計算
    currentState = State::A;
    currentIndex = 0;
    while (currentState != State::Finished && !m_stop) {
        int percent = ((currentIndex + 1) * 100) / stateCount;
        QString name;
        switch (currentState) {
            case State::A:
                processA();
                name = "A";
                break;
            case State::B:
                processB();
                name = "B";
                break;
            case State::C:
                processC();
                name = "C";
                break;
            case State::D:
                processD();
                name = "D";
                break;
            default:
                name = "";
                percent = 0;
                break;
        }
        emit progressChanged(percent, name + " done");
        nextState();
        ++currentIndex;
    }
    emit finished();
}

// --- 各プロセスの処理テンプレート ---
void Worker::processA() // TODO: Aの処理をここに実装
{
    QThread::sleep(1);
}

void Worker::processB() // TODO: Bの処理をここに実装
{
    QThread::sleep(1);
}

void Worker::processC() // TODO: Cの処理をここに実装
{
    QThread::sleep(1);
}

void Worker::processD() // TODO: Dの処理をここに実装
{
    QThread::sleep(1);
}

void Worker::nextState() // 状態遷移（次の状態へ）
{
    switch (currentState) {
        case State::A:
            currentState = State::B;
            break;
        case State::B:
            currentState = State::C;
            break;
        case State::C:
            currentState = State::D;
            break;
        case State::D:
            currentState = State::Finished;
            break;
        default:
            currentState = State::Finished;
            break;
    }
}

void Worker::retryState() // リトライ（同じ状態を再実行）
{
    // 何もしない（currentStateを維持）
}

void Worker::skipState() // スキップ（次の状態へ進む）
{
    nextState();
}

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
