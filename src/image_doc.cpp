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
    process([](cv::Mat* img, const void* userdata)
    {
        const uchar div = *(const uchar*)userdata;
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
    }, &div);
}

void __fastcall ImageDoc::gbrToGray()
{
    process([](cv::Mat* img, const void*)
    {
        if (3 > img->channels())
            return false;

        cv::cvtColor(*img, *img, cv::COLOR_BGRA2GRAY);
        return true;
    });
}

void __fastcall ImageDoc::gbrToHsv()
{
    process([](cv::Mat* img, const void*)
    {
        if (3 > img->channels())
            cv::cvtColor(*img, *img, cv::COLOR_GRAY2BGR);

        cv::cvtColor(*img, *img, cv::COLOR_BGR2HSV);
        return true;
    });
}
