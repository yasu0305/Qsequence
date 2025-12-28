#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDateTime>
#include <QLabel>
#include <QMainWindow>
#include <QProgressBar>
#include <QPushButton>
#include <QThread>
#include <QTimer>

class Worker : public QThread
{
    Q_OBJECT
public:
    Worker(QObject* parent = nullptr);
    void stop();

signals:
    void progressChanged(int value, const QString& message);
    void finished();

protected:
    void run() override;

private:
    enum class State
    {
        A,
        B,
        C,
        D,
        Finished
    };
    static constexpr State stateList[4] = {State::A, State::B, State::C, State::D};
    static constexpr int   stateCount   = 4;
    State                  currentState = State::A;
    int                    currentIndex = 0;
    bool                   m_stop       = false;
    void                   processA();
    void                   processB();
    void                   processC();
    void                   processD();
    void                   nextState();
    void                   retryState();
    void                   skipState();
};

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
