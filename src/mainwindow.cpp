#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "doc_form.h"
#include <opencv2/opencv.hpp>
#include <QFileDialog>
#include <QMdiSubWindow>
#include <QDirIterator>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->mdiArea);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void __fastcall MainWindow::showImageForm(ImageDoc* doc) const
{
    DocForm* form = new DocForm();
    form->setImageDoc(doc);
    form->setWindowTitle(doc->path().c_str());
    QMdiSubWindow* subWindow = ui->mdiArea->addSubWindow(form);
    QSize formSize = form->size();
    subWindow->resize(formSize.width() + 18, formSize.height() + 40);
    subWindow->show();
}

ImageDoc* __fastcall MainWindow::getDoc() const
{
    QMdiSubWindow* subWindow = ui->mdiArea->activeSubWindow();
    if (Q_NULLPTR == subWindow)
        return nullptr;

    DocForm* form = dynamic_cast<DocForm*>(subWindow->widget());
    if (nullptr == form)
        return nullptr;
    return form->imageDoc();
}

CaptureForm* __fastcall MainWindow::getCapture() const
{
    QMdiSubWindow* subWindow = ui->mdiArea->activeSubWindow();
    if (Q_NULLPTR == subWindow)
        return Q_NULLPTR;

    return dynamic_cast<CaptureForm*>(subWindow->widget());
}
// void callbackButton(int state, void* userdata)
// {

// }

void MainWindow::on_actionOpen_triggered()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open"), "", "PNG (*.png);;JPEG (*.jpg;*.jpe;*.jfif);;TIFF (*.tif;*.tiff);;BMP (*.bmp;*.dib);;All files (*.*)");
    if (path.isEmpty())
        return;

    ImageDoc* doc = new ImageDoc();
    if (doc->read(path.toStdString()))
        showImageForm(doc);
    else
        delete doc;

    // QFileDialog fileDialog(this, tr("Open"), "", "PNG (*.png);;JPEG (*.jpg;*.jpe;*.jfif);;TIFF (*.tif;*.tiff);;BMP (*.bmp;*.dib);;All files (*.*)");
    // if (fileDialog.exec() == QDialog::Accepted)
    // {
    //     // cv::String name = "C:/Tools/opencv/4.9.0/lena.png";
    //     cv::String name = fileDialog.selectedFiles().first().toStdString();

    //     // cv::Mat img = cv::imread(name);
    //     ImageDoc* doc = new ImageDoc();
    //     if (doc->read(name))
    //         showImageForm(doc);
    //     else
    //         delete doc;

    //     // cv::namedWindow(name);
    //     // cv::imshow(name, img);

    //     // cv::displayStatusBar(name, "abc");
    //     // cv::createButton("ABC", callbackButton, NULL, cv::QT_PUSH_BUTTON, 0);
    //     // cv::displayOverlay(name, "abc");


    //     // // 创建新的文档窗口
    //     // QDockWidget *dockWidget = new QDockWidget("Document", this);
    //     // QTextEdit *textEdit = new QTextEdit(dockWidget);
    //     // dockWidget->setWidget(textEdit);

    //     // // 添加文档到列表中
    //     // //documents << dockWidget;

    //     // // 显示文档窗口
    //     // addDockWidget(Qt::RightDockWidgetArea, dockWidget);
    //     // dockWidget->show();

    //     // showImageForm(img);
    // }
}

void MainWindow::on_actionSave_triggered()
{
    ImageDoc* doc = getDoc();
    if (nullptr == doc)
        return;

    if (doc->path().empty())
        on_actionSaveAs_triggered();
    else
        doc->write();
}

void MainWindow::on_actionSaveAs_triggered()
{
    ImageDoc* doc = getDoc();
    if (nullptr == doc)
        return;

    QMdiSubWindow* subWindow = ui->mdiArea->activeSubWindow();
    QString path = QFileDialog::getSaveFileName(this, tr("Save"), "", "PNG (*.png);;JPEG (*.jpg;*.jpe;*.jfif);;TIFF (*.tif;*.tiff);;BMP (*.bmp;*.dib);;All files (*.*)");
    if (path.isEmpty())
        return;
    if (doc->write(path.toStdString()))
        subWindow->setWindowTitle(path);
}

void MainWindow::on_actionUndo_triggered()
{
    ImageDoc* doc = getDoc();
    if (nullptr != doc)
        doc->undo();
}

void MainWindow::on_actionRedo_triggered()
{
    ImageDoc* doc = getDoc();
    if (nullptr != doc)
        doc->redo();
}

void MainWindow::on_actionColorReduction_triggered()
{
    // QMdiSubWindow* subWindow = ui->mdiArea->activeSubWindow();
    // OpenCVForm* form = dynamic_cast<OpenCVForm*>(subWindow->widget());
    // if (NULL == form)
    //     return;
    // ImageDoc* doc = form->getDoc();
    // doc->colorReduction();
    ImageDoc* doc = getDoc();
    if (nullptr != doc)
        doc->colorReduction();

    // QFileDialog fileDialog(this, tr("Open"), "", "PNG (*.png);;JPEG (*.jpg;*.jpe;*.jfif);;TIFF (*.tif;*.tiff);;BMP (*.bmp;*.dib);;All files (*.*)");
    // if (fileDialog.exec() == QDialog::Accepted)
    // {
    //     cv::String name = fileDialog.selectedFiles().first().toStdString();

    //     ImageDoc doc;
    //     if (doc.read(name))
    //     {
    //         cv::namedWindow(name);
    //         cv::imshow(name, *doc.getImage());
    //         doc.colorReduction(128);
    //         cv::namedWindow(name + "123");
    //         cv::imshow(name + "123", *doc.getImage());
    //     }
    // }
}


void MainWindow::on_actionGBRToGray_triggered()
{
    ImageDoc* doc = getDoc();
    if (nullptr != doc)
        doc->gbrToGray();

    // QFileDialog fileDialog(this, tr("Open"), "", "PNG (*.png);;JPEG (*.jpg;*.jpe;*.jfif);;TIFF (*.tif;*.tiff);;BMP (*.bmp;*.dib);;All files (*.*)");
    // if (fileDialog.exec() == QDialog::Accepted)
    // {
    //     cv::String name = fileDialog.selectedFiles().first().toStdString();

    //     ImageDoc doc;
    //     if (doc.read(name))
    //     {
    //         cv::namedWindow(name);
    //         cv::imshow(name, *doc.getImage());
    //         doc.gbrToGray();
    //         cv::namedWindow(name + "123");
    //         cv::imshow(name + "123", *doc.getImage());
    //     }
    // }
}

void MainWindow::on_actionGBRToHSV_triggered()
{
    ImageDoc* doc = getDoc();
    if (nullptr != doc)
        doc->gbrToHsv();
}

void MainWindow::on_actionMonitor_triggered()
{
    // cv::VideoCapture cap;
    // cap.open(0);
    // if (!cap.isOpened())
    //     return;

    // cv::Mat mat;
    // cv::namedWindow("VideoCapture");
    // for(;;)
    // {
    //     cap >> mat;
    //     cv::imshow("VideoCapture", mat);
    //     if (27 == cv::waitKey(33))
    //         break;
    // }
    // cv::destroyWindow("VideoCapture");

    QList<QMdiSubWindow *> subWindowList = ui->mdiArea->subWindowList();
    for (QMdiSubWindow* subWindow : subWindowList)
        if (tr("Monitor") == subWindow->windowTitle())
        {
            subWindow->activateWindow();
            return;
        }

    CaptureForm* form = new CaptureForm();
    form->setWindowTitle(tr("Monitor"));
    QMdiSubWindow* subWindow = ui->mdiArea->addSubWindow(form);
    QSize formSize = form->size();
    subWindow->resize(formSize.width() + 18, formSize.height() + 40);
    form->start();
    subWindow->show();
}

void MainWindow::on_actionPhotograph_triggered()
{
    CaptureForm* form = getCapture();
    if (nullptr == form)
        return;

    showImageForm(ImageDoc::create(form->captureImage().clone()));
}

void MainWindow::on_actionPreprocess_triggered()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setWindowTitle(tr("选择文件夹"));
    if (dialog.exec() != QDialog::Accepted)
        return;

    QString path = dialog.selectedFiles()[0];
    QDir dir(path);
    if (!dir.exists())
    {
        qWarning() << "Directory does not exist.";
        return;
    }

    dir.mkdir(path+ "/out");

    cv::CascadeClassifier faceCascade;
    faceCascade.load("haarcascade_frontalface_default.xml");


    QDirIterator di(path, QDir::Files);
    while (di.hasNext())
    {
        QFileInfo fileInfo = di.nextFileInfo();

        std::vector<cv::Rect> faces;
        cv::Mat img = cv::imread((fileInfo.absolutePath() + "/" + fileInfo.fileName()).toStdString());
        cv::Mat imgGray;
        cv::cvtColor(img, imgGray, cv::COLOR_BGR2GRAY);
        cv::equalizeHist(imgGray, imgGray);

        faceCascade.detectMultiScale(imgGray, faces);
        for (cv::Rect face : faces)
        {
            cv::Mat faceROI = imgGray(face);
            cv::Mat faceImg;
            if (faceROI.cols > 100)
            {
                cv::resize(faceROI, faceImg, cv::Size(92, 112));
                imwrite((fileInfo.absolutePath() + "/out/" + fileInfo.fileName()).toStdString(), faceImg);
            }
        }
    }

    QMessageBox::information(this, tr("information"), tr("The processing succeeded."));
}

