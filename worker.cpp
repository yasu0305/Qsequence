#include "worker.h"

Worker::Worker(QObject* parent) : QThread(parent)
{
}

void Worker::stop()
{
    m_stop = true;
}

void Worker::run()
{
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
