#include "OpenCVDemo3.h"
#include <QDebug>
OpenCVDemo3::OpenCVDemo3(QWidget *parent)
	: QWidget(parent)
	, beishu(1)
	,delay(0)
{
	ui.setupUi(this);
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(onBtnClicked()));
	connect(ui.pushButton_fast, SIGNAL(clicked()), this, SLOT(onBtnFastClicked()));
	connect(ui.pushButton_slow, SIGNAL(clicked()), this, SLOT(onBtnSlowClicked()));
	connect(&timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
}

OpenCVDemo3::~OpenCVDemo3()
{
	capture.release();
}
void OpenCVDemo3::onBtnFastClicked()
{

	beishu = beishu * 2;
	timer.stop();
	timer.start(delay / beishu);
}
void OpenCVDemo3::onBtnSlowClicked()
{
	beishu = beishu / 2;
	timer.stop();
	timer.start(delay / beishu);
}
void OpenCVDemo3::onBtnClicked()
{

	//打开视频文件：其实就是建立一个VideoCapture结构
	std::string str = QString::fromLocal8Bit("E:\\精品视频\\E87501659DD7AD4ADB31B7070E087F18.mp4").toLocal8Bit().data();
	capture.open(str);
	//检测是否正常打开:成功打开时，isOpened返回ture
	if (!capture.isOpened())
	{
		ui.textEdit->append("fail to open!");
		return;
	}
	//获取整个帧数
	long totalFrameNumber = capture.get(CV_CAP_PROP_FRAME_COUNT);
	ui.textEdit->append(QString::fromLocal8Bit("整个视频共 %1 帧").arg(totalFrameNumber));

	ui.label->resize(QSize(capture.get(CV_CAP_PROP_FRAME_WIDTH), capture.get(CV_CAP_PROP_FRAME_HEIGHT)));
	//设置开始帧()
	long frameToStart = 0;
	capture.set(CV_CAP_PROP_POS_FRAMES, frameToStart);
	ui.textEdit->append(QString::fromLocal8Bit("从第 %1 帧开始读").arg(frameToStart));

	//获取帧率
	double rate = capture.get(CV_CAP_PROP_FPS);
	ui.textEdit->append(QString::fromLocal8Bit("帧率为:第 %1 ").arg(rate));

	delay = 1000 / rate;
	timer.start(delay / beishu);

}
void OpenCVDemo3::onTimeout()
{
	Mat frame;
	//读取下一帧
	if (!capture.read(frame))
	{
		ui.textEdit->append(QString::fromLocal8Bit("读取视频失败"));
		return;
	}

	QImage image=MatToQImage(frame);
	
	ui.label->setScaledContents(true);
	QSize qs = ui.label->rect().size();
	ui.label->setPixmap(QPixmap::fromImage(image).scaled(qs));
	
	ui.label->repaint();
	//这里加滤波程序

	long totalFrameNumber = capture.get(CV_CAP_PROP_POS_FRAMES);

	ui.textEdit->append(QString::fromLocal8Bit("正在读取第：第 %1 帧").arg(totalFrameNumber));


}
QImage OpenCVDemo3::MatToQImage(const cv::Mat& mat)
{
	// 8-bits unsigned, NO. OF CHANNELS = 1    
	if (mat.type() == CV_8UC1)
	{
		QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
		// Set the color table (used to translate colour indexes to qRgb values)    
		image.setColorCount(256);
		for (int i = 0; i < 256; i++)
		{
			image.setColor(i, qRgb(i, i, i));
		}
		// Copy input Mat    
		uchar *pSrc = mat.data;
		for (int row = 0; row < mat.rows; row++)
		{
			uchar *pDest = image.scanLine(row);
			memcpy(pDest, pSrc, mat.cols);
			pSrc += mat.step;
		}
		return image;
	}
	// 8-bits unsigned, NO. OF CHANNELS = 3    
	else if (mat.type() == CV_8UC3)
	{
		// Copy input Mat   
		const uchar *pSrc = (const uchar*)mat.data;
		// Create QImage with same dimensions as input Mat    
		QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
		return image.rgbSwapped();
	}
	else if (mat.type() == CV_8UC4)
	{
		// Copy input Mat    
		const uchar *pSrc = (const uchar*)mat.data;
		// Create QImage with same dimensions as input Mat    
		QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
		return image.copy();
	}
	else
	{
		return QImage();
	}
}
