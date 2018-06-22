#ifndef DECODETHREAD_H
#define DECODETHREAD_H

#include <QThread>



class DecodeThread : public QThread
{    
    Q_OBJECT
public:
//    explicit DecodeThread();
    DecodeThread();
    ~DecodeThread();

protected:
    void run();


};

#endif
