#pragma once

#include <string>
#include <iostream>
using namespace std;

/**
 * @class CesarEncryption
 * @brief Implementación del cifrado César con soporte para mayúsculas, minúsculas y dígitos.
 *
 * Esta clase permite cifrar, descifrar y realizar fuerza bruta sobre un mensaje utilizando el clásico cifrado de desplazamiento.
 */
class CesarEncryption {
public:
  /**
   * @brief Constructor por defecto.
   */
  CesarEncryption() = default;

  /**
   * @brief Destructor por defecto.
   */
  ~CesarEncryption() = default;

  /**
   * @brief Cifra un texto utilizando el algoritmo César.
   *
   * Recorre cada carácter del texto y aplica un desplazamiento si es letra o número.
   *
   * @param texto Texto plano a cifrar.
   * @param desplazamiento Cantidad de posiciones a desplazar (cifrado).
   * @return Texto cifrado.
   */
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
        result += c; // Deja símbolos y espacios sin cifrar
      }
    }

    return result;
  }

  /**
   * @brief Descifra un texto utilizando el algoritmo César.
   *
   * Internamente reutiliza `Encryption` con desplazamiento inverso.
   *
   * @param texto Texto cifrado.
   * @param desplazamiento Cantidad de posiciones con las que fue cifrado.
   * @return Texto descifrado.
   */
  std::string decode(const std::string& texto, int desplazamiento) {
    return Encryption(texto, 26 - (desplazamiento % 26));
  }

  /**
   * @brief Realiza un ataque de fuerza bruta mostrando los 26 posibles desplazamientos.
   *
   * Intenta descifrar el mensaje con todos los desplazamientos posibles del cifrado César.
   *
   * @param texto Texto cifrado.
   * @return Cadena con todas las combinaciones posibles.
   */
  std::string BruteForce(const std::string& texto) {
    std::string result = "";
    for (int i = 0; i < 26; i++) {
      result += "Desplazamiento " + to_string(i) + ": " + decode(texto, i) + "\n";
    }
    return result;
  }

private:
  // No se utilizan atributos privados en esta clase.
};
