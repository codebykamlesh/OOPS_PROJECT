# 🏦 Bank Management System (C++ CLI)

A fully functional **Bank Management System** built in **C++** with a rich terminal UI (colors, tables, and formatted output).
This project supports **Admin Panel** and **Customer Panel** with secure PIN-based operations and transaction logging.

---

## ✨ Features

### 🔐 Admin Panel

* Secure login system (username + password)
* Add new customer accounts
* View all accounts in table format
* Deposit & Withdraw from any account
* View all transactions
* Search accounts
* Delete accounts (**no PIN required**)
* Add new admin users

---

### 👤 Customer Panel

* Access using account number
* View personal account details
* Deposit & Withdraw money (PIN required)
* View own transactions
* Delete own account (**PIN required**)

---

### 🔒 Security Features

* Password & PIN hashing
* Hidden password input (masked with `*`)
* PIN attempt limit (3 tries)
* Temporary account lock (30 seconds after failed attempts)

---

### 📊 Data Management

* Data stored in CSV files:

  * `data.csv` → customer records
  * `pins.csv` → hashed PINs
  * `user.csv` → admin credentials
  * `transactions.csv` → transaction history

---

### 🎨 UI Features

* Colored terminal interface
* Structured tables for accounts & transactions
* Clean navigation menus
* Cross-platform support (Windows/Linux)

---

## 🛠️ Technologies Used

* C++
* STL (map, vector, stringstream, etc.)
* File Handling (CSV-based storage)
* Terminal control (ANSI colors)

---

## 🚀 How to Run

### 🔹 Compile

```bash
g++ main.cpp -o bank
```

### 🔹 Run

```bash
./bank
```

---

## 🧠 How It Works

1. On start, choose:

   * Admin Login
   * Customer Access

2. Admin:

   * Must login
   * Gets full system control

3. Customer:

   * Enters account number
   * Can perform limited operations

---

## 🔥 Special Logic

### 🗑️ Account Deletion Rules

| User Type | PIN Required |
| --------- | ------------ |
| Admin     | ❌ No         |
| Customer  | ✅ Yes        |

---

## 📂 Project Structure

```
📁 Project Folder
 ├── main.cpp
 ├── data.csv
 ├── pins.csv
 ├── user.csv
 └── transactions.csv
```

---

## 💡 Future Improvements

* GUI version (Windows app)
* Online database integration
* Customer login via PIN instead of account number
* Email/SMS notifications
* Advanced analytics dashboard

---

## 🧑‍💻 Author

* TEAM CODE CATALYST
* Purohit Kamleshsingh Savalsingh
* Anuruddha kumar kahar
* Aayush Singh
* Shrimali Jainam Ravindrakumar
* Joshi Rudram Manojbhai

---

## ⭐ Support

If you like this project:

* ⭐ Star the repo
* 🍴 Fork it
* 🚀 Improve it

---
