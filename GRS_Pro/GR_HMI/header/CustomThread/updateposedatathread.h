#ifndef UPDATEPOSEDATATHREAD_H
#define UPDATEPOSEDATATHREAD_H

#include <QThread>

class UpdatePoseDataThread : public QThread
{
    Q_OBJECT
public:
    explicit UpdatePoseDataThread(QObject *parent = 0);
    ~UpdatePoseDataThread();

protected:
    void run();

signals:
    void poseDataUpdated(QString *poseData);

public slots:

private:

};

#endif // UPDATEPOSEDATATHREAD_H
