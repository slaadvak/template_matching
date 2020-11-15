#include "qcvdetectfilter.h"

#include <opencv2/imgcodecs.hpp>
#include "opencv2/core/core_c.h"

cv::CascadeClassifier classifier;

//TODO check constness

QVideoFilterRunnable* QCvDetectFilter::createFilterRunnable()
{
    return new QCvDetectFilterRunnable(this);
}

void QCvDetectFilter::enqueue_frame(QVideoFrame* frame, int height, int width, int size)
{
    Q_CHECK_PTR(frame);

    // This array will be deleted by the consumer threads
    uchar* bits = new uchar[size];

    Q_CHECK_PTR(bits);

    std::memcpy(bits, frame->bits(), size);

    sq.push({frame_count, height, width, size, bits});
    frame_count += 1;
}

QCvDetectFilter::~QCvDetectFilter()
{
    for(size_t i = 0; i < consumers.size(); i++)
    {
        sq.push({0, 0, 0, 0, nullptr});
    }

    for (auto& consumer : consumers)
    {
        consumer.join();
    }
}

void QCvDetectFilter::launch_consumer_threads()
{
    for(size_t i = 0; i < CONSUMER_THREADS_COUNT; i+=1)
    {
        std::thread t([this](int id)
        {
            qDebug() << "Starting T#" << id;

            while(true)
            {
                // Retrieve frame
                Frame f;
                sq.pop(f);

                // Are we asked to stopped ?
                if(f.bits == nullptr)
                {
                    break;
                }

                qDebug() << "T#" << id << " on frame #" << f.id;

//                cv::Mat mat(f.height, f.width, CV_8UC1, f.bits);

                cv::Mat tmp(f.height, f.width, CV_8UC1, f.bits);
                cv::Mat mat(f.height, f.width, CV_8UC3);
                cv::cvtColor(tmp, mat, cv::COLOR_YUV2RGB_YV12);

                // Create filename
                auto size = std::snprintf(nullptr, 0, FILE_NAME_FORMAT, f.id);
                std::string filename(size + 1, '\0');
                std::sprintf(&filename[0], FILE_NAME_FORMAT, f.id);

                // Write file
                if(! cv::imwrite(FILE_PATH + filename, mat))
                {
                    qDebug() << "Error writing file " << filename.c_str();
                }

                // Delete bits array created in enqueue_frame
                delete[] f.bits;
            }

            qDebug() << "Deleting T#" << id;

        }, i);

        consumers.push_back(std::move(t));
    }
}

QVideoFrame QCvDetectFilterRunnable::run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags)
{
    Q_UNUSED(flags);

    Q_CHECK_PTR(input);

    static int counter = 0;
    qDebug() << "Counter=" << counter;

    counter += 1;

    if(! input->map(QAbstractVideoBuffer::ReadOnly))
    {
        qDebug() << "Cannot map VideoFrame!";
    }
    else
    {
        if(surfaceFormat.handleType() == QAbstractVideoBuffer::NoHandle)
        {
//            uchar abuf[input->height() * input->width()];
//            std::memcpy(abuf, input->bits(), sizeof(abuf));


//            cv::Mat tmp(input->height() /*+ input->height() / 2*/, input->width(), CV_8UC1, (uchar *) input->bits());
//            cv::Mat tmp(input->height() /*+ input->height() / 2*/, input->width(), CV_8UC1, abuf);
    //        cv::Mat mat(input->height(), input->width(), CV_8UC3);
    //        cv::cvtColor(tmp, mat, cv::COLOR_YUV2RGB_YV12);

//            cv::imwrite("img.png", tmp);


//            filter->enqueue_frame(input, input->height(), input->width());
            filter->enqueue_frame(input, input->height(), input->width(), input->mappedBytes());


            //////////////////////////////////////////
                /*
                 * Resize in not mandatory but it can speed up things quite a lot!
                 */
    //            QSize resized = image.size().scaled(320, 240, Qt::KeepAspectRatio);
    //            cv::resize(mat, mat, cv::Size(resized.width(), resized.height()));
            //////////////////////////////////////////

    //            classifier.detectMultiScale(mat, detected, 1.1);

    //            // We'll use only the first detection to make sure things are not slow on the qml side
    //            if(detected.size() > 0)
    //            {
    //                // Normalize x,y,w,h to values between 0..1 and send them to UI
    //                emit filter->objectDetected(float(detected[0].x) / float(mat.cols),
    //                        float(detected[0].y) / float(mat.rows),
    //                        float(detected[0].width) / float(mat.cols),
    //                        float(detected[0].height) / float(mat.rows));
    //            }
    //            else
    //            {
    //                emit filter->objectDetected(0.0,
    //                                            0.0,
    //                                            0.0,
    //                                            0.0);
    //            }
    //        }
        }
        else
        {
            qDebug() << "Other surface formats are not supported yet!";
        }

        input->unmap();
    }

    return *input;
}

//cv::Mat QCvDetectFilterRunnable::yuvFrameToMat8(const QVideoFrame &frame)
//{
//    Q_ASSERT(frame.handleType() == QAbstractVideoBuffer::NoHandle && frame.isReadable());
//    Q_ASSERT(frame.pixelFormat() == QVideoFrame::Format_YUV420P || frame.pixelFormat() == QVideoFrame::Format_NV12);



//    int code = 0;

//    if(frame.pixelFormat() == QVideoFrame::Format_YUV420P)
//    {
//        code = cv::COLOR_YUV2RGB_YV12;
//    }
//    else
//    {
//        code = cv::COLOR_YUV2BGR_NV12;
//    }

//    cv::cvtColor(tmp, result, code);
//    return result;
//}


