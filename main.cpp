#include <QApplication>
#include "video.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  Video video;
  video.show();
  return QApplication::exec();
}
