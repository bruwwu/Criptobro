// FileIO.h  ─── versión “ligera” inspirada en Vignere
#pragma once
#include "Prerequisites.h"

namespace fs = std::filesystem;

class FileIO {
  // Cambia ROOT si algún día quieres leer todo desde otra carpeta.
  static inline const fs::path ROOT = fs::current_path();

public:
  /* ────────── NUEVAS SOBRECARGAS ────────── */
  static std::string readAll(const std::string& fileName) {
    return readAll(ROOT / fileName);              // delega
  }
  static void writeAll(const std::string& fileName,
    const std::string& data) {
    writeAll(ROOT / fileName, data);              // delega
  }

  /* ────────── API ORIGINAL ────────── */
  static std::string readAll(const fs::path& in) {
    std::ifstream ifs(in, std::ios::binary);
    if (!ifs)
      throw std::runtime_error("No se pudo abrir " + in.string());
    return { std::istreambuf_iterator<char>(ifs),
             std::istreambuf_iterator<char>() };
  }

  static void writeAll(const fs::path& out, const std::string& data) {
    if (!out.parent_path().empty())
      fs::create_directories(out.parent_path());
    std::ofstream ofs(out, std::ios::binary | std::ios::trunc);
    if (!ofs)
      throw std::runtime_error("No se pudo abrir " + out.string());
    ofs.write(data.data(), static_cast<std::streamsize>(data.size()));
  }


  /* (sha256 sigue igual; lo omito por brevedad) */
};
