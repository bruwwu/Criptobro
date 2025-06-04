#pragma once
#include "Prerequisites.h"

class XOREncoder {
public:
  XOREncoder() = default;
  ~XOREncoder() = default;

  // Método para cifrar el texto usando XOR con una clave proporcionada
  // Input: La cadena que se quiere cifrar. -> "Hola Mundo"
  // Key: La clave a utilizar para el cifrado. -> "clave"
  string encode(const string& input, const string& key) {
    string output = input;
    for (int i = 0; i < input.size(); i++) {
      output[i] = input[i] ^ key[i % key.size()];  // Aplica el XOR entre el texto y la clave
    }
    return output;
  }

  // Convierte un texto hexadecimal a su representación en bytes
  vector<unsigned char> HexToBytes(const string& input) {
    vector<unsigned char> bytes;
    istringstream iss(input);
    string hexValue;

    while (iss >> hexValue) {
      if (hexValue.size() == 1) {
        hexValue = "0" + hexValue;  // Asegura que el valor hexadecimal tenga dos dígitos
      }
      unsigned int byte;
      stringstream ss;
      ss << hex << hexValue;
      ss >> byte;
      bytes.push_back(static_cast<unsigned char>(byte));  // Convierte a unsigned char y lo guarda
    }
    return bytes;
  }

  // Imprime el texto en formato hexadecimal
  void printHex(const string& input) {
    for (unsigned char c : input) {
      cout << hex << setw(2) << setfill('0') << (int)c << " ";  // Muestra cada byte en formato hexadecimal
    }
  }

  // Verifica si el texto contiene solo caracteres imprimibles o espacios
  bool isValidText(const string& data) {
    return all_of(data.begin(), data.end(), [](unsigned char c) {
      return isprint(c) || isspace(c) || c == '\n' || c == ' ';
      });
  }

  // Fuerza de ataque de 1 byte con XOR
  void bruteForce_1Byte(const vector<unsigned char>& cifrado) {
    for (int clave = 0; clave < 256; ++clave) {
      string result;

      for (unsigned char c : cifrado) {
        result += static_cast<unsigned char>(c ^ clave);  // Realiza el XOR con cada byte
      }

      if (isValidText(result)) {
        cout << "=============================\n";
        cout << "Clave 1 byte  : '" << static_cast<char>(clave)
          << "' (0x" << hex << setw(2) << setfill('0') << clave << ")\n";
        cout << "Texto posible : " << result << "\n";
      }
    }
  }

  // Fuerza de ataque de 2 bytes con XOR
  void bruteForce_2Byte(const vector<unsigned char>& cifrado) {
    for (int b1 = 0; b1 < 256; ++b1) {
      for (int b2 = 0; b2 < 256; ++b2) {
        string result;
        unsigned char key[2] = { static_cast<unsigned char>(b1), static_cast<unsigned char>(b2) };

        for (int i = 0; i < cifrado.size(); i++) {
          result += cifrado[i] ^ key[i % 2];  // Realiza el XOR con cada byte usando los dos bytes de clave
        }

        if (isValidText(result)) {
          cout << "=============================\n";
          cout << "Clave 2 bytes : '" << static_cast<char>(b1) << static_cast<char>(b2)
            << "' (0x" << hex << setw(2) << setfill('0') << b1
            << " 0x" << setw(2) << setfill('0') << b2 << ")\n";
          cout << "Texto posible : " << result << "\n";
        }
      }
    }
  }

  // Fuerza de ataque utilizando un diccionario de claves comunes
  void bruteForceByDictionary(const vector<unsigned char>& cifrado) {
    vector<string> clavesComunes = {
        "clave", "admin", "1234", "root", "test", "abc", "hola", "user",
        "pass", "12345", "0000", "password", "default"
    };

    for (const auto& clave : clavesComunes) {
      string result;
      for (int i = 0; i < cifrado.size(); i++) {
        result += static_cast<unsigned char>(cifrado[i] ^ clave[i % clave.size()]);  // XOR con cada palabra del diccionario
      }
      if (isValidText(result)) {
        cout << "=============================\n";
        cout << "Clave de diccionario: '" << clave << "'\n";
        cout << "Texto posible : " << result << "\n";
      }
    }
  }
private:
  // Aquí podrían ir otras funciones privadas si fueran necesarias
};
