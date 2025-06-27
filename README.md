# 🚇 CairoRide Metro System

## 📋 Overview

**CairoRide** is a console-based C++ application developed as a project for the *Structured Programming* course at the Faculty of Computers and Information, Ain Shams University.
It simulates a metro ticketing system for the Cairo Metro, allowing users to manage accounts, purchase subscriptions, book rides, and track travel history, while administrators can oversee user accounts, subscriptions, stations, and fares.
The system uses a text-based interface and stores data persistently in text files.
📄 For a detailed description of features and deliverables, refer to `CairoRide.pdf`.

---

## ✨ Key Features

### 👤 User Management

* Account creation, secure login/logout, and profile updates (username, password, balance, phone number).

### 🎟️ Subscription Plans

* **General**: Monthly (60 trips, 310–600 LE) or yearly (730 trips, 3500–5000 LE) for Zones 1–4.
* **Scholar**: 3-month plan (180 trips, 150–300 LE) for users aged 16–26.
* **Smart Wallet**: Pay-as-you-go with max balance of 400 LE (charged in multiples of 10 LE).

### 🚉 Ride Management

* Supports 3 metro lines:

  * Line 1: 35 stations
  * Line 2: 20 stations
  * Line 3: 34 stations (Main, Northern, Southern)
* Zone-based pricing:

  * 8 LE (1–9 stations)
  * 10 LE (10–16)
  * 15 LE (17–23)
  * 20 LE (24+)
* Tracks ride history with entry/exit stations, dates, and fares.

### 🛠️ Admin Controls

* Manage users, subscriptions, stations, fares, ride logs, and view statistics.

### 📀 Data Persistence

* Stores data in text files:

  * `Globals_File`, `Stations_File`, `User_File`, `Subscription_File`, `Ride_File`

### 🔐 Input Validation

* Validates phone numbers (11 digits, starting with 010/011/012/015), dates, and numeric inputs.

---

## ⚙️ Implementation Details

* **Data Structures**: Uses arrays of structs for User, Subscription, Ride, and Stations.
* **Constraints**:

  * Max: 1000 users, 1000 rides, 100 stations.
  * Transition stations (e.g., El Shohadaa, Nasser, Attaba) cannot be edited or removed.
* **File I/O**:

  * Files are created without extensions (e.g., `Globals_File`, not `Globals_File.txt`), but are plain text files.
* **Console Interface**:

  * Uses `<windows.h>` for coloring:

    * 🛣️ Purple → errors
    * 🟡 Yellow → success
    * ⚪ White → prompts

---

## 🧰 Setup Instructions

### 📦 Prerequisites

* C++ compiler (e.g., `g++`, `MSVC`) with C++11+
* Windows OS (for `<windows.h>` dependency)

### 🛠️ Installation

1. **Download** or clone `CairoRide.cpp`
2. **Compile**:

   ```bash
   g++ CairoRide.cpp -o CairoRide
   ```
3. **Initialize Files**:

   * In `main()`, uncomment `writing_process();` temporarily
   * Run the program once to generate the files
   * Then re-comment `writing_process();` to avoid overwriting
4. **Run**:

   ```bash
   ./CairoRide
   ```

---

## 📁 File Requirements

* Uses text files without extensions:

  * `Globals_File`, `Stations_File`, `User_File`, `Subscription_File`, `Ride_File`
* Created automatically in the same directory during first run.
* Ensure read/write permissions in the directory.
* Can be opened with any text editor (e.g., Notepad).

---

## 🧑‍💻 Usage

* **Main Menu**:

  * Create Account
  * Sign In (admin login: `admin` / `12345`)
  * Exit

* **User Menu**:

  * Manage subscriptions, book trips, view history, update profile, sign out.

* **Admin Menu**:

  * Manage users, subscriptions, stations, fares, ride logs, view statistics.

---

## ⚠️ Limitations

* 🪠 Windows-only (due to `<windows.h>`)
* 📃 Text-based interface (no GUI)
* 📊 Limited to 1000 users/rides and 100 stations
* ⛔ Basic file I/O error handling — ensure write access

---

## 📄 License

Licensed under the **MIT License**.
