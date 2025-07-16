// FileIO.h ─── Versión ligera inspirada en Vigenère
#pragma once
#include "Prerequisites.h"

namespace fs = std::filesystem;

/**
 * @class FileIO
 * @brief Utilidad estática para lectura y escritura de archivos en disco.
 *
 * Facilita la lectura y escritura de texto en archivos binarios de forma sencilla y directa.
 * Está pensada para usarse en conjunto con operaciones de cifrado (como César, XOR, Vigenère).
 */
class FileIO {
  /**
   * @brief Ruta raíz desde la cual se resuelven los nombres de archivo relativos.
   *
   * Por defecto es el directorio actual, pero puede modificarse si se desea centralizar
   * todos los accesos a archivos desde una carpeta específica.
   */
  static inline const fs::path ROOT = fs::current_path();

public:
  // ─────────────────────────────────────────────
  // NUEVAS SOBRECARGAS (delegan a API original)
  // ─────────────────────────────────────────────

  /**
   * @brief Lee el contenido completo de un archivo dado su nombre (relativo al directorio raíz).
   *
   * @param fileName Nombre del archivo a leer.
   * @return Cadena con el contenido del archivo.
   */
  static std::string readAll(const std::string& fileName) {
    return readAll(ROOT / fileName);
  }

  /**
   * @brief Escribe datos completos a un archivo (relativo al directorio raíz).
   *
   * Crea directorios intermedios si es necesario.
   *
   * @param fileName Nombre del archivo de salida.
   * @param data Contenido a escribir.
   */
  static void writeAll(const std::string& fileName,
    const std::string& data) {
    writeAll(ROOT / fileName, data);
  }

  // ─────────────────────────────────────────────
  // API ORIGINAL ─ acceso absoluto con std::filesystem::path
  // ─────────────────────────────────────────────

  /**
   * @brief Lee todo el contenido de un archivo dado su path completo.
   *
   * Abre el archivo en modo binario.
   *
   * @param in Ruta absoluta o relativa del archivo.
   * @return Contenido completo del archivo como string.
   * @throws std::runtime_error si el archivo no puede abrirse.
   */
  static std::string readAll(const fs::path& in) {
    std::ifstream ifs(in, std::ios::binary);
    if (!ifs)
      throw std::runtime_error("No se pudo abrir " + in.string());

    return { std::istreambuf_iterator<char>(ifs),
             std::istreambuf_iterator<char>() };
  }

  /**
   * @brief Escribe datos binarios en un archivo dado su path completo.
   *
   * Crea automáticamente las carpetas necesarias y sobrescribe si el archivo ya existe.
   *
   * @param out Ruta de salida.
   * @param data Contenido a escribir.
   * @throws std::runtime_error si no se puede abrir el archivo.
   */
  static void writeAll(const fs::path& out, const std::string& data) {
    if (!out.parent_path().empty())
      fs::create_directories(out.parent_path());

    std::ofstream ofs(out, std::ios::binary | std::ios::trunc);
    if (!ofs)
      throw std::runtime_error("No se pudo abrir " + out.string());

    ofs.write(data.data(), static_cast<std::streamsize>(data.size()));
  }
};
