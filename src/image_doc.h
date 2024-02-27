#ifndef IMAGEDOC_H
#define IMAGEDOC_H

#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>

typedef bool (__fastcall *ImageProc)(cv::Mat* img, const void* userdata);
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

    bool __fastcall read(const cv::String);
    bool __fastcall write();
    bool __fastcall write(const cv::String);
    void __fastcall colorReduction(const uchar div = 64);
    void __fastcall gbrToGray();
    void __fastcall gbrToHsv();

private:
    std::vector<cv::Mat*> history;
    std::vector<cv::Mat*>::size_type index = -1;
    cv::Mat* imageMat;
    cv::String imagePath;

    inline void process(ImageProc proc, const void* userdata = NULL)
    {
        cv::Mat* img = new cv::Mat();
        imageMat->copyTo(*img);

        if (proc(img, userdata))
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
};

#endif // IMAGEDOC_H
