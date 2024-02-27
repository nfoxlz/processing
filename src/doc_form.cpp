#include "doc_form.h"

DocForm::DocForm() {}

DocForm::~DocForm()
{
    if (NULL != doc)
        delete doc;
}
