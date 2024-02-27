#include "opencv_form.h"
#include <QPainter>

OpenCVForm::OpenCVForm(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
}

void OpenCVForm::paintEvent(QPaintEvent*)
{
    if (image.empty())
        return;

    QPainter painter(this);
    // 将OpenCV图像转换为QImage
    QImage qimage = QImage((const unsigned char*)(image.data), image.cols, image.rows, image.step, QImage::Format_RGB888);
    // 在Qt窗口上绘制QImage
    painter.drawImage(0, 0, qimage);
}
