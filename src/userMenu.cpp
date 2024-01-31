#include "headers/userMenu.h"

#define addUser 1
#define login 2
#define quit 3

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
  std::string userName;
  std::cout << "Enter Username: ";
  std::getline(std::cin, userName);
  std::string line = this->findUser(userName);
  if (line.size() > 0) {
    std::string hash =
        line.substr(line.find_first_of(',') + 1,
                    line.find_last_of(',') - userName.size() - 1);
    std::string salt = line.substr(line.find_last_of(',') + 1);
    std::string password;
    std::cout << "Enter password: ";
    std::getline(std::cin, password);
    if (this->verifyPW(hash, salt, password)) {
      std::cout << "Valid login\n";
      return true;
    }
    std::cout << "Invalid credentials\n";
    return false;
  }
  return false;
}

std::string userMenu::getUserLine(const std::string &outputText) {
  std::string inLine;
  std::cout << outputText;
  std::getline(std::cin, inLine);
  return inLine;
}

bool userMenu::validatePW(const std::string &password) {
  if (password.size() < 8) {
    std::cout << "Password requires at least 8 characters\n";
    return false;
  }
  if (!std::ranges::any_of(password,
                           [](const char &c) { return std::isupper(c); })) {
    std::cout << "Password requires a upper case character\n";
    return false;
  }
  if (!std::ranges::any_of(password,
                           [](const char &c) { return !std::isupper(c); })) {
    std::cout << "Password requires a lower case character\n";
    return false;
  }
  if (!std::ranges::any_of(password,
                           [](const char &c) { return std::isdigit(c); })) {
    std::cout << "Password requires a number\n";
    return false;
  }
  if (!std::ranges::any_of(password, [](const char &c) {
        if (std::isdigit(c) || std::isalpha(c)) {
          return false;
        }
        return true;
      })) {
    std::cout << "Password requires a special character\n";
    return false;
  }
  return true;
}

bool userMenu::createUser(void) {
  std::string userName;
  std::string found;
  do {
    userName = getUserLine("Enter desired Username: ");
    found = this->findUser(userName);
    //TODO: Make it "email like"
    if (found != "") {
      std::cout << std::format("User name {} is taken\n",
                               found.substr(0, found.find_first_of(',')));
    }
  } while (found != "");
  std::string password;
  do {
    password = getUserLine("Enter password: ");
  } while (!this->validatePW(password));
  // TODO: fix some actual salt
  std::string salt = "FixMeLater";
  std::string line = std::format("{},{},{}", userName,
                                 hasher.hashString(password + salt), salt);
  (void)usersFile.writeLine(line);
  return 1;
}

std::string userMenu::findUser(const std::string &userName) {
  usersFile.reset();
  while (usersFile.hasLine()) {
    std::string line = usersFile.readLine();
    std::string user = line.substr(0, line.find_first_of(','));
    if (user == userName) {
      usersFile.reset();
      return line;
    }
  }
  usersFile.reset();
  return "";
}

bool userMenu::verifyPW(const std::string &hash, const std::string &salt,
                        const std::string &password) {
  std::string strToHash = password + salt;
  if (hash == this->hasher.hashString(strToHash)) {
    return true;
  }
  return false;
}

void userMenu::findPW(void) { return; }

// void userMenu::findPW(void) {
//   while (this->io_hashed_list.hasLine()) {
//     std::string line = this->io_hashed_list.readLine();
//     std::string hashStr;
//     std::string saltStr;
//
//     hashStr = line.substr(0, line.find_first_of(','));
//     saltStr = line.substr(line.find_first_of(',') + 1, line.size());
//
//     std::cout << std::format("Hash: {} Salt: {}\n", hashStr, saltStr);
//     this->crackPW(hashStr, saltStr);
//   }
//   this->io_hashed_list.reset();
// }
//
// void userMenu::crackPW(const std::string &hashStr, const std::string
// &saltStr) {
//   while (this->io_common_PW_list.hasLine()) {
//     std::string pw = this->io_common_PW_list.readLine();
//     std::string pwSalt = pw + saltStr;
//     std::string hashedString = this->hash.hashString(pwSalt);
//     if (hashedString == hashStr) {
//       std::cout << std::format("Found match!\nHash: {}\nPW: {}\n", hashStr,
//       pw); this->io_common_PW_list.reset(); return;
//     } else {
//       std::cout << std::format("Hash: {}\nHashedStr: {}\n", hashStr,
//                                hashedString);
//     }
//   }
//   this->io_common_PW_list.reset();
// }
//
userMenu::~userMenu(void) { return; }
