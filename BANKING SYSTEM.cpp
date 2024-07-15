#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

#ifdef _WIN32
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

class Transaction {
public:
    int transactionID;
    std::string date;
    double amount;
    std::string type;
    std::string description;

    Transaction(int id, std::string dt, double amt, std::string tp, std::string desc)
        : transactionID(id), date(dt), amount(amt), type(tp), description(desc) {}

    void displayTransactionInfo() const {
        std::cout << std::setw(15) << "Transaction ID: " << transactionID 
                  << "\n" << std::setw(15) << "Date: " << date 
                  << "\n" << std::setw(15) << "Amount: " << amount 
                  << "\n" << std::setw(15) << "Type: " << type 
                  << "\n" << std::setw(15) << "Description: " << description << std::endl;
    }
};

class Account {
public:
    int accountNumber;
    std::string accountType;
    double balance;
    std::vector<Transaction> transactions;

    Account(int number, std::string type, double bal)
        : accountNumber(number), accountType(type), balance(bal) {}

    void deposit(double amount) {
        balance += amount;
        transactions.push_back(Transaction(transactions.size() + 1, "2024-07-15", amount, "Deposit", "Deposit of " + std::to_string(amount)));
    }

    void withdraw(double amount) {
        if (amount <= balance) {
            balance -= amount;
            transactions.push_back(Transaction(transactions.size() + 1, "2024-07-15", amount, "Withdrawal", "Withdrawal of " + std::to_string(amount)));
        } else {
            std::cout << "Insufficient funds." << std::endl;
        }
    }

    void transfer(Account &toAccount, double amount) {
        if (amount <= balance) {
            balance -= amount;
            toAccount.deposit(amount);
            transactions.push_back(Transaction(transactions.size() + 1, "2024-07-15", amount, "Transfer", "Transfer to account " + std::to_string(toAccount.accountNumber)));
        } else {
            std::cout << "Insufficient funds." << std::endl;
        }
    }

    double getBalance() const {
        return balance;
    }

    void displayAccountInfo() const {
        std::cout << "\nAccount Number: " << accountNumber 
                  << "\nAccount Type: " << accountType 
                  << "\nBalance: " << balance << "\nTransactions:\n";
        for (const auto &transaction : transactions) {
            transaction.displayTransactionInfo();
            std::cout << "--------------------------\n";
        }
    }
};

class Customer {
public:
    int customerID;
    std::string name;
    std::string address;
    std::string contact;
    std::vector<Account> accounts;

    Customer(int id, std::string nm, std::string addr, std::string cont)
        : customerID(id), name(nm), address(addr), contact(cont) {}

    void addAccount(Account account) {
        accounts.push_back(account);
    }

    void displayCustomerInfo() const {
        std::cout << "\nCustomer ID: " << customerID 
                  << "\nName: " << name 
                  << "\nAddress: " << address 
                  << "\nContact: " << contact 
                  << "\nAccounts:\n";
        for (const auto &account : accounts) {
            account.displayAccountInfo();
        }
    }
};

class BankingService {
public:
    std::vector<Customer> customers;

    void createCustomer() {
        int id;
        std::string name, address, contact;
        
        std::cout << "Enter Customer ID: ";
        std::cin >> id;
        std::cin.ignore();
        std::cout << "Enter Name: ";
        std::getline(std::cin, name);
        std::cout << "Enter Address: ";
        std::getline(std::cin, address);
        std::cout << "Enter Contact: ";
        std::getline(std::cin, contact);

        customers.push_back(Customer(id, name, address, contact));
        std::cout << "Customer created successfully.\n";
    }

    void createAccount(Customer &customer) {
        int accountNumber;
        std::string accountType;
        double initialBalance;

        std::cout << "Enter Account Number: ";
        std::cin >> accountNumber;
        std::cin.ignore();
        std::cout << "Enter Account Type: ";
        std::getline(std::cin, accountType);
        std::cout << "Enter Initial Balance: ";
        std::cin >> initialBalance;

        Account newAccount(accountNumber, accountType, initialBalance);
        customer.addAccount(newAccount);
        std::cout << "Account created successfully.\n";
    }

    void performDeposit(Customer &customer) {
        int accountNumber;
        double amount;

        std::cout << "Enter Account Number: ";
        std::cin >> accountNumber;
        std::cout << "Enter Deposit Amount: ";
        std::cin >> amount;

        for (auto &account : customer.accounts) {
            if (account.accountNumber == accountNumber) {
                account.deposit(amount);
                std::cout << "Deposit successful.\n";
                break;
            }
        }
    }

    void performWithdrawal(Customer &customer) {
        int accountNumber;
        double amount;

        std::cout << "Enter Account Number: ";
        std::cin >> accountNumber;
        std::cout << "Enter Withdrawal Amount: ";
        std::cin >> amount;

        for (auto &account : customer.accounts) {
            if (account.accountNumber == accountNumber) {
                account.withdraw(amount);
                std::cout << "Withdrawal successful.\n";
                break;
            }
        }
    }

    void performTransfer(Customer &fromCustomer, Customer &toCustomer) {
        int fromAccountNumber, toAccountNumber;
        double amount;

        std::cout << "Enter From Account Number: ";
        std::cin >> fromAccountNumber;
        std::cout << "Enter To Account Number: ";
        std::cin >> toAccountNumber;
        std::cout << "Enter Transfer Amount: ";
        std::cin >> amount;

        Account *fromAccount = nullptr;
        Account *toAccount = nullptr;

        for (auto &account : fromCustomer.accounts) {
            if (account.accountNumber == fromAccountNumber) {
                fromAccount = &account;
                break;
            }
        }

        for (auto &account : toCustomer.accounts) {
            if (account.accountNumber == toAccountNumber) {
                toAccount = &account;
                break;
            }
        }

        if (fromAccount && toAccount) {
            fromAccount->transfer(*toAccount, amount);
            std::cout << "Transfer successful.\n";
        } else {
            std::cout << "Account not found.\n";
        }
    }

    void viewAccountInfo(Customer &customer) {
        int accountNumber;

        std::cout << "Enter Account Number: ";
        std::cin >> accountNumber;

        for (const auto &account : customer.accounts) {
            if (account.accountNumber == accountNumber) {
                account.displayAccountInfo();
                break;
            }
        }
    }

    void viewCustomerInfo(Customer &customer) {
        customer.displayCustomerInfo();
    }

    Customer* findCustomer(int customerID) {
        for (auto &customer : customers) {
            if (customer.customerID == customerID) {
                return &customer;
            }
        }
        return nullptr;
    }
};

int main() {
    BankingService bank;
    int choice;

    while (true) {
        std::cout << "\nBanking Service Menu:\n";
        std::cout << "1. Create Customer\n";
        std::cout << "2. Create Account\n";
        std::cout << "3. Deposit\n";
        std::cout << "4. Withdraw\n";
        std::cout << "5. Transfer\n";
        std::cout << "6. View Account Info\n";
        std::cout << "7. View Customer Info\n";
        std::cout << "8. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        system(CLEAR);

        switch (choice) {
            case 1: {
                bank.createCustomer();
                break;
            }
            case 2: {
                int customerID;
                std::cout << "Enter Customer ID: ";
                std::cin >> customerID;
                Customer *customer = bank.findCustomer(customerID);
                if (customer) {
                    bank.createAccount(*customer);
                } else {
                    std::cout << "Customer not found.\n";
                }
                break;
            }
            case 3: {
                int customerID;
                std::cout << "Enter Customer ID: ";
                std::cin >> customerID;
                Customer *customer = bank.findCustomer(customerID);
                if (customer) {
                    bank.performDeposit(*customer);
                } else {
                    std::cout << "Customer not found.\n";
                }
                break;
            }
            case 4: {
                int customerID;
                std::cout << "Enter Customer ID: ";
                std::cin >> customerID;
                Customer *customer = bank.findCustomer(customerID);
                if (customer) {
                    bank.performWithdrawal(*customer);
                } else {
                    std::cout << "Customer not found.\n";
                }
                break;
            }
            case 5: {
                int fromCustomerID, toCustomerID;
                std::cout << "Enter From Customer ID: ";
                std::cin >> fromCustomerID;
                std::cout << "Enter To Customer ID: ";
                std::cin >> toCustomerID;
                Customer *fromCustomer = bank.findCustomer(fromCustomerID);
                Customer *toCustomer = bank.findCustomer(toCustomerID);
                if (fromCustomer && toCustomer) {
                    bank.performTransfer(*fromCustomer, *toCustomer);
                } else {
                    std::cout << "Customer not found.\n";
                }
                break;
            }
            case 6: {
                int customerID;
                std::cout << "Enter Customer ID: ";
                std::cin >> customerID;
                Customer *customer = bank.findCustomer(customerID);
                if (customer) {
                    bank.viewAccountInfo(*customer);
                } else {
                    std::cout << "Customer not found.\n";
                }
                break;
            }
            case 7: {
                int customerID;
                std::cout << "Enter Customer ID: ";
                std::cin >> customerID;
                Customer *customer = bank.findCustomer(customerID);
                if (customer) {
                    bank.viewCustomerInfo(*customer);
                } else {
                    std::cout << "Customer not found.\n";
                }
                break;
            }
            case 8: {
                std::cout << "Exiting...\n";
                return 0;
            }
            default: {
                std::cout << "Invalid choice. Please try again.\n";
                break;
            }
        }
        
        std::cout << "\nPress Enter to continue...";
        std::cin.ignore();
        std::cin.get();
        system(CLEAR);
    }

    return 0;
}
