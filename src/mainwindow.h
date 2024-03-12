#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "image_doc.h"
#include "capture_form.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionExit_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionUndo_triggered();
    void on_actionRedo_triggered();
    void on_actionColorReduction_triggered();
    void on_actionGBRToGray_triggered();
    void on_actionGBRToHSV_triggered();
    void on_actionSaveAs_triggered();
    void on_actionPhotograph_triggered();
    void on_actionMonitor_triggered();
    void on_actionPreprocess_triggered();
    void on_actionMakeBorder_triggered();
    void on_actionCartoon_triggered();
    void on_actionFindContours_triggered();

    void on_actionAverage_triggered();

    void on_actionGaussian_triggered();

    void on_actionMedian_triggered();

    void on_actionBilateral_triggered();

    void on_actionSobel_triggered();

    void on_actionScharr_triggered();

    void on_actionLaplacian_triggered();

    void on_actionCanny_triggered();

    void on_actionDifference_triggered();

    void on_actionRoberts_triggered();

    void on_actionPrewitt_triggered();

    void on_actionLoG_triggered();

    void on_actionLinearTransform_triggered();

private:
    Ui::MainWindow *ui;

    void __fastcall showImageForm(ImageDoc*) const;
    ImageDoc* __fastcall getDoc() const;
    CaptureForm* __fastcall getCapture() const;
};
#endif // MAINWINDOW_H
