//
// Created by changyuli on 7/4/21.
//

#ifndef VIDEO__VIDEO_H_
#define VIDEO__VIDEO_H_

#include <QWidget>
#include <QFileDialog>
#include <opencv2/videoio.hpp>
#include <memory>
#include <string>
#include <QDebug>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class Video; }
QT_END_NAMESPACE

class Video : public QWidget {
 Q_OBJECT

 public:
  explicit Video(QWidget *parent = nullptr);
  ~Video() override;


 private slots:
  void on_openBtn_clicked();
  void on_startFrameBtn_clicked();
  void on_endFrameBtn_clicked();
  void on_saveBtn_clicked();
  void on_startFrame_valueChanged(int value);
  void on_endFrame_valueChanged(int value);
  void on_saveStartFrame_clicked();
  void on_saveEndFrame_clicked();


 private:
  void getFrame(int frames);
  void showFrame(QLabel* label);
  void spinValueChanged(int value, QLabel* label);
  void changeProgressBar(int value);
  void saveFrameToFile(QLabel* label);

  Ui::Video *ui;
  std::unique_ptr<cv::VideoCapture> vc;
  cv::Mat frame;
  std::string cStr;
  double fps_;
  double width_;
  double height_;

  const static int frameLabelHeight_ {512};
  const static int frameLabelWidth_ {640};
};

#endif //VIDEO__VIDEO_H_
