#include "headers/userMenu.h"
#include "headers/io.h"
#include <iostream>
#include <string>

#define addUser 1
#define login 2
#define quit 3

void userMenu::menu(void) {
  std::string input;
  size_t index;
  int result;
  while (1) {
    std::cout << "1. Add User\n2. Login\n3. Exit\n";
    std::getline(std::cin, input);
    index = input.find_first_of("0123456789");
    if (index != std::string::npos) {
      result = stoi(input.substr(index, 1));
    } else {
      result = 0;
    }

    switch (result) {
    case addUser:
      this->createUser();
      break;
    case login:
      this->attemptLogin();
      break;
    case quit:
      return;
      break;
    default:
      std::cout << "Invalid input\n";
      break;
    }
  }
  return;
}

bool userMenu::attemptLogin(void) {
  std::cout << "LOGGING IN\n";
  return 1;
}

bool userMenu::createUser(void) {
  std::cout << "CREATING USER\n";
  return 1;
}

userMenu::~userMenu(void) { return; }
