# iPhone Rental System

A comprehensive C++ application for managing iPhone rental operations, built with modern development practices using xmake build system.

## Project Overview

This system provides complete management of an iPhone rental business with the following features:

- **User Management**: Admin and Customer roles with authentication
- **iPhone Inventory**: Track iPhone models, pricing, and availability status
- **Reservation System**: Queue-based reservation with priority levels (VIP, Regular, New)
- **Transaction Handling**: Rental checkout, return, and payment processing with late fees
- **Reporting**: Revenue reports, popularity analytics, occupancy rates, and user spending analysis

## Requirements

- **C++17** or later
- **xmake** build system

### Installing xmake

```bash
# On Ubuntu/Debian
curl -fsSL https://xmake.io/shget.text | bash

# On macOS
brew install xmake

# Other platforms: https://xmake.io/guide/installation.html
```

## Building the Project

Navigate to the project root and run:

```bash
xmake build
```

The compiled executable will be created in `bin/app`.

### Build Options

```bash
# Release build (optimized)
xmake build -m release

# Debug build (with symbols)
xmake build -m debug
```

## Running the Application

```bash
xmake run app
```

## Project Structure

```
proyek-akhir-apl/
├── src/              # Main application entry point
├── includes/         # Header files (structs, constants, helpers)
├── modules/          # Core logic modules (rental, users, priority, transactions, reports)
├── utils/            # Utility functions (file I/O, validation)
├── data/             # CSV data files (iphones, users, reservations, transactions)
├── xmake.lua         # Build configuration
└── README.md         # This file
```

## Default Credentials

- **Admin**: username `admin`, password `admin`
- **Sample Users**: Check `data/users.csv`
