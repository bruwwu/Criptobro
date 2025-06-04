#pragma once

#include <string>
#include <iostream>
using namespace std;

class CesarEncryption {
public:
  CesarEncryption() = default;
  ~CesarEncryption() = default;

  std::string Encryption(const std::string& texto, int desplazamiento) {
    std::string result = "";

    for (char c : texto) {
      if (c >= 'A' && c <= 'Z') {
        result += (char)(((c - 'A' + desplazamiento) % 26) + 'A');
      }
      else if (c >= 'a' && c <= 'z') {
        result += (char)(((c - 'a' + desplazamiento) % 26) + 'a');
      }
      else if (c >= '0' && c <= '9') {
        result += (char)(((c - '0' + desplazamiento) % 10) + '0');
      }
      else {
        result += c;
      }
    }

    return result;
  }

  std::string decode(const std::string& texto, int desplazamiento) {
    return Encryption(texto, 26 - (desplazamiento % 26));
  }

  string BruteForce(const std::string& texto) {
    string result = "";
    for (int i = 0; i < 26; i++) {
      result += "Desplazamiento " + to_string(i) + ": " + decode(texto, i) + "\n";
    }
    return result;
  }


private:

};