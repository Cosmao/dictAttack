#include <iostream>
#include <qt6/QtCore/QCoreApplication>
#include <qt6/QtWidgets/QtWidgets>

int main(int argc, char *argv[]) {
  QApplication test(argc, argv);
  QWidget window;
  window.resize(320, 240);
  window.show();
  std::cout << "Is working!\n";
  return test.exec();
}
