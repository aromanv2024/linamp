#ifndef AUDIOSOURCEPYTHON_H
#define AUDIOSOURCEPYTHON_H

#define PY_SSIZE_T_CLEAN
#undef slots
#include <Python.h>
#define slots Q_SLOTS

#include <QObject>
#include <QTimer>
#include <QtConcurrent>

#include "audiosourcewspectrumcapture.h"

class AudioSourcePython : public AudioSourceWSpectrumCapture
{
    Q_OBJECT
public:
    explicit AudioSourcePython(QString module, QString className, QObject *parent = nullptr);
    ~AudioSourcePython();

public slots:
    void activate();
    void deactivate();
    void handlePl();
    void handlePrevious();
    void handlePlay();
    void handlePause();
    void handleStop();
    void handleNext();
    void handleOpen();
    void handleShuffle();
    void handleRepeat();
    void handleSeek(int mseconds);

private:
    bool isActive = false;

    PyObject *playerModule;
    PyObject *player;

    QTimer *progressRefreshTimer = nullptr;
    QTimer *progressInterpolateTimer = nullptr;
    QElapsedTimer progressInterpolateElapsedTimer;
    quint32 currentProgress = 0;
    void refreshProgress();
    void interpolateProgress();

    // Python event loop thread
    void runPythonLoop();
    QFutureWatcher<void> pyLoopWatcher;

    // Poll events thread
    QTimer *pollEventsTimer = nullptr;
    bool pollInProgress = false;
    void pollEvents();
    bool doPollEvents();
    void handlePollResult();
    QFutureWatcher<bool> pollResultWatcher;

    // Load details thread
    void doLoad();
    void handleLoadEnd();
    QFutureWatcher<void> loadWatcher;

    // Eject thread
    void doEject();
    void handleEjectEnd();
    QFutureWatcher<void> ejectWatcher;

    QString currentStatus; // Status as it comes from python
    bool isShuffleEnabled = false;
    bool isRepeatEnabled = false;

    void refreshStatus(bool shouldRefreshTrackInfo = true);
    void refreshTrackInfo(bool force = false);
    void refreshMessage();

    QMediaMetaData currentMetadata;
};

#endif // AUDIOSOURCEPYTHON_H
