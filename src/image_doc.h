#ifndef IMAGEDOC_H
#define IMAGEDOC_H

#include <stdarg.h>
#include <vector>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>

typedef bool (__fastcall *ImageProc)(cv::Mat*, std::vector<void*>);
typedef void (__fastcall *ImageProcAfter)(const void* userdata);

class ImageDoc
{
public:
    ImageDoc();
    virtual ~ImageDoc();

    ImageProcAfter imageProc;
    void* imageProcData;

    static ImageDoc* __fastcall create(cv::Mat);

    static ImageDoc* __fastcall readDoc(const cv::String);

    inline cv::Mat* __fastcall image() const
    {
        return imageMat;
    }

    inline cv::String __fastcall path() const
    {
        return imagePath;
    }

    inline bool __fastcall undo()
    {
        if (0 >= index)
            return false;

        index--;
        imageMat = history[index];

        imageProc(imageProcData);

        return true;
    }

    inline bool __fastcall redo()
    {
        if (history.size() <= index + 1)
            return false;

        index++;
        imageMat = history[index];

        imageProc(imageProcData);

        return true;
    }

    bool __fastcall read(const cv::String path);
    bool __fastcall write();
    bool __fastcall write(const cv::String path);
    void __fastcall colorReduction(const uchar div = 64);
    void __fastcall gbrToGray();
    void __fastcall gbrToHsv();
    void __fastcall linearTransform(const double a, const int b);
    void __fastcall makeBorder(const int top, const int bottom, const int left, const int right, const int borderType, const cv::Scalar & value = cv::Scalar());
    void __fastcall findContours();
    void __fastcall sobel(const int ddepth, const int dx, const int dy, const int ksize = 3, const double scale = 1, const double delta = 0, const int borderType = cv::BORDER_DEFAULT);
    void __fastcall scharr(const int ddepth, const int dx, const int dy, const double scale = 1, const double delta = 0, const int borderType = cv::BORDER_DEFAULT);
    void __fastcall laplacian(const int ddepth, const int ksize = 1, const double scale = 1, const double delta = 0, const int borderType = cv::BORDER_DEFAULT);
    void __fastcall canny(const double threshold1, const double threshold2, const int apertureSize = 3, const bool l2gradient = false);
    void __fastcall difference();
    void __fastcall roberts();
    void __fastcall prewitt(int ddepth, double delta = 0, int borderType = cv::BORDER_DEFAULT);
    void __fastcall loG(cv::Size wsize, double sigma, double k = 1.6);

    void __fastcall blur(const cv::Size ksize, const cv::Point anchor = cv::Point(-1,-1), const int borderType = cv::BORDER_DEFAULT);
    void __fastcall medianBlur(const int ksize);
    void __fastcall gaussianBlur(const cv::Size ksize, const double sigmaX, const double sigmaY = 0, const int borderType = cv::BORDER_DEFAULT);
    void __fastcall bilateralFilter(const int d, const double sigmaColor, const double sigmaSpace, const int borderType = cv::BORDER_DEFAULT);

    void __fastcall convertCartoon();

private:
    std::vector<cv::Mat*> history;
    std::vector<cv::Mat*>::size_type index = -1;
    cv::Mat* imageMat;
    cv::String imagePath;

    inline void process(ImageProc proc, const int cnt = 0, ...)
    {
        cv::Mat* img = new cv::Mat();
        imageMat->copyTo(*img);

        std::vector<void*> args(cnt);
        va_list ap;
        va_start(ap, cnt);
        for (int i = 0; i < cnt; i++)
            args[i] = va_arg(ap, void*);
        va_end(ap);

        if (proc(img, args))
        {
            for (auto i = history.size() - 1; i > index; i--)
            {
                delete history[i];
                history.pop_back();
            }
            imageMat = img;
            history.push_back(img);
            index++;

            imageProc(imageProcData);
        }
        else
            delete img;
    }

    inline void __fastcall clearHistory()
    {
        for (auto item : history)
            delete item;
        history.clear();
        index = -1;
    }

    // //x，y方向联合实现获取高斯模板
    // void generateGaussMask(cv::Mat& Mask, cv::Size wsize, double sigma);
    // //按二维高斯函数实现高斯滤波
    // void gaussianFilter(cv::Mat& src, cv::Mat& dst, cv::Mat window);
};

#endif // IMAGEDOC_H
