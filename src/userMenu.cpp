#include "headers/userMenu.h"
#include <cstring>
#include <format>

#define addUser 1
#define login 2
#define quit 3
#define crack 4

void userMenu::menu(void) {
  std::string input;
  while (1) {
    std::cout << "1. Add User\n2. Login\n3. Exit\n";
    std::getline(std::cin, input);

    switch (this->validateInput(input)) {
    case addUser:
      this->createUser();
      break;
    case login:
      this->attemptLogin();
      break;
    case quit:
      return;
      break;
    case crack:
      this->findPW();
      break;
    default:
      std::cout << "Invalid input\n";
      break;
    }
  }
  return;
}

int userMenu::validateInput(std::string input) {
  int result = 0;
  size_t index = input.find_first_of("0123456789");
  if (index != std::string::npos) {
    result = std::stoi(input.substr(index, 1));
  }
  return result;
}

bool userMenu::attemptLogin(void) {
  std::cout << "LOGGING IN\n";
  return 1;
}

bool userMenu::createUser(void) {
  std::cout << "CREATING USER\n";
  return 1;
}

void userMenu::findPW(void) {
  while (this->io_hashed_list.hasLine()) {
    std::string line = this->io_hashed_list.readLine();
    std::string hashStr;
    std::string saltStr;

    hashStr = line.substr(0, line.find_first_of(','));
    saltStr = line.substr(line.find_first_of(',') + 1, line.size());

    std::cout << std::format("Hash: {} Salt: {}\n", hashStr, saltStr);
    this->crackPW(hashStr, saltStr);
  }
  this->io_hashed_list.reset();
}

void userMenu::crackPW(const std::string &hashStr, const std::string &saltStr) {
  while (this->io_common_PW_list.hasLine()) {
    std::string pw = this->io_common_PW_list.readLine();
    std::string pwSalt = pw + saltStr;
    std::string hashedString = this->hash.hashString(pwSalt);
    if (hashedString == hashStr) {
      std::cout << std::format("Found match!\nHash: {}\nPW: {}\n", hashStr, pw);
      this->io_common_PW_list.reset();
      return;
    } else {
      std::cout << std::format("Hash: {}\nHashedStr: {}\n", hashStr,
                               hashedString);
    }
  }
  this->io_common_PW_list.reset();
}

userMenu::~userMenu(void) { return; }
