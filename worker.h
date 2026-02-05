#ifndef WORKER_H
#define WORKER_H

#include <QThread>
#include <QString>

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

#endif // WORKER_H
