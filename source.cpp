#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
using namespace std;

class BankAccount {
public:
    int accNumber;
    string name;
    string accType;
    double balance;

    void input() {
        cout << "Enter Account Number: ";
        cin >> accNumber;
        cout << "Enter Account Holder Name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter Account Type (Saving/Current): ";
        cin >> accType;
        cout << "Enter Initial Balance: ₹";
        cin >> balance;
    }

    void display() const {
        cout << "----------------------------------------\n";
        cout << "Account Number : " << accNumber << endl;
        cout << "Account Holder : " << name << endl;
        cout << "Account Type   : " << accType << endl;
        cout << fixed << setprecision(2);
        cout << "Balance        : ₹" << balance << endl;
        cout << "----------------------------------------\n";
    }

    void writeToFile(ofstream& file) const {
        file << accNumber << "," << name << "," << accType << "," << balance << endl;
    }
};

// Load all accounts from file
void loadAccounts(vector<BankAccount>& accounts) {
    accounts.clear();
    ifstream file("accounts.txt");
    string line;
    while (getline(file, line)) {
        BankAccount acc;
        size_t pos1 = line.find(',');
        size_t pos2 = line.find(',', pos1 + 1);
        size_t pos3 = line.find(',', pos2 + 1);

        acc.accNumber = stoi(line.substr(0, pos1));
        acc.name = line.substr(pos1 + 1, pos2 - pos1 - 1);
        acc.accType = line.substr(pos2 + 1, pos3 - pos2 - 1);
        acc.balance = stod(line.substr(pos3 + 1));

        accounts.push_back(acc);
    }
    file.close();
}

// Rewrite updated data to file
void saveAccounts(const vector<BankAccount>& accounts) {
    ofstream file("accounts.txt", ios::trunc);
    for (const auto& acc : accounts) {
        acc.writeToFile(file);
    }
    file.close();
}

void createAccount() {
    BankAccount acc;
    acc.input();
    ofstream file("accounts.txt", ios::app);
    acc.writeToFile(file);
    file.close();
    cout << "Account created and saved.\n";
}

void displayAllAccounts() {
    vector<BankAccount> accounts;
    loadAccounts(accounts);

    if (accounts.empty()) {
        cout << "No account records found.\n";
        return;
    }

    for (const auto& acc : accounts) {
        acc.display();
    }
}

void searchAccount(int searchAccNo) {
    vector<BankAccount> accounts;
    loadAccounts(accounts);

    bool found = false;
    for (const auto& acc : accounts) {
        if (acc.accNumber == searchAccNo) {
            acc.display();
            found = true;
            break;
        }
    }

    if (!found)
        cout << "Account not found.\n";
}

void depositOrWithdraw(bool isDeposit) {
    int accNo;
    double amount;
    vector<BankAccount> accounts;
    loadAccounts(accounts);

    cout << "Enter Account Number: ";
    cin >> accNo;
    bool found = false;

    for (auto& acc : accounts) {
        if (acc.accNumber == accNo) {
            cout << (isDeposit ? "Enter amount to deposit: ₹" : "Enter amount to withdraw: ₹");
            cin >> amount;

            if (isDeposit) {
                acc.balance += amount;
                cout << amount << " deposited.\n";
            } else {
                if (acc.balance >= amount) {
                    acc.balance -= amount;
                    cout << amount << " withdrawn.\n";
                } else {
                    cout << "Insufficient balance.\n";
                    return;
                }
            }
            found = true;
            break;
        }
    }

    if (found)
        saveAccounts(accounts);
    else
        cout << "Account not found.\n";
}

void modifyAccount() {
    int accNo;
    vector<BankAccount> accounts;
    loadAccounts(accounts);

    cout << "Enter Account Number to Modify: ";
    cin >> accNo;
    bool found = false;

    for (auto& acc : accounts) {
        if (acc.accNumber == accNo) {
            cout << "Modify Account Holder Name (current: " << acc.name << "): ";
            cin.ignore();
            getline(cin, acc.name);
            cout << "Modify Account Type (current: " << acc.accType << "): ";
            cin >> acc.accType;
            cout << "Modify Balance (current: ₹" << acc.balance << "): ₹";
            cin >> acc.balance;
            found = true;
            break;
        }
    }

    if (found) {
        saveAccounts(accounts);
        cout << "Account details updated successfully.\n";
    } else {
        cout << "Account not found.\n";
    }
}

int main() {
    int choice;

    do {
        cout << "\n========= BANK MANAGEMENT SYSTEM =========\n";
        cout << "1. Create Account\n";
        cout << "2. Display All Accounts\n";
        cout << "3. Search Account\n";
        cout << "4. Deposit Money\n";
        cout << "5. Withdraw Money\n";
        cout << "6. Modify Account\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: createAccount(); break;
            case 2: displayAllAccounts(); break;
            case 3: {
                int accNo;
                cout << "Enter Account Number to search: ";
                cin >> accNo;
                searchAccount(accNo);
                break;
            }
            case 4: depositOrWithdraw(true); break;
            case 5: depositOrWithdraw(false); break;
            case 6: modifyAccount(); break;
            case 7: cout << "Exiting... Thank you!\n"; break;
            default: cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 7);

    return 0;
}
