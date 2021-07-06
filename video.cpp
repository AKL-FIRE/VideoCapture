//
// Created by changyuli on 7/4/21.
//

// You may need to build the project (run Qt uic code generator) to get "ui_Video.h" resolved


#include "video.h"
#include "ui_video.h"

Video::Video(QWidget *parent) :
	QWidget(parent), ui(new Ui::Video) {
  ui->setupUi(this);
  this->setWindowTitle("视频截取器");
}

Video::~Video() {
  delete ui;
}

void Video::on_openBtn_clicked() {
  QString deftPath = "",
	  deftSuffic = "Videos(*.avi *.mp4 *.wmv *.mkv *.rmvb *.mpeg *.m2v);;Alls(*.*)";
  auto mVideoPath = QFileDialog::getOpenFileName(this, "Open-Video", deftPath, deftSuffic);
  if (mVideoPath.isEmpty())  return;

  ui->pathLabel->setText(mVideoPath);

  vc = std::make_unique<cv::VideoCapture>();
  cStr = mVideoPath.toStdString();
  qDebug() << "Video: " << mVideoPath;
  vc->open(cStr);

  QString str;
  if (vc->isOpened())
  {
	str = QString::fromLocal8Bit("视频读取成功！");
	qDebug() << str;
  }
  else
  {
	str = QString::fromLocal8Bit("视频读取失败！");
	qDebug() << str;
	return;
  }

  double Ratio = vc->get(cv::CAP_PROP_FPS),//帧率
  wid = vc->get(cv::CAP_PROP_FRAME_WIDTH),//每帧图像宽度
  heit = vc->get(cv::CAP_PROP_FRAME_HEIGHT);//每帧图像高度

  fps_ = Ratio;
  width_ = wid;
  height_ = heit;

  auto mFramesCnt = vc->get(cv::CAP_PROP_FRAME_COUNT);//帧数
  qDebug() << "Ratio: " << Ratio << "Width: " << wid << "Height: " << heit << "count: " << mFramesCnt;
//  ui->videoLabel->setMinimumWidth(wid);
//  ui->videoLabel->setMinimumHeight(heit);
}

void Video::getFrame(int frames) {
  vc->open(cStr);
  auto mFramesCnt = vc->get(cv::CAP_PROP_FRAME_COUNT);
  if (mFramesCnt <= 0) {
	for (int i = 0; i < frames; ++i) {
	  vc->read(frame);
	}
  } else {
    vc->set(cv::CAP_PROP_POS_FRAMES, frames);
    vc->read(frame);
  }
}

void Video::on_startFrameBtn_clicked() {
  auto frame_index = ui->startFrame->value();
  getFrame(frame_index);
  showFrame(ui->startFrameLabel);
}

void Video::showFrame(QLabel* label) {
  if (frame.empty())
	return;
  QImage srcQImage = QImage((uchar*)(frame.data), frame.cols, frame.rows, QImage::Format_RGB888);
  srcQImage = srcQImage.scaled(frameLabelWidth_, frameLabelHeight_);
  label->setPixmap(QPixmap::fromImage(srcQImage));
  label->resize(srcQImage.size());
  label->show();
}

void Video::on_endFrameBtn_clicked() {
  auto frame_index = ui->endFrame->value();
  getFrame(frame_index);
  showFrame(ui->endFrameLabel);
}

void Video::on_saveBtn_clicked() {
  auto file_path = ui->savePath->text();
  if (file_path.isEmpty())
	return;
  auto start = ui->startFrame->value();
  auto end = ui->endFrame->value();
  if (end <= start || start < 0 || end < 0) {
    qDebug() << "开始与结束帧不合法";
	return;
  }
  auto video_writer = cv::VideoWriter(file_path.toStdString(),
									  cv::VideoWriter::fourcc('m', 'p', '4', 'v'),
									  fps_,
									  cv::Size(width_, height_));
  getFrame(start);
  auto current = start;
  double progress = 0;
  do {
    video_writer.write(frame);
    vc->read(frame);
    progress = double(current - start) / (end - start);
	changeProgressBar(progress * 100);
  } while (++current <= end);
}

void Video::spinValueChanged(int value, QLabel *label) {
  getFrame(value);
  showFrame(label);
}

void Video::on_startFrame_valueChanged(int value) {
  spinValueChanged(value, ui->startFrameLabel);
}

void Video::on_endFrame_valueChanged(int value) {
  spinValueChanged(value, ui->endFrameLabel);
}

void Video::changeProgressBar(int value) {
  ui->progressBar->setValue(value);
}

void Video::saveFrameToFile(QLabel *label) {
  if (ui->savePath->text().isEmpty()) {
    qDebug() << "保存路径不能为空";
	return;
  }
  auto save_path = ui->savePath->text();
  auto pic = label->pixmap();
  pic->save(save_path);
}

void Video::on_saveStartFrame_clicked() {
  saveFrameToFile(ui->startFrameLabel);
}

void Video::on_saveEndFrame_clicked() {
  saveFrameToFile(ui->endFrameLabel);
}




