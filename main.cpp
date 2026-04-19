#include <bits/stdc++.h>
#include <ctime>

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

using namespace std;

// ---------- COLORS ----------
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"

// ---------- UI ----------
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pauseScreen() {
    cout << YELLOW << "\nPress Enter to continue..." << RESET;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// ---------- TIME ----------
string getCurrentTime() {
    time_t now = time(0);
    tm *ltm = localtime(&now);

    char buffer[100];
    strftime(buffer, sizeof(buffer),
             "%a %d %b %Y %H:%M:%S",
             ltm);

    return string(buffer);
}

// ---------- HIDDEN INPUT ----------
string hiddenInput() {
    string pass;
    char ch;

#ifdef _WIN32
    while ((ch = _getch()) != '\r') {
        if (ch == '\b' && !pass.empty()) {
            pass.pop_back();
            cout << "\b \b";
        } else {
            pass += ch;
            cout << "*";
        }
    }
#else
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    getline(cin, pass);

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    cout << string(pass.length(), '*');
#endif

    cout << endl;
    return pass;
}

// ---------- HASH ----------
string hashStr(string s) {
    long long h = 0;
    long long p = 31;
    long long mod = 1e9 + 7;

    for (char c : s) {
        h = (h * p + c) % mod;
    }

    return to_string(h);
}

// ---------- LOGIN ----------
bool login() {
    clearScreen();

    cout << CYAN << BOLD;
    cout << "================================\n";
    cout << "           LOGIN PORTAL          \n";
    cout << "================================\n" << RESET;

    ifstream file("user.csv");

    // First user creation
    if (!file) {
        string user, pass;
        cout << BLUE << "Create Admin Username: " << RESET;
        cin >> user;
        cout << BLUE << "Create Admin Password: " << RESET;
        cin.ignore();
        pass = hiddenInput();

        ofstream out("user.csv");
        out << "Usernam,Password\n";
        out << user << "," << hashStr(pass) << endl;

        cout << GREEN << "[SUCCESS] Admin Created!\n" << RESET;
        return true;
    }

    string inputUser, inputPass;
    cout << BLUE << "Username: " << RESET;
    cin >> inputUser;
    cout << BLUE << "Password: " << RESET;
    cin.ignore();
    inputPass = hiddenInput();

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string u, p;
        getline(ss, u, ',');
        getline(ss, p, ',');

        if (u == inputUser && p == hashStr(inputPass)) {
            cout << GREEN << "[SUCCESS] Login Successful!\n" << RESET;
            return true;
        }
    }

    cout << RED << "[ERROR] Login Failed!\n" << RESET;
    return false;
}

// ---------- ADD ADMIN ----------
void addAdmin() {
    string user, pass;

    cout << BLUE << "New Admin Username: " << RESET;
    cin >> user;

    cout << BLUE << "New Admin Password: " << RESET;
    cin.ignore();
    pass = hiddenInput();

    ofstream file("user.csv", ios::app);
    file << user << "," << hashStr(pass) << endl;

    cout << GREEN << "[SUCCESS] New Admin Added!\n" << RESET;
}

// ---------- CUSTOMER ----------
class Customer {
public:
    string name;
    int acc;
    float bal;

    Customer(string n="", int a=0, float b=0)
        : name(n), acc(a), bal(b) {}
};

// ---------- RANDOM ACCOUNT ----------
int genAcc(map<int, Customer>& m) {
    int x;
    do {
        x = rand() % 900000 + 100000;
    } while (m.count(x));
    return x;
}

// ---------- FILES ----------
void saveCustomers(map<int, Customer>& m) {
    ofstream f("data.csv", ios::trunc);
    f << "Acc,Name,Balance\n";
    f << fixed << setprecision(2);

    for (auto &p : m)
        f << p.second.acc << "," << p.second.name << "," << p.second.bal << endl;
}

void loadCustomers(map<int, Customer>& m) {
    ifstream f("data.csv");
    string line;
    getline(f, line);

    while (getline(f, line)) {
        stringstream ss(line);
        string a,n,b;
        getline(ss,a,','); getline(ss,n,','); getline(ss,b,',');

        m[stoi(a)] = Customer(n, stoi(a), stof(b));
    }
}

// ---------- PIN FILE ----------
void savePins(map<int,string>& pins) {
    ofstream f("pins.csv", ios::trunc);
    f << "Acc,PIN\n";

    for (auto &p : pins)
        f << p.first << "," << p.second << endl;
}

void loadPins(map<int,string>& pins) {
    ifstream f("pins.csv");
    if (!f) return;

    string line;
    getline(f, line); // skip header

    while (getline(f, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string acc, pin;

        getline(ss, acc, ',');
        getline(ss, pin, ',');

        pins[stoi(acc)] = pin;
    }
}

// ---------- VERIFY PIN ----------
bool verifyPin(map<int,string>& pins, int acc, string input) {
    return pins.count(acc) && pins[acc] == hashStr(input);
}

// ---------- TRANSACTION ----------
void logTrans(int acc, string type, float amt) {
    bool exists = ifstream("transactions.csv").good();
    ofstream f("transactions.csv", ios::app);

    if (!exists)
        f << "DateTime,Account,Type,Amount\n";

    f << getCurrentTime() << ","
         << acc << ","
         << type << ","
         << amt << endl;
}

// ---------- TABLE DISPLAY ----------
void printCustomersTable(map<int, Customer>& m) {
    cout << CYAN << BOLD;
    cout << "+------------+----------------------+------------+\n";
    cout << "| Account No | Name                 | Balance    |\n";
    cout << "+------------+----------------------+------------+\n";
    cout << RESET;

    for (auto &p : m) {
        cout << CYAN << "| " << RESET;
        cout << left << setw(10) << p.second.acc << " | "
             << left << setw(20) << p.second.name << " | "
             << right << setw(10) << fixed << setprecision(2) << p.second.bal
             << CYAN << " |\n" << RESET;
    }

    cout << CYAN;
    cout << "+------------+----------------------+------------+\n";
    cout << RESET;
}

// ---------- TRANSACTION TABLE ----------
void printTransactionTable() {
    ifstream file("transactions.csv");
    string line;

    if (!file || file.peek() == EOF) {
        cout << YELLOW << "No transactions yet.\n" << RESET;
        return;
    }

    cout << CYAN << BOLD;
    cout << "+---------------------------+-----------+------------+----------+\n";
    cout << "| DateTime                  | Account   | Type       | Amount   |\n";
    cout << "+---------------------------+-----------+------------+----------+\n";
    cout << RESET;

    getline(file, line); // skip header

    bool found = false;

    while (getline(file, line)) {
        stringstream ss(line);
        string dt, acc, type, amt;

        getline(ss, dt, ',');
        getline(ss, acc, ',');
        getline(ss, type, ',');
        getline(ss, amt, ',');

        cout << CYAN << "| " << GREEN;
        cout << left << setw(25) << dt << " | "
             << setw(9) << acc << " | "
             << setw(10) << type << " | "
             << setw(8) << amt
             << CYAN << " |\n";

        cout << RESET;
        found = true;
    }

    if (!found)
        cout << YELLOW << "No transactions yet.\n" << RESET;

    cout << CYAN;
    cout << "+---------------------------+-----------+------------+----------+\n";
    cout << RESET;
}

// ---------- VIEW TRANSACTIONS ----------
void viewAllTransactions() {
    printTransactionTable();
}

void viewAccountTransactions(int acc) {
    ifstream f("transactions.csv");

    if (!f || f.peek() == EOF) {
        cout << YELLOW << "No transactions yet.\n" << RESET;
        return;
    }

    string line;
    getline(f, line); // skip header

    cout << CYAN << BOLD;
    cout << "+---------------------------+-----------+------------+----------+\n";
    cout << "| DateTime                  | Account   | Type       | Amount   |\n";
    cout << "+---------------------------+-----------+------------+----------+\n";
    cout << RESET;

    bool found = false;

    while (getline(f, line)) {
        if (line.find(to_string(acc)) != string::npos) {
            stringstream ss(line);
            string dt, a, type, amt;

            getline(ss, dt, ',');
            getline(ss, a, ',');
            getline(ss, type, ',');
            getline(ss, amt, ',');

            cout << CYAN << "| " << GREEN;
            cout << left << setw(25) << dt << " | "
                 << setw(9) << a << " | "
                 << setw(10) << type << " | "
                 << setw(8) << amt
                 << CYAN << " |\n";

            cout << RESET;
            found = true;
        }
    }

    if (!found)
        cout << YELLOW << "No transactions for this account.\n" << RESET;

    cout << CYAN;
    cout << "+---------------------------+-----------+------------+----------+\n";
    cout << RESET;
}

// ---------- SEARCH ----------
void searchAcc(map<int, Customer>& m) {
    int a;
    cout << BLUE << "Enter Account No: " << RESET;
    cin >> a;

    if (m.count(a)) {
        cout << CYAN << BOLD;
        cout << "+------------+----------------------+------------+\n";
        cout << "| Account No | Name                 | Balance    |\n";
        cout << "+------------+----------------------+------------+\n";
        cout << RESET;

        cout << CYAN << "| " << RESET;
        cout << left << setw(10) << m[a].acc << " | "
             << left << setw(20) << m[a].name << " | "
             << right << setw(10) << fixed << setprecision(2) << m[a].bal
             << CYAN << " |\n" << RESET;

        cout << CYAN;
        cout << "+------------+----------------------+------------+\n";
        cout << RESET;
    } else {
        cout << RED << "[ERROR] Account not found!\n" << RESET;
    }
}

// ---------- PIN ATTEMPT SYSTEM ----------
bool checkPin(map<int,string>& pins,
              map<int,int>& attempts,
              map<int,time_t>& blockTime,
              int acc,
              string input) {

    time_t now = time(0);

    // ⏳ Auto unlock after 30 seconds
    if (blockTime[acc] != 0 && (now - blockTime[acc]) >= 30) {
        attempts[acc] = 0;
        blockTime[acc] = 0;

        cout << GREEN << "[UNLOCKED] You can try again now.\n" << RESET;
    }

    // 🔒 If still blocked
    if (attempts[acc] >= 3) {
        int remaining = 30 - (now - blockTime[acc]);

        if (remaining > 0) {
            cout << RED << "[BLOCKED] Try again in "
                 << remaining << " seconds.\n" << RESET;
            return false;
        }
    }

    // ✅ Correct PIN
    if (pins[acc] == hashStr(input)) {
        attempts[acc] = 0;
        blockTime[acc] = 0;
        return true;
    }

    // ❌ Wrong PIN
    attempts[acc]++;

    cout << RED << "[ERROR] Wrong PIN! Attempts left: "
         << (3 - attempts[acc]) << RESET << endl;

    if (attempts[acc] >= 3) {
        blockTime[acc] = now;
        cout << RED << "[BLOCKED] Account locked for 30 seconds!\n" << RESET;
    }

    return false;
}

// ---------- MAIN ----------
int main() {
    srand(time(0));

    // 🔐 ROLE SELECTION
    int role;
    clearScreen();

    cout << CYAN << BOLD;
    cout << "================================\n";
    cout << "       SELECT USER TYPE         \n";
    cout << "================================\n" << RESET;

    cout << YELLOW;
    cout << "1. Admin Login\n";
    cout << "2. Customer\n" << RESET;

    cout << BLUE << "Enter choice: " << RESET;
    cin >> role;

    bool isAdmin = false;

    if (role == 1) {
        if (!login()) return 0;
        isAdmin = true;
    }
    else if (role == 2) {
        cout << GREEN << "[INFO] Customer Mode Activated\n" << RESET;
    }
    else {
        cout << RED << "[ERROR] Invalid choice!\n" << RESET;
        return 0;
    }

    map<int, Customer> m;
    map<int, string> pins;
    map<int, int> pinAttempts;
    map<int, time_t> blockTime;

    loadCustomers(m);
    loadPins(pins);

    int currentAcc = -1;

    if (!isAdmin) {
        cout << BLUE << "Enter Your Account Number: " << RESET;
        cin >> currentAcc;
    }

    while (1) {
        clearScreen();

        cout << CYAN << BOLD;
        cout << "================================\n";
        cout << "     BANK MANAGEMENT SYSTEM     \n";
        cout << "================================\n" << RESET;

        // 🔥 SPLIT MENU
        if (isAdmin) {
            cout << YELLOW;
            cout << "1. Add Account\n";
            cout << "2. View Accounts\n";
            cout << "3. Deposit\n";
            cout << "4. Withdraw\n";
            cout << "5. View All Transactions\n";
            cout << "6. View Account Transactions\n";
            cout << "7. Search Account\n";
            cout << "8. Delete Account\n";
            cout << "9. Add Admin\n";
            cout << "0. Exit\n" << RESET;
        }
        else {
            cout << YELLOW;
            cout << "1. View My Account\n";
            cout << "2. Deposit\n";
            cout << "3. Withdraw\n";
            cout << "4. View My Transactions\n";
            cout << "5. Delete My Account\n";
            cout << "0. Exit\n" << RESET;
        }

        int c;
        cout << BLUE << "Enter choice: " << RESET;

        if (!(cin >> c)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << RED << "[ERROR] Invalid input!\n" << RESET;
            pauseScreen();
            continue;
        }

        // ---------------- ADMIN FULL ACCESS ----------------
        if (isAdmin) {

            if (c == 1) {
                string n,p;
                float b;

                cin.ignore();
                cout << BLUE << "Name: " << RESET; getline(cin,n);
                cout << BLUE << "Balance: " << RESET; cin >> b;

                do {
                    cout << BLUE << "Enter 4-digit PIN: " << RESET;
                    cin >> p;
                } while (p.length() != 4);

                int a = genAcc(m);
                m[a] = Customer(n,a,b);
                pins[a] = hashStr(p);

                saveCustomers(m);
                savePins(pins);

                cout << GREEN << "[SUCCESS] Account Created! Account No: " << a << "\n" << RESET;
                pauseScreen();
            }

            else if (c == 2) {
                printCustomersTable(m);
                pauseScreen();
            }

            else if (c == 3 || c == 4) {
                int a; float amt; string p;

                cout << BLUE << "Account No: " << RESET; cin >> a;
                cout << BLUE << "Amount: " << RESET; cin >> amt;
                cout << BLUE << "PIN: " << RESET; cin >> p;

                if (!m.count(a))
                    cout << RED << "[ERROR] Account not found!\n" << RESET;
                else if (!checkPin(pins, pinAttempts, blockTime, a, p));
                else {
                    if (c == 3) {
                        m[a].bal += amt;
                        logTrans(a,"Deposit",amt);
                    } else {
                        if (amt > m[a].bal)
                            cout << RED << "[ERROR] Insufficient balance!\n" << RESET;
                        else {
                            m[a].bal -= amt;
                            logTrans(a,"Withdraw",amt);
                        }
                    }
                    saveCustomers(m);
                }

                pauseScreen();
            }

            else if (c == 5) {
                viewAllTransactions();
                pauseScreen();
            }

            else if (c == 6) {
                int a;
                cout << BLUE << "Account No: " << RESET;
                cin >> a;
                viewAccountTransactions(a);
                pauseScreen();
            }

            else if (c == 7) {
                searchAcc(m);
                pauseScreen();
            }

            // 🔥 ADMIN DELETE (NO PIN)
            else if (c == 8) {
                int a;
                cout << BLUE << "Enter Account No to delete: " << RESET;
                cin >> a;

                if (m.count(a)) {
                    m.erase(a);
                    pins.erase(a);
                    saveCustomers(m);
                    savePins(pins);
                    cout << GREEN << "[SUCCESS] Account deleted!\n" << RESET;
                } else {
                    cout << RED << "[ERROR] Account not found!\n" << RESET;
                }
                pauseScreen();
            }

            else if (c == 9) {
                addAdmin();
                pauseScreen();
            }
            else if (c == 0) {
                cout << GREEN << BOLD << "Exiting... Goodbye!\n" << RESET;
                break;
            }
        }

        // ---------------- CUSTOMER PANEL ----------------
        else {

            if (!m.count(currentAcc)) {
                cout << RED << "[ERROR] Account not found!\n" << RESET;
                pauseScreen();
                continue;
            }

            if (c == 1) {
                searchAcc(m); // reuse UI
                pauseScreen();
            }

            else if (c == 2 || c == 3) {
                float amt; string p;

                cout << BLUE << "Amount: " << RESET; cin >> amt;
                cout << BLUE << "PIN: " << RESET; cin >> p;

                if (!checkPin(pins, pinAttempts, blockTime, currentAcc, p));
                else {
                    if (c == 2) {
                        m[currentAcc].bal += amt;
                        logTrans(currentAcc,"Deposit",amt);
                    } else {
                        if (amt > m[currentAcc].bal)
                            cout << RED << "[ERROR] Insufficient balance!\n" << RESET;
                        else {
                            m[currentAcc].bal -= amt;
                            logTrans(currentAcc,"Withdraw",amt);
                        }
                    }
                    saveCustomers(m);
                }
                pauseScreen();
            }

            else if (c == 4) {
                viewAccountTransactions(currentAcc);
                pauseScreen();
            }

            // 🔥 CUSTOMER DELETE (PIN REQUIRED)
            else if (c == 5) {
                string p;
                cout << BLUE << "Enter PIN: " << RESET;
                cin >> p;

                if (!verifyPin(pins, currentAcc, p)) {
                    cout << RED << "[ERROR] Wrong PIN!\n" << RESET;
                } else {
                    m.erase(currentAcc);
                    pins.erase(currentAcc);
                    saveCustomers(m);
                    savePins(pins);
                    cout << GREEN << "[SUCCESS] Account deleted!\n" << RESET;
                }
                pauseScreen();
            }
            else if (c == 0) {
                cout << GREEN << BOLD << "Exiting... Goodbye!\n" << RESET;
                break;
            }

            else {
                cout << RED << "[ERROR] Invalid choice!\n" << RESET;
                pauseScreen();
    }
    }
}
}