#ifndef QCVDETECTFILTER_H
#define QCVDETECTFILTER_H

#include <QAbstractVideoFilter>
#include <QDebug>
#include <QTemporaryFile>
#include <QMutex>
#include "opencv2/opencv.hpp"
#include "sync_queue.h"

#include <vector>
#include <thread>

// TODO: add namespace

struct Frame
{
    int id;
    int height;
    int width;
    int size;
    uchar* bits;
};

class QCvDetectFilter : public QAbstractVideoFilter
{
    Q_OBJECT
public:
    explicit QCvDetectFilter(QObject *parent = nullptr) : QAbstractVideoFilter(parent)
    {
        qDebug() << "Ctor QCvDetectFilter";
        launch_consumer_threads();
    }
    QVideoFilterRunnable *createFilterRunnable();
    void enqueue_frame(QVideoFrame* frame, int height, int width, int size);
    ~QCvDetectFilter();

    static constexpr auto FILE_PATH              {"frames/"};
    static constexpr auto FILE_NAME_FORMAT       {"frame%04d.png"};
    static constexpr auto CONSUMER_THREADS_COUNT {1};

signals:
    void objectDetected(float x, float y, float w, float h);

public slots:

private:
    int frame_count{0};
    sync_queue<Frame> sq;
    std::vector<std::thread> consumers;

    void launch_consumer_threads(void);

};

class QCvDetectFilterRunnable : public QVideoFilterRunnable
{
public:
    QCvDetectFilterRunnable(QCvDetectFilter *creator) {filter = creator;}
    QVideoFrame run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags);

private:
    static cv::Mat yuvFrameToMat8(const QVideoFrame &frame);
    QCvDetectFilter *filter;
};

#endif // QCVDETECTFILTER_H
