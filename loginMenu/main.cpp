#include "loginMenu/userMenu.h"
#include "srcShared/io_file.h"
#include "srcShared/openSSL_EVP.h"

#define filePath_usr "../../../users.txt"

int main(void) {
  io_file commonPW(filePath_usr);
  EVP_Hash hash(MD5_Hash);
  userMenu menu(commonPW, hash);
  menu.menu();
  return 0;
}
