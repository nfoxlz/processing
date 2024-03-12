#include "image_doc.h"
#include <opencv2/imgcodecs.hpp>

ImageDoc::ImageDoc() {}

ImageDoc::~ImageDoc()
{
    clearHistory();
}

ImageDoc* __fastcall ImageDoc::create(cv::Mat img)
{
    ImageDoc* result = new ImageDoc();
    result->imageMat = new cv::Mat(img);
    result->clearHistory();
    result->history.push_back(result->imageMat);
    result->index = 0;
    return result;
}

ImageDoc* __fastcall ImageDoc::readDoc(const cv::String path)
{
    cv::Mat img = cv::imread(path);
    if (img.empty())
        return nullptr;

    ImageDoc* result = create(img);
    result->imagePath = path;
    return result;
}

bool __fastcall ImageDoc::read(const cv::String path)
{
    cv::Mat img = cv::imread(path);

    bool result = !img.empty();
    if (result)
    {
        imageMat = new cv::Mat(img);
        clearHistory();
        history.push_back(imageMat);
        index = 0;
        imagePath = path;
    }
    return result;
}

bool __fastcall ImageDoc::write(const cv::String path)
{
    if (path.empty())
        return false;

    if (imagePath != path)
        imagePath = path;

    return cv::imwrite(path, *imageMat);
}

bool __fastcall ImageDoc::write()
{
    return write(imagePath);
}

void __fastcall ImageDoc::colorReduction(const uchar div)
{
    process([](cv::Mat* img, std::vector<void*> args)
    {
        const uchar div = *(uchar *)args[0];

        int r = img->rows;
        int c = img->cols * img->channels();
        if (img->isContinuous())
        {
            c *= r;
            r = 1;
        }

        for (int i = 0; i < r; i++)
        {
            uchar* data = img->ptr<uchar>(i);
            for (int j = 0; j++ < c; j++)
                data[j] = cv::saturate_cast<uchar>(data[j] / div * div + div / 2);
        }
        return true;
    }, 1, &div);
}

void __fastcall ImageDoc::gbrToGray()
{
    process([](cv::Mat* img, std::vector<void*>)
    {
        if (3 > img->channels())
            return false;

        cv::cvtColor(*img, *img, cv::COLOR_BGRA2GRAY);
        return true;
    });
}

void __fastcall ImageDoc::gbrToHsv()
{
    process([](cv::Mat* img, std::vector<void*>)
    {
        if (3 > img->channels())
            cv::cvtColor(*img, *img, cv::COLOR_GRAY2BGR);

        cv::cvtColor(*img, *img, cv::COLOR_BGR2HSV);
        return true;
    });
}

void __fastcall ImageDoc::linearTransform(const double a, const int b)
{
    process([](cv::Mat* img, std::vector<void*> args)
    {
        const int rows = img->rows;
        const int cols = img->cols;
        const int chas = img->channels();

        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
                for (int c = 0; c < chas; c++)
                    img->at<cv::Vec3b>(i, j)[c] = cv::saturate_cast<uchar>(*(float*)args[0] * (img->at<cv::Vec3b>(i, j)[c]) + *(int*)args[1]);

        return true;
    }, 2, &a, &b);
}

void __fastcall ImageDoc::makeBorder(const int top, const int bottom, const int left, const int right, const int borderType, const cv::Scalar & value)
{
    process([](cv::Mat* img, std::vector<void*> args)
    {
        cv::copyMakeBorder(*img, *img, *(int*)args[0], *(int*)args[1], *(int*)args[2], *(int*)args[3], *(int*)args[4], *(cv::Scalar*)args[5]);
        return true;
    }, 6, &top, &bottom, &left, &right, &borderType, &value);
}

void __fastcall ImageDoc::findContours()
{
    process([](cv::Mat* img, std::vector<void*>)
    {
        // cv::GaussianBlur(*img, *img, cv::Size(3, 3), 0);
        cv::Canny(*img, *img, 100, 200);
        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::Vec4i> hierarchy;
        cv::findContours(*img, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_NONE, cv::Point());
        cv::Mat imageContours = cv::Mat::zeros(img->size(), CV_8UC1);
        // cv::Mat Contours = cv::Mat::zeros(img->size(), CV_8UC1);
        // size_t contourSize;
        size_t contoursSize = contours.size();
        for (int i = 0; i < contoursSize; i++)
        {
            // contourSize = contours[i].size();
            // for (int j = 0; j < contourSize; j++)
            // {

            // }
            cv::drawContours(imageContours, contours, i, cv::Scalar(255), 1, cv::LINE_8, hierarchy);
        }
        *img = imageContours.clone();
        return true;
    });
}

void __fastcall ImageDoc::sobel(const int ddepth, const int dx, const int dy, const int ksize, const double scale, const double delta, const int borderType)
{
    process([](cv::Mat* img, std::vector<void*> args)
    {
        cv::Mat tmpX, tmpY;
        cv::Sobel(*img, tmpX, *(int*)args[0], *(int*)args[1], 0, *(int*)args[3], *(double*)args[4], *(double*)args[5], *(int*)args[6]);
        cv::convertScaleAbs(tmpX, tmpX);
        cv::Sobel(*img, tmpY, *(int*)args[0], 0, *(int*)args[2], *(int*)args[3], *(double*)args[4], *(double*)args[5], *(int*)args[6]);
        cv::convertScaleAbs(tmpY, tmpY);
        *img = tmpX + tmpY;
        return true;
    }, 7, &ddepth, &dx, &dy, &ksize, &scale, &delta, &borderType);
}

void __fastcall ImageDoc::scharr(const int ddepth, const int dx, const int dy, const double scale, const double delta, const int borderType)
{
    process([](cv::Mat* img, std::vector<void*> args)
    {
        cv::Mat tmpX, tmpY;
        cv::Scharr(*img, tmpX, *(int*)args[0], *(int*)args[1], 0, *(double*)args[3], *(double*)args[4], *(int*)args[5]);
        cv::convertScaleAbs(tmpX, tmpX);
        cv::Scharr(*img, tmpY, *(int*)args[0], 0, *(int*)args[2], *(double*)args[3], *(double*)args[4], *(int*)args[5]);
        cv::convertScaleAbs(tmpY, tmpY);
        *img = tmpX + tmpY;
        return true;
    }, 6, &ddepth, &dx, &dy, &scale, &delta, &borderType);
}

void __fastcall ImageDoc::laplacian(const int ddepth, const int ksize, const double scale, const double delta, const int borderType)
{
    process([](cv::Mat* img, std::vector<void*> args)
    {
        cv::Laplacian(*img, *img, *(int*)args[0], *(int*)args[1], *(double*)args[2], *(double*)args[3], *(int*)args[4]);
        return true;
    }, 5, &ddepth, &ksize, &scale, &delta, &borderType);
}

void __fastcall ImageDoc::canny(const double threshold1, const double threshold2, const int apertureSize, const bool l2gradient)
{
    process([](cv::Mat* img, std::vector<void*> args)
    {
        if (1 < img->channels())
            cv::cvtColor(*img, *img, cv::COLOR_BGR2GRAY);

        cv::Canny(*img, *img, *(double*)args[0], *(double*)args[1], *(int*)args[2], *(bool*)args[3]);
        return true;
    }, 4, &threshold1, &threshold2, &apertureSize, &l2gradient);
}

void __fastcall ImageDoc::difference()
{
    process([](cv::Mat* img, std::vector<void*>)
    {
        if (1 < img->channels())
            cv::cvtColor(*img, *img, cv::COLOR_BGR2GRAY);

        auto imgSize = img->size();
        int type = img->type();
        cv::Mat edgeXImage(imgSize, type);
        cv::Mat edgeYImage(imgSize, type);

        int rows = img->rows;
        int cols = img->cols;
        for (int i = 0; i < rows - 1; i++)
            for (int j = 0; j < cols - 1; j++)
            {
                // 计算垂直边边缘
                edgeXImage.at<uchar>(i, j) = abs(img->at<uchar>(i + 1, j) - img->at<uchar>(i, j));
                // 计算水平边缘
                edgeYImage.at<uchar>(i, j) = abs(img->at<uchar>(i, j + 1) - img->at<uchar>(i, j));
            }

        cv::addWeighted(edgeXImage, 0.5, edgeYImage, 0.5, 0.0, *img);

        return true;
    });
}

void __fastcall ImageDoc::roberts()
{
    process([](cv::Mat* img, std::vector<void*> args)
    {
        if (1 < img->channels())
            cv::cvtColor(*img, *img, cv::COLOR_BGR2GRAY);

        cv::Mat dstImage = img->clone();
        int rows = dstImage.rows;
        int cols = dstImage.cols;
        for (int i = 0; i < rows - 1; i++)
            for (int j = 0; j < cols - 1; j++)
                dstImage.at<uchar>(i, j) = (uchar)sqrt((img->at<uchar>(i, j) - img->at<uchar>(i + 1, j + 1)) * (img->at<uchar>(i, j) - img->at<uchar>(i + 1, j + 1))
                                                        + (img->at<uchar>(i + 1, j) - img->at<uchar>(i, j + 1)) * (img->at<uchar>(i + 1, j) - img->at<uchar>(i, j + 1)));
        *img = dstImage.clone();
        return true;
    });
}

void __fastcall ImageDoc::prewitt(int ddepth, double delta, int borderType)
{
    process([](cv::Mat* img, std::vector<void*> args)
    {
        if (1 < img->channels())
            cv::cvtColor(*img, *img, cv::COLOR_BGR2GRAY);

        //水平方向
        cv::Mat horizontal = (cv::Mat_<float>(3, 3) << -1, -1, -1, 0, 0, 0, 1, 1, 1);
        //垂直方向
        cv::Mat vertical = (cv::Mat_<float>(3, 3) << -1, 0, 1, -1, 0, 1, -1, 0, 1);
        //对角135°
        cv::Mat diagonal1 = (cv::Mat_<float>(3, 3) << 0, 1, 1, -1, 0, 1, -1, -1, 0);
        //对角45°
        cv::Mat diagonal2 = (cv::Mat_<float>(3, 3) << -1, -1, 0, -1, 0, 1, 0, 1, 1);

        //逆时针反转180°得到卷积核
        cv::flip(horizontal, horizontal, -1);
        cv::flip(vertical, vertical, -1);
        cv::flip(diagonal1, diagonal1, -1);
        cv::flip(diagonal2, diagonal2, -1);

        int ddepth = *(int*)args[0];
        double delta = *(double*)args[1];
        int borderType = *(int*)args[2];

        cv::Mat dst1, dst2, dst3, dst4, dst;

        //卷积得到水平方向边缘
        cv::filter2D(*img, dst1, ddepth, horizontal, cv::Point(-1, -1), delta, borderType);
        //卷积得到4垂直方向边缘
        cv::filter2D(*img, dst2, ddepth, vertical, cv::Point(-1, -1), delta, borderType);
        //卷积得到45°方向边缘
        cv::filter2D(*img, dst3, ddepth, diagonal1, cv::Point(-1, -1), delta, borderType);
        //卷积得到135°方向边缘
        cv::filter2D(*img, dst4, ddepth, diagonal2, cv::Point(-1, -1), delta, borderType);

        //边缘强度（近似）
        cv::convertScaleAbs(dst1, dst1); //求绝对值并转为无符号8位图
        cv::convertScaleAbs(dst2, dst2);
        cv::convertScaleAbs(dst3, dst3);
        cv::convertScaleAbs(dst4, dst4);
        dst = dst1 + dst2 + dst3 + dst4;

        *img = dst.clone();
        return true;
    }, 3, &ddepth, &delta, &borderType);
}

// //x，y方向联合实现获取高斯模板
// void ImageDoc::generateGaussMask(cv::Mat& Mask, cv::Size wsize, double sigma)
// {
//     Mask.create(wsize, CV_64F);
//     int h = wsize.height;
//     int w = wsize.width;
//     int center_h = (h - 1) / 2;
//     int center_w = (w - 1) / 2;
//     double sum = 0.0;
//     double x, y;
//     for (int i = 0; i < h; ++i) {
//         y = pow(i - center_h, 2);
//         for (int j = 0; j < w; ++j) {
//             x = pow(j - center_w, 2);
//             //因为最后都要归一化的，常数部分可以不计算，也减少了运算量
//             double g = exp(-(x + y) / (2 * sigma*sigma));
//             Mask.at<double>(i, j) = g;
//             sum += g;
//         }
//     }
//     Mask = Mask / sum;
// }

// void ImageDoc::gaussianFilter(cv::Mat& src, cv::Mat& dst, cv::Mat window)
// {
//     int hh = (window.rows - 1) / 2;
//     int hw = (window.cols - 1) / 2;
//     dst = cv::Mat::zeros(src.size(), src.type());
//     //边界填充
//     cv::Mat newSrc;
//     cv::copyMakeBorder(src, newSrc, hh, hh, hw, hw, cv::BORDER_REPLICATE);//边界复制

//     //高斯滤波
//     for (int i = hh; i < src.rows + hh; ++i)
//         for (int j = hw; j < src.cols + hw; ++j)
//         {
//             double sum[3] = { 0 };

//             for (int r = -hh; r <= hh; ++r)
//                 for (int c = -hw; c <= hw; ++c)
//                     if (src.channels() == 1)
//                         sum[0] = sum[0] + newSrc.at<uchar>(i + r, j + c) * window.at<double>(r + hh, c + hw);
//                     else if (src.channels() == 3) {
//                         cv::Vec3b rgb = newSrc.at<cv::Vec3b>(i + r, j + c);
//                         sum[0] = sum[0] + rgb[0] * window.at<double>(r + hh, c + hw);//B
//                         sum[1] = sum[1] + rgb[1] * window.at<double>(r + hh, c + hw);//G
//                         sum[2] = sum[2] + rgb[2] * window.at<double>(r + hh, c + hw);//R
//                     }

//             for (int k = 0; k < src.channels(); ++k)
//                 if (sum[k] < 0)
//                     sum[k] = 0;
//                 else if (sum[k] > 255)
//                     sum[k] = 255;

//             if (src.channels() == 1)
//                 dst.at<uchar>(i - hh, j - hw) = static_cast<uchar>(sum[0]);
//             else if (src.channels() == 3)
//             {
//                 cv::Vec3b rgb = { static_cast<uchar>(sum[0]), static_cast<uchar>(sum[1]), static_cast<uchar>(sum[2]) };
//                 dst.at<cv::Vec3b>(i - hh, j - hw) = rgb;
//             }
//         }
// }

void __fastcall ImageDoc::loG(cv::Size wsize, double sigma, double k)
{
    process([](cv::Mat* img, std::vector<void*> args)
    {
        if (1 < img->channels())
            cv::cvtColor(*img, *img, cv::COLOR_BGR2GRAY);

        cv::Size wsize = *(cv::Size*)args[0];
        double sigma = *(double*)args[1];
        cv::Mat gaussianDst1, gaussianDst2;

        //高斯滤波
        cv::GaussianBlur(*img, gaussianDst1, wsize, *(double*)args[2] * sigma);
        cv::GaussianBlur(*img, gaussianDst2, wsize, sigma);

        *img = gaussianDst1 - gaussianDst2;
        cv::threshold(*img, *img, 0, 255, cv::THRESH_BINARY);

        return true;
    }, 3, &wsize, &sigma, &k);
}

void __fastcall ImageDoc::blur(const cv::Size ksize, const cv::Point anchor, const int borderType)
{
    process([](cv::Mat* img, std::vector<void*> args)
    {
        cv::blur(*img, *img, *(cv::Size*)args[0], *(cv::Point*)args[1], *(int*)args[2]);
        return true;
    }, 3, &ksize, &anchor, &borderType);
}

void __fastcall ImageDoc::gaussianBlur(const cv::Size ksize, const double sigmaX, const double sigmaY, const int borderType)
{
    process([](cv::Mat* img, std::vector<void*> args)
    {
        cv::GaussianBlur(*img, *img, *(cv::Size*)args[0], *(double*)args[1], *(double*)args[2], *(int*)args[3]);
        return true;
    }, 4, &ksize, &sigmaX, &sigmaY, &borderType);
}

void __fastcall ImageDoc::medianBlur(const int ksize)
{
    process([](cv::Mat* img, std::vector<void*> args)
    {
        cv::medianBlur(*img, *img, *(int*)args[0]);
        return true;
    }, 1, &ksize);
}

void __fastcall ImageDoc::bilateralFilter(const int d, const double sigmaColor, const double sigmaSpace, const int borderType)
{
    process([](cv::Mat* img, std::vector<void*> args)
    {
        cv::Mat dst;
        cv::bilateralFilter(*img, dst, *(int*)args[0], *(double*)args[1], *(double*)args[2], *(int*)args[3]);
        *img = dst.clone();
        return true;
    }, 4, &d, &sigmaColor, &sigmaSpace, &borderType);
}

void __fastcall ImageDoc::convertCartoon()
{
    process([](cv::Mat* img, std::vector<void*>)
    {
        cv::Mat mask;
        img->copyTo(mask);
        if (1 < img->channels())
            cv::cvtColor(mask, mask, cv::COLOR_BGR2GRAY);

        cv::medianBlur(mask, mask, 7);
        cv::Laplacian(mask, mask, CV_8U, 5);
        cv::threshold(mask, mask, 80, 255, cv::THRESH_BINARY_INV);

        cv::Size smallSize, size = img->size();
        smallSize.width = size.width / 2;
        smallSize.height = size.height / 2;

        cv::resize(*img, *img, smallSize, 0, 0, cv::INTER_LINEAR);
        cv::Mat tmp = cv::Mat(smallSize, CV_8UC3);
        for (auto i = 0; i < 7; i++)
        {
            cv::bilateralFilter(*img, tmp, 9, 9, 7);
            cv::bilateralFilter(tmp, *img, 9, 9, 7);
        }

        cv::Mat bigImg;
        cv::resize(*img, bigImg, size, 0, 0, cv::INTER_LINEAR);
        img->setTo(0);
        bigImg.copyTo(*img, mask);

        return true;
    });
}
