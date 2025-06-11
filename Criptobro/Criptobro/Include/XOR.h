#pragma once
#include "Prerequisites.h"

/**
 * @class XOREncoder
 * @brief Clase para cifrado y descifrado de texto utilizando el algoritmo XOR.
 *
 * Soporta cifrado simétrico, impresión en hexadecimal, y ataques de fuerza bruta
 * de 1 byte, 2 bytes o por diccionario. Permite convertir y validar datos cifrados.
 */
class XOREncoder {
public:
  XOREncoder() = default;
  ~XOREncoder() = default;

  /**
   * @brief Cifra una cadena de texto utilizando una clave mediante operación XOR.
   *
   * @param input Texto a cifrar.
   * @param key Clave de cifrado.
   * @return Texto cifrado como string.
   */
  std::string encode(const std::string& input, const std::string& key) {
    std::string output = input;
    for (int i = 0; i < input.size(); i++) {
      output[i] = input[i] ^ key[i % key.size()];
    }
    return output;
  }

  /**
   * @brief Convierte una cadena hexadecimal a un vector de bytes.
   *
   * @param input Cadena en formato hexadecimal.
   * @return Vector de bytes representando los datos.
   */
  std::vector<unsigned char> HexToBytes(const std::string& input) {
    std::vector<unsigned char> bytes;
    std::istringstream iss(input);
    std::string hexValue;

    while (iss >> hexValue) {
      if (hexValue.size() == 1) hexValue = "0" + hexValue;

      unsigned int byte;
      std::stringstream ss;
      ss << std::hex << hexValue;
      ss >> byte;

      bytes.push_back(static_cast<unsigned char>(byte));
    }

    return bytes;
  }

  /**
   * @brief Imprime una cadena en formato hexadecimal.
   *
   * @param input Cadena de texto o datos cifrados.
   */
  void printHex(const std::string& input) {
    for (unsigned char c : input) {
      std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)c << " ";
    }
  }

  /**
   * @brief Verifica si un texto contiene solo caracteres imprimibles y espacios válidos.
   *
   * @param data Texto a validar.
   * @return `true` si todos los caracteres son válidos, `false` si hay binarios o no imprimibles.
   */
  bool isValidText(const std::string& data) {
    return std::all_of(data.begin(), data.end(), [](unsigned char c) {
      return isprint(c) || isspace(c) || c == '\n' || c == ' ';
      });
  }

  /**
   * @brief Ejecuta fuerza bruta con todas las posibles claves de 1 byte.
   *
   * @param cifrado Vector de bytes cifrados.
   */
  void bruteForce_1Byte(const std::vector<unsigned char>& cifrado) {
    for (int clave = 0; clave < 256; ++clave) {
      std::string result;
      for (unsigned char c : cifrado) {
        result += static_cast<unsigned char>(c ^ clave);
      }

      if (isValidText(result)) {
        std::cout << "=============================\n";
        std::cout << "Clave 1 byte  : '" << static_cast<char>(clave)
          << "' (0x" << std::hex << std::setw(2) << std::setfill('0') << clave << ")\n";
        std::cout << "Texto posible : " << result << "\n";
      }
    }
  }

  /**
   * @brief Ejecuta fuerza bruta probando todas las combinaciones posibles de claves de 2 bytes.
   *
   * @param cifrado Vector de bytes cifrados.
   */
  void bruteForce_2Byte(const std::vector<unsigned char>& cifrado) {
    for (int b1 = 0; b1 < 256; ++b1) {
      for (int b2 = 0; b2 < 256; ++b2) {
        std::string result;
        unsigned char key[2] = {
            static_cast<unsigned char>(b1),
            static_cast<unsigned char>(b2)
        };

        for (int i = 0; i < cifrado.size(); i++) {
          result += cifrado[i] ^ key[i % 2];
        }

        if (isValidText(result)) {
          std::cout << "=============================\n";
          std::cout << "Clave 2 bytes : '" << static_cast<char>(b1) << static_cast<char>(b2)
            << "' (0x" << std::hex << std::setw(2) << std::setfill('0') << b1
            << " 0x" << std::setw(2) << std::setfill('0') << b2 << ")\n";
          std::cout << "Texto posible : " << result << "\n";
        }
      }
    }
  }

  /**
   * @brief Realiza ataque de fuerza bruta con un diccionario de claves comunes.
   *
   * @param cifrado Vector de bytes cifrados.
   */
  void bruteForceByDictionary(const std::vector<unsigned char>& cifrado) {
    std::vector<std::string> clavesComunes = {
      "clave", "admin", "1234", "root", "test", "abc", "hola", "user",
      "pass", "12345", "0000", "password", "default"
    };

    for (const auto& clave : clavesComunes) {
      std::string result;
      for (int i = 0; i < cifrado.size(); i++) {
        result += static_cast<unsigned char>(cifrado[i] ^ clave[i % clave.size()]);
      }

      if (isValidText(result)) {
        std::cout << "=============================\n";
        std::cout << "Clave de diccionario: '" << clave << "'\n";
        std::cout << "Texto posible : " << result << "\n";
      }
    }
  }

private:
  // Espacio reservado para métodos internos adicionales si se requiere.
};
