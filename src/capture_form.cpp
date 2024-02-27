#include "capture_form.h"
#include <opencv2/imgproc.hpp>

CaptureForm::CaptureForm() {}

CaptureForm::~CaptureForm()
{
    if (0 != id)
        QObject::killTimer(id);
    capture.release();
}

bool __fastcall CaptureForm::start()
{
    capture.open(0);
    if (!capture.isOpened())
        return false;

    id = QObject::startTimer(interval, Qt::PreciseTimer);
    return true;
}

void CaptureForm::timerEvent(QTimerEvent*)
{
    capture >> captureMat;
    cv::cvtColor(captureMat, image, cv::COLOR_BGRA2RGB);
    update();
}
