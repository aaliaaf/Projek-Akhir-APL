// MODUL 8: Include Guards (#ifndef/#define/#endif)
#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>
#include <iostream>

// ========== ENUM ==========
enum StatusiPhone { Tersedia, Disewa, Maintenance, Rusak };
enum UserLevel { VIP, REGULAR, BARU };

// ========== KONSTANTA ==========
const double DENDA_PER_HARI = 20000.0;
const double DISKON_VIP = 0.10;

// ========== DEBUGGING MACROS ==========
// MODUL 8: Teknik debugging dengan ASSERT, __FILE__, __LINE__, __func__

#ifdef DEBUG
    #define ASSERT(condition, message) \
        do { \
            if (!(condition)) { \
                std::cerr << "\n[ASSERTION FAILED]" << std::endl; \
                std::cerr << "  Kondisi: " << #condition << std::endl; \
                std::cerr << "  File   : " << __FILE__ << std::endl; \
                std::cerr << "  Line   : " << __LINE__ << std::endl; \
                std::cerr << "  Fungsi : " << __func__ << std::endl; \
                std::cerr << "  Pesan  : " << message << std::endl; \
                exit(1); \
            } \
        } while(0)
#else
    #define ASSERT(condition, message) ((void)0)
#endif

// MODUL 8: Macro untuk logging error otomatis
#define LOG_ERROR(msg) \
    std::cerr << "[ERROR] " << __FILE__ << ":" << __LINE__ << " (" << __func__ << ") - " << msg << std::endl

#endif
