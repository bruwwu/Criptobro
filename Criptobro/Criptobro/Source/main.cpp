#include "Prerequisites.h"
#include "ASCII.h"

int main() {
  AsciiBinary AB;

  std::string input = "profe vamonos a marmotear";
  std::string binary = AB.stringToBinary(input);
  std::cout << "Texto a binario: " << binary << std::endl;
  std::string message = AB.binaryToString(binary);
  std::cout << "Binario a texto: " << message << std::endl;

  return 0;
}

