#ifndef CAPTUREFORM_H
#define CAPTUREFORM_H

#include "opencv_form.h"
#include <opencv2/videoio.hpp>

class CaptureForm : public OpenCVForm
{
    Q_OBJECT
public:
    CaptureForm();
    ~CaptureForm();

    inline void __fastcall setInterval(const int intval)
    {
        interval = intval;
    }

    inline cv::Mat __fastcall captureImage() const
    {
        return captureMat;
    }

    bool __fastcall start();

protected:
    void timerEvent(QTimerEvent*) override;

private:
    int id = 0;
    int interval = 33;
    cv::VideoCapture capture;
    cv::Mat captureMat;
};

#endif // CAPTUREFORM_H
