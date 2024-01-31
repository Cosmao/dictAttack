#include "headers/io_file.h"
#include "headers/openSSL_EVP.h"
#include "headers/userMenu.h"

#define filePath_usr "../../users.txt"

int main(void) {
  io_file users_list(filePath_usr);
  EVP_Hash EVP_Hasher(MD5_Hash);
  userMenu menu(users_list, EVP_Hasher);
  menu.menu();
  return 0;
}
