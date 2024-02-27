#ifndef OPENCV_FORM_H
#define OPENCV_FORM_H

#include <QWidget>
#include <opencv2/imgcodecs.hpp>

class OpenCVForm : public QWidget
{
    Q_OBJECT

public:
    explicit OpenCVForm(QWidget *parent = nullptr);

protected:
    cv::Mat image;

    void paintEvent(QPaintEvent *) override;
};

#endif // OPENCV_FORM_H
