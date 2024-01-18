#include "headers/io.h"
#include "headers/userMenu.h"

#define filePath "filePath"

int main(void){
  io_hander io = *new io_hander(filePath);
  userMenu menu = *new userMenu(io);
  menu.menu();
  return 0;
}
