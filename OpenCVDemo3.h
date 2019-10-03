#ifndef OPENCVDEMO3_H
#define OPENCVDEMO3_H

#include <QtWidgets/QWidget>
#include "ui_OpenCVDemo3.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QTimer>
#include <QImage>
using namespace cv;
class OpenCVDemo3 : public QWidget
{
	Q_OBJECT

public:
	OpenCVDemo3(QWidget *parent = 0);
	~OpenCVDemo3();
	QImage  MatToQImage(const cv::Mat& mat);
private slots:
	void onBtnClicked();
	void onTimeout();
	void onBtnFastClicked();
	void onBtnSlowClicked();
private:
	Ui::OpenCVDemo3Class ui;
	VideoCapture capture;
	QTimer timer;
	int beishu;
	int delay;
};

#endif // OPENCVDEMO3_H
