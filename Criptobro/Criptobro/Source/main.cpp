// main.cpp (versión con .des en texto hex)
#include "FileIO.h"               // lectura/escritura
#include "LiruSisaEncrypton.h"    // César
#include "XOR.h"                  // XOR
#include "Vinegere.h"             // Vigenère
#include "DES.h"                  // DES

#include <bitset>
#include <iostream>
#include <filesystem>
#include <algorithm>

namespace fs = std::filesystem;

/* ───── Toggle de trazas debug ───── */
#define DEBUG_LOG 0
#if DEBUG_LOG
#define DBG(x) do { std::cerr << "[DEBUG] " << x << '\n'; } while (0)
#else
#define DBG(x)
#endif

/* ──────────────────────────────────────────────────────────────
   Carpetas base:   raw/  (archivos originales)
                    cif/  (archivos cifrados en **hex**)
──────────────────────────────────────────────────────────────── */
const fs::path BASE_DIR = fs::current_path();
const fs::path RAW_DIR = BASE_DIR / "raw";
const fs::path CIF_DIR = BASE_DIR / "cif";

/* ───── Helpers DES con clave fija ───── */
namespace DESUtils {
  constexpr const char* DES_KEY =
    "0001001100110100010101110111100110011011101111001101111111110001";

  ::DES makeDES() {
    const std::bitset<64> bits{ std::string{DES_KEY} };
    return ::DES(bits);
  }

  std::string encrypt(const std::string& in) {
    auto des = makeDES();
    std::string out;
    for (size_t i = 0; i < in.size(); i += 8) {
      std::string block = in.substr(i, 8);
      if (block.size() < 8) block.append(8 - block.size(), '\0');
      out += des.bitset64ToString(
        des.encode(des.stringToBitset64(block)));
    }
    return out;                         // binario puro
  }

  std::string decrypt(const std::string& in) {
    auto des = makeDES();
    std::string out;
    for (size_t i = 0; i < in.size(); i += 8) {
      std::string block = in.substr(i, 8);
      out += des.bitset64ToString(
        des.decode(des.stringToBitset64(block)));
    }
    while (!out.empty() && out.back() == '\0') out.pop_back();
    return out;                         // texto/binario plano
  }
}

/* ───── Utilidades hex <-> bin ───── */
std::string binToHex(const std::string& bin)
{
  static const char* d = "0123456789ABCDEF";
  std::string hex;  hex.reserve(bin.size() * 2);
  for (unsigned char c : bin) {
    hex.push_back(d[c >> 4]);
    hex.push_back(d[c & 0x0F]);
  }
  return hex;
}

int hexVal(char ch) {
  return (ch <= '9') ? ch - '0' :
    (ch <= 'F') ? ch - 'A' + 10 : ch - 'a' + 10;
}

std::string hexToBin(const std::string& hex)
{
  std::string bin; bin.reserve(hex.size() / 2);
  for (size_t i = 0; i + 1 < hex.size(); i += 2)
    bin.push_back(char((hexVal(hex[i]) << 4) | hexVal(hex[i + 1])));
  return bin;
}

/* ───── Selección de algoritmo ───── */
enum class Cipher { CESAR, XORC, VIGENERE, DES };

Cipher askCipher() {
  int opc;
  std::cout <<
    "Elige algoritmo:\n"
    "  1) Cesar\n"
    "  2) XOR\n"
    "  3) Vigenère\n"
    "  4) DES (64-bit, clave fija)\n> ";
  std::cin >> opc;
  return static_cast<Cipher>(opc - 1);
}

bool askMode() {
  char c;
  std::cout << "¿Encrypt (e) o Decrypt (d)? > ";
  std::cin >> c;
  return (c == 'e' || c == 'E');
}

fs::path askInputPath(bool encrypting) {
  std::cout << "Nombre de archivo (sin ruta) > ";
  std::string name; std::cin >> name;
  return (encrypting ? RAW_DIR : CIF_DIR) / name;
}

std::string extFor(Cipher c) {
  switch (c) {
  case Cipher::CESAR:    return ".csr";
  case Cipher::XORC:     return ".xor";
  case Cipher::VIGENERE: return ".vig";
  case Cipher::DES:      return ".des";   // pero será TEXTO hex
  }
  return ".bin";
}

/* ───── Autotest (igual, ahora usa sample alfabético) ───── */
bool selfTest(Cipher c) {
  const std::string sample = "ANinIpmu";   // alfabético para César
  std::string enc, dec;
  switch (c) {
  case Cipher::CESAR: {
    CesarEncryption ce; int k = 5;
    enc = ce.Encryption(sample, k);
    dec = ce.decode(enc, k);
    break;
  }
  case Cipher::XORC: {
    XOREncoder xo; std::string key = "K";
    enc = xo.encode(sample, key);
    dec = xo.encode(enc, key);
    break;
  }
  case Cipher::VIGENERE: {
    Vignere v("KEY");
    enc = v.encode(sample);
    dec = v.decode(enc);
    break;
  }
  case Cipher::DES: {
    enc = DESUtils::encrypt(sample);
    dec = DESUtils::decrypt(enc);
    break;
  }
  }
  return dec == sample;
}

/* ───── Main ───── */
int main() try {
  std::ios::sync_with_stdio(false);
  fs::create_directories(RAW_DIR);
  fs::create_directories(CIF_DIR);

  Cipher cipher = askCipher();
  bool encrypting = askMode();

  if (!selfTest(cipher)) {
    std::cerr << "⛔ Validación interna del algoritmo falló.\n";
    return 2;
  }

  fs::path inFile = askInputPath(encrypting);
  const std::string ext = extFor(cipher);
  fs::path outFile;

  if (encrypting)
    outFile = CIF_DIR / (inFile.filename().string() + ext);
  else {
    std::string base = inFile.filename().string();
    if (inFile.extension() == ext)
      base = inFile.stem().string();
    else
      base += ".dec";
    outFile = RAW_DIR / base;
  }

  std::cout << "Leyendo de : " << inFile << '\n'
    << "Escribiendo: " << outFile << '\n';

  std::string input = FileIO::readAll(inFile);
  std::string output;

  switch (cipher) {
  case Cipher::CESAR: {
    int shift; std::cout << "Desplazamiento César (int) > "; std::cin >> shift;
    CesarEncryption ce;
    output = encrypting ? ce.Encryption(input, shift)
      : ce.decode(input, shift);
    break;
  }
  case Cipher::XORC: {
    std::string key; std::cout << "Clave XOR > "; std::cin >> key;
    XOREncoder xo; output = xo.encode(input, key);
    break;
  }
  case Cipher::VIGENERE: {
    std::string key; std::cout << "Clave Vigenère > "; std::cin >> key;
    Vignere v(key);
    output = encrypting ? v.encode(input) : v.decode(input);
    break;
  }
  case Cipher::DES: {
    if (encrypting) {
      std::string raw = DESUtils::encrypt(input);  // binario
      std::cout << "HEX preview: " << binToHex(raw).substr(0, 64) << "…\n";
      output = binToHex(raw);                     // se guarda como texto
    }
    else {
      std::string raw = hexToBin(input);          // de texto a binario
      output = DESUtils::decrypt(raw);
    }
    break;
  }
  }

  FileIO::writeAll(outFile, output);
  std::cout << "✅ Bytes escritos: " << output.size() << '\n';
  return 0;

}
catch (const std::exception& e) {
  std::cerr << "⛔ Error: " << e.what() << '\n';
  return 1;
}
