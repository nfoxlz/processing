#ifndef DOCFORM_H
#define DOCFORM_H

#include "opencv_form.h"
#include "image_doc.h"

class DocForm : public OpenCVForm
{
    Q_OBJECT
    Q_PROPERTY(ImageDoc* imageDoc READ imageDoc WRITE setImageDoc)

public:
    DocForm();
    ~DocForm();

    inline ImageDoc* __fastcall imageDoc() const
    {
        return doc;
    }

    inline void __fastcall setImageDoc(ImageDoc* imageDoc)
    {
        doc = imageDoc;
        cv::cvtColor(*doc->image(), image, cv::COLOR_BGRA2RGB);
        resize(image.cols, image.rows);
        doc->imageProcData = this;
        doc->imageProc = DocForm::refresh;
    }

private:
    ImageDoc* doc = NULL;

    inline static void __fastcall refresh(const void* userdata)
    {
        DocForm* form = (DocForm*)userdata;
        cv::cvtColor(*form->doc->image(), form->image, cv::COLOR_BGRA2RGB);
        form->update();
    }
};

#endif // DOCFORM_H
