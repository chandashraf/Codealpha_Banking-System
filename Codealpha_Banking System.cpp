#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

// 1. Transaction Class
// Transaction history ko store aur display karne ke liye
class Transaction
{
private:
    string type;
    double amount;

public:
    Transaction(string t, double amt)
        : type(t), amount(amt)
    {
    }

    void display() const
    {
        cout << left << setw(35) << type
             << " | Amount: $" << fixed << setprecision(2)
             << amount << endl;
    }
};


// 2. Account Class
// Account number, balance aur transactions manage karne ke liye
class Account
{
private:
    int accountNumber;
    double balance;
    vector<Transaction> history;

public:

    // Default Constructor
    Account()
        : accountNumber(0), balance(0.0)
    {
    }

    // Parameterized Constructor
    Account(int accNo, double initialBalance)
        : accountNumber(accNo), balance(initialBalance)
    {
        if (initialBalance > 0)
        {
            history.push_back(
                Transaction("Initial Deposit", initialBalance)
            );
        }
    }

    int getAccountNumber() const
    {
        return accountNumber;
    }

    double getBalance() const
    {
        return balance;
    }


    // Deposit Money
    void deposit(double amount)
    {
        if (amount <= 0)
        {
            cout << "[Error] Deposit amount must be positive.\n";
            return;
        }

        balance += amount;

        history.push_back(
            Transaction("Deposit", amount)
        );

        cout << fixed << setprecision(2);
        cout << "[Success] Deposited $" << amount
             << ". New Balance: $" << balance << endl;
    }


    // Withdraw Money
    bool withdraw(double amount)
    {
        if (amount <= 0)
        {
            cout << "[Error] Withdrawal amount must be positive.\n";
            return false;
        }

        if (amount > balance)
        {
            cout << "[Error] Insufficient balance!\n";
            return false;
        }

        balance -= amount;

        history.push_back(
            Transaction("Withdrawal", amount)
        );

        cout << fixed << setprecision(2);
        cout << "[Success] Withdrawn $" << amount
             << ". New Balance: $" << balance << endl;

        return true;
    }


    // Transfer Money
    bool transfer(Account &receiver, double amount)
    {
        if (amount <= 0)
        {
            cout << "[Error] Transfer amount must be positive.\n";
            return false;
        }

        if (amount > balance)
        {
            cout << "[Error] Insufficient balance!\n";
            return false;
        }

        // Sender balance decrease
        balance -= amount;

        // Receiver balance increase
        receiver.balance += amount;

        // Sender transaction
        history.push_back(
            Transaction(
                "Transfer Out (Acc: "
                + to_string(receiver.accountNumber) + ")",
                amount
            )
        );

        // Receiver transaction
        receiver.history.push_back(
            Transaction(
                "Transfer In (Acc: "
                + to_string(accountNumber) + ")",
                amount
            )
        );

        cout << fixed << setprecision(2);
        cout << "[Success] Transferred $" << amount
             << " to Account #"
             << receiver.getAccountNumber() << endl;

        return true;
    }


    // Display Transaction History
    void displayHistory() const
    {
        cout << "\n--- Transaction History for Account #"
             << accountNumber << " ---\n";

        if (history.empty())
        {
            cout << "No transactions yet.\n";
            return;
        }

        for (const auto &tx : history)
        {
            tx.display();
        }
    }
};


// 3. Customer Class
// Customer information manage karne ke liye
class Customer
{
private:
    int customerId;
    string name;
    Account account;

public:

    Customer(int id, string n, Account acc)
        : customerId(id), name(n), account(acc)
    {
    }

    int getId() const
    {
        return customerId;
    }

    string getName() const
    {
        return name;
    }

    Account& getAccount()
    {
        return account;
    }

    void displayCustomerInfo() const
    {
        cout << "\n--- Account Details ---\n";

        cout << "Customer ID : " << customerId << endl;
        cout << "Name        : " << name << endl;
        cout << "Account No  : "
             << account.getAccountNumber() << endl;

        cout << "Balance     : $"
             << fixed << setprecision(2)
             << account.getBalance() << endl;
    }
};


// 4. Bank Class
// Multiple customers ko manage karne ke liye
class Bank
{
private:
    vector<Customer> customers;

    int nextAccNo = 1001;
    int nextCustId = 1;

public:

    // Create New Customer Account
    void createCustomer(const string &name, double initialDeposit)
    {
        if (name.empty())
        {
            cout << "[Error] Customer name cannot be empty.\n";
            return;
        }

        if (initialDeposit < 0)
        {
            cout << "[Error] Initial deposit cannot be negative.\n";
            return;
        }

        int customerId = nextCustId++;
        int accountNumber = nextAccNo++;

        Account newAccount(accountNumber, initialDeposit);

        customers.emplace_back(
            customerId,
            name,
            newAccount
        );

        cout << "\n[Success] Customer registered successfully!\n";

        cout << "Assigned Customer ID: "
             << customerId << endl;

        cout << "Account Number      : "
             << accountNumber << endl;
    }


    // Find Customer using Account Number
    Customer* findCustomerByAccount(int accNo)
    {
        for (auto &customer : customers)
        {
            if (customer.getAccount().getAccountNumber() == accNo)
            {
                return &customer;
            }
        }

        return nullptr;
    }
};


// Main Function
int main()
{
    Bank bank;
    int choice;

    while (true)
    {
        cout << "\n==============================\n";
        cout << "        BANKING SYSTEM\n";
        cout << "==============================\n";

        cout << "1. Create New Account\n";
        cout << "2. Deposit Money\n";
        cout << "3. Withdraw Money\n";
        cout << "4. Transfer Funds\n";
        cout << "5. View Account Details\n";
        cout << "6. View Transaction History\n";
        cout << "7. Exit\n";

        cout << "Enter your choice (1-7): ";
        cin >> choice;


        // Exit
        if (choice == 7)
        {
            cout << "\nThank you for using the Banking System!\n";
            break;
        }


        switch (choice)
        {
            // --------------------------------
            // 1. Create Account
            // --------------------------------
            case 1:
            {
                string name;
                double deposit;

                cout << "Enter Customer Name: ";

                cin.ignore();
                getline(cin, name);

                cout << "Enter Initial Deposit: $";
                cin >> deposit;

                bank.createCustomer(name, deposit);

                break;
            }


            // --------------------------------
            // 2. Deposit
            // --------------------------------
            case 2:
            {
                int accNo;
                double amount;

                cout << "Enter Account Number: ";
                cin >> accNo;

                Customer* customer =
                    bank.findCustomerByAccount(accNo);

                if (customer)
                {
                    cout << "Enter Deposit Amount: $";
                    cin >> amount;

                    customer->getAccount().deposit(amount);
                }
                else
                {
                    cout << "[Error] Account not found!\n";
                }

                break;
            }


            // --------------------------------
            // 3. Withdraw
            // --------------------------------
            case 3:
            {
                int accNo;
                double amount;

                cout << "Enter Account Number: ";
                cin >> accNo;

                Customer* customer =
                    bank.findCustomerByAccount(accNo);

                if (customer)
                {
                    cout << "Enter Withdrawal Amount: $";
                    cin >> amount;

                    customer->getAccount().withdraw(amount);
                }
                else
                {
                    cout << "[Error] Account not found!\n";
                }

                break;
            }


            // --------------------------------
            // 4. Transfer
            // --------------------------------
            case 4:
            {
                int senderAcc;
                int receiverAcc;
                double amount;

                cout << "Enter Your Account Number: ";
                cin >> senderAcc;

                cout << "Enter Receiver Account Number: ";
                cin >> receiverAcc;

                Customer* sender =
                    bank.findCustomerByAccount(senderAcc);

                Customer* receiver =
                    bank.findCustomerByAccount(receiverAcc);


                if (!sender || !receiver)
                {
                    cout << "[Error] One or both accounts "
                         << "do not exist!\n";
                }
                else if (senderAcc == receiverAcc)
                {
                    cout << "[Error] Cannot transfer funds "
                         << "to the same account!\n";
                }
                else
                {
                    cout << "Enter Transfer Amount: $";
                    cin >> amount;

                    sender->getAccount().transfer(
                        receiver->getAccount(),
                        amount
                    );
                }

                break;
            }


            // --------------------------------
            // 5. Account Details
            // --------------------------------
            case 5:
            {
                int accNo;

                cout << "Enter Account Number: ";
                cin >> accNo;

                Customer* customer =
                    bank.findCustomerByAccount(accNo);

                if (customer)
                {
                    customer->displayCustomerInfo();
                }
                else
                {
                    cout << "[Error] Account not found!\n";
                }

                break;
            }


            // --------------------------------
            // 6. Transaction History
            // --------------------------------
            case 6:
            {
                int accNo;

                cout << "Enter Account Number: ";
                cin >> accNo;

                Customer* customer =
                    bank.findCustomerByAccount(accNo);

                if (customer)
                {
                    customer->getAccount().displayHistory();
                }
                else
                {
                    cout << "[Error] Account not found!\n";
                }

                break;
            }


            // --------------------------------
            // Invalid Choice
            // --------------------------------
            default:
            {
                cout << "[Error] Invalid option. "
                     << "Please choose between 1 and 7.\n";
            }
        }
    }

    return 0;
}