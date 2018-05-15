#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QDebug>


#define CAM_ID 0

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->comboBox->addItem("Select Camera");

    //Obtaining a list of available cameras on the system
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    foreach (const QCameraInfo &cameraInfo, cameras)
        ui->comboBox->addItem(cameraInfo.deviceName());

    //Frames Per Second
    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this,  SLOT(updateView()));
    timer->start(16);   // The view updates every 16ms --> 1000/16 = ~60FPS

    ui->camViewer->setScene(&scene);
}

MainWindow::~MainWindow()
{
    delete ui;
    if(video.isOpened())
    {
         video.release();
    }
}

void MainWindow::on_pushButton_clicked()
{
    if(video.isOpened()) {

         video.release();
         ui->pushButton->setText("Start");      //Change the push button text to start when there is no stream
    }

    else
    {
        QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
        for ( int cam_id = 0; cam_id < cameras.count(); cam_id++)

        {
            if( ui->comboBox->currentText() == cameras[cam_id].deviceName()) {

                    video.open(cam_id);
                    ui->pushButton->setText("Stop");
            }
        }

    }
}




void MainWindow::updateView()
{
    if(!video.isOpened()) return;
    cv::Mat frame;
    while(1)
    {
        video >> frame;
        if(!frame.empty()) break;
    }
    if(frame.empty()) return;
    ui->camViewer->setImage(QImage((const unsigned char*)(frame.data), frame.cols,frame.rows,QImage::Format_RGB888).rgbSwapped());


}


