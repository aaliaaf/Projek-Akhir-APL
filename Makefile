# Makefile untuk Sistem Penyewaan iPhone
# -std=c++17, compile dengan g++
#
# Cara pakai:
#   make        -> compile program
#   make debug  -> compile dengan flag DEBUG (mengaktifkan ASSERT)
#   make clean  -> hapus binary
#   make run    -> compile + jalankan

CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iincludes
LDFLAGS  =
TARGET   = bin/app

# Semua source file
SRCS = \
    src/main.cpp \
    modules/core_rental.cpp \
    modules/priority.cpp \
    modules/reports.cpp \
    modules/transactions.cpp \
    modules/user_manager.cpp \
    utils/file_io.cpp \
    utils/validator.cpp

# Default target
all: $(TARGET)

$(TARGET): $(SRCS) includes/helpers.h includes/structs.h includes/constants.h includes/my_library.h
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) -o $@ $(SRCS) $(LDFLAGS)
	@echo "=== Build berhasil! Jalankan dengan: ./$(TARGET) ==="

# Mode debug: ASSERT aktif
debug: CXXFLAGS += -DDEBUG -g
debug: all

run: all
	./$(TARGET)

clean:
	rm -rf bin/

.PHONY: all debug run clean
