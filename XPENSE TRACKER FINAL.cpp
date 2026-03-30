#include <iostream>
#include <vector>
using namespace std;

// Constants
const int MAX_FRIENDS = 50;

// Globals for Loan Manager
string borrowerNames[MAX_FRIENDS];
float amountsOwed[MAX_FRIENDS] = {0};
string sourceOfLoan[MAX_FRIENDS];  // "savings", "core", "fun"
int friendCount = 0;

// --- Loan Manager: to handle the loans lending and recieving ---
void lendMoney(float &userSavings, float &fun_quota, float &core_expense) {
    string borrowerName;
    float loanAmount;
    string isUrgent;

    cout << "Who are you lending money to? ";
    cin >> borrowerName;
    cout << "How much would you like to lend? Rs. ";
    cin >> loanAmount;

    string loanSource = "savings";  // our first priority is to lend money from our savings

    if (loanAmount > userSavings) {
        cout << "Oops! Buddy you do not have enough savings available";

        cout << "Does your friend really need that money? Should we use Fun or Core funds instead? (yes/no): "<<endl;
        cin >> isUrgent;

        if (isUrgent == "yes" || isUrgent == "YES") {
            if (loanAmount <= fun_quota) {
                fun_quota -= loanAmount;
                loanSource = "fun";
            } else {
                cout << "Use Core expenses instead? (yes/no): ";
                cin >> isUrgent;
                if (isUrgent == "yes" || isUrgent == "YES") {
                    if (loanAmount <= core_expense) {
                        core_expense -= loanAmount;
                        loanSource = "core";
                    } else {
                        cout << "Sorry, not enough in Core expenses either.";
                        return;
                    }
                } else {
                    cout << "Loan process cancelled.";
                    return;
                }
            }
        } else {
            cout << "Alright, no loan given";
            return;
        }
    } else {
        userSavings -= loanAmount;
        
    }

    // to  Check if the borrower already owes you money
    bool found = false;
    for (int i = 0; i < friendCount; i++) {
        if (borrowerNames[i] == borrowerName) {
            amountsOwed[i] += loanAmount;
            found = true;
            break;
        }
    }

    if (!found && friendCount < MAX_FRIENDS) {
        borrowerNames[friendCount] = borrowerName;
        amountsOwed[friendCount] = loanAmount;
        sourceOfLoan[friendCount] = loanSource;
        friendCount++;
    }

    cout << "You’ve successfully lent Rs. " << loanAmount << " to  your friend" << borrowerName << " from " << loanSource << " funds.\n";

}

void receiveMoney(float &userSavings, float &fun_quota, float &core_expense) {
    string borrowerName;
    float loanAmount;

    cout << "who's' returning money: ";
    cin >> borrowerName;

    for (int i = 0; i < friendCount; i++) {
        if (borrowerNames[i] == borrowerName) {
            if (amountsOwed[i] <= 0) {
                cout << borrowerName << " doesn't owe you anything.\n";
                return;
            }
        // Add returned amount to the correct category    

            cout << "Amount owed by " << borrowerName << ": Rs. " << amountsOwed[i] << "\n";
            cout << "How much amount is being returned? ";
            cin >> loanAmount;

            if (loanAmount > amountsOwed[i]) {
                cout << "That's more than they even owe you! \n";
                return;
            }

            if (sourceOfLoan[i] == "core") core_expense += loanAmount;
            else if (sourceOfLoan[i] == "fun") fun_quota += loanAmount;
            else userSavings += loanAmount;

            amountsOwed[i] -= loanAmount;
            
// If loan fully paid, remove entry
            if (amountsOwed[i] == 0) {
                for (int j = i; j < friendCount - 1; j++) {
                    borrowerNames[j] = borrowerNames[j + 1];
                    amountsOwed[j] = amountsOwed[j + 1];
                    sourceOfLoan[j] = sourceOfLoan[j + 1];
                }
                friendCount--;
                cout << "Loan fully repaid. Removed " << borrowerName << ".\n";
            } else {
                cout << "Partial repayment. " << borrowerName << " still owes Rs. " << amountsOwed[i] << ".\n";
            }
            return;
        }
    }

    cout << "No record found for " << borrowerName << ".\n";
}

void viewAllLoans() {
    if (friendCount == 0) {
        cout << "Nice no one owes you anything! \n";
        return;
    }

    cout << "Here’s a list of who owes you what:\n-----------------------\n";
    for (int i = 0; i < friendCount; i++) {
        cout << i + 1 << ". " << borrowerNames[i] << " owes Rs. " << amountsOwed[i] << " (from " << sourceOfLoan[i] << ")\n";
    }
    cout << "-----------------------\n";
}

// Goal Planner
void goalPlanner() {
    double income = 0;
    const int maxExpenses = 100;
    double expenses[maxExpenses];
    string expenseNames[maxExpenses];
    int expenseCount = 0;
    double totalExpenses = 0;
    double remaining = 0;
    double goalPrice = 0;
    string goalName;
//getting income and expenses 
    cout << "What's your total income for this month? Rs. ";
    cin >> income;
    if (income <= 0) {
        cout << "Invalid input. Income must be positive.\n";
        return;
    }

    cout << "Let's list your core expenses. Type -1 when done.";
    double expense;
    while (expenseCount < maxExpenses) {
        cout << "Name of the expense: ";
        cin >> expenseNames[expenseCount];
        cout << "Amount for this expense: Rs. ";
        cin >> expense;
        if (expense == -1) break;
        if (expense < 0) {
            cout << "Invalid input.\n";
            continue;
        }
        expenses[expenseCount++] = expense;
        totalExpenses += expense;
    }
//to illustrate the expenses
    cout << "Here’s how your spending looks like:";


    for (int i = 0; i < expenseCount; i++) {
        cout << " - " << expenseNames[i] << ": Rs. " << expenses[i] << "\n";
    }

    remaining = income - totalExpenses;
    if (remaining < 0) {
        cout << "Warning: Expenses exceed income.\n";
        return;
    }
    cout << "You have Rs. " << remaining << "\n";
    
	cout << "Got a goal in mind? What are you saving for? ";
    cin >> goalName;
    cout << "How much do you need to save for it? Rs. ";
    cin >> goalPrice;

    if (goalPrice <= 0) {
        cout << "Invalid goal price.\n";
        return;
    }

    if (remaining == 0) {
        cout << "Looks like you’ve got nothing left to save this month.";

    } else {
        int months = (goalPrice + remaining - 1) / remaining;
        cout << "You’ll reach '" << goalName << "' in " << months << " month(s). Keep saving!\n";
    }
}

// Bill Splitter
void splitBill(float &funQuota, float &savings) {
    float totalBill;
    int numFriends;

    cout << "Alright, what's the total fare for the bill?";
    cin >> totalBill;
    cout << "Including you, how many people are chipping in for the ride? ";
    cin >> numFriends;

    if (numFriends <= 0) {
        cout << "Invalid number.\n";
        return;
    }

    vector<string> names(numFriends);
    for (int i = 0; i < numFriends; i++) {
        cout << "Name of person"  << i + 1 << ": ";
        cin >> names[i];
    }

    float share = totalBill / numFriends;
    cout << "Each one of you should pay: Rs. " << share << endl;

    string yourName;
    cout << "What’s your name? ";
    cin >> yourName;

    cout << "Let’s process your part of the payment...";
// to get the money from saving if not in fun quota
    if (funQuota >= share) {
        funQuota -= share;
        cout << "Paid from Fun Quota.\n";
    } else {
        float remaining = share - funQuota;
        if (savings >= remaining) {
            cout << "Your Fun Quota wasn’t enough. Used Rs. " << funQuota << " + Rs " << remaining << " from Savings.\n";
            funQuota = 0;
            savings -= remaining;
        } else {
            cout << "You don’t have enough funds to cover your part.";

            return;
        }
    }

    cout << "--- Updated Balances ---\nFun Quota: Rs " << funQuota << "\nSavings: Rs " << savings << endl;
}

// Expense Tracker
void expenseTracker() {
    float savings = 0, funQuota, remainingFunQuota;
    float dailySavings[31] = {0};
    int currentDay = 0;

    cout << "How much do you have saved up so far? Rs.";
    cin >> savings;
    cout << "What's your monthly fun budget? Rs. ";
    cin >> funQuota;
    remainingFunQuota = funQuota;

    while (true) {
// prompts to ask user       
 cout << "=== Daily Expense Tracker ===";

        cout << "1. Record Daily Savings\n2. Record Fun Spending\n3. Show Status\n4. End Month\n5. Exit to Main Menu\nEnter choice: ";
        int choice;
        cin >> choice;

        if (choice == 1) {
            if (currentDay >= 31) {
                cout << "Month complete.\n";
                continue;
            }
            char transport, lunch;
            float todaySaving = 0;
            cout << "Did you save on transport today? (y/n): ";
            cin >> transport;
            if (transport == 'y' || transport == 'Y') todaySaving += 5;
            cout << "Did you bring lunch from home? (y/n): ";
            cin >> lunch;
            if (lunch == 'y' || lunch == 'Y') todaySaving += 10;
            if (todaySaving > 0) {
                dailySavings[currentDay] = todaySaving;
                savings += todaySaving;
                cout << "Saved $" << todaySaving << endl;
            }
            currentDay++;
        } else if (choice == 2) {
            float amt;
            cout << "Enter spending: $";
            cin >> amt;
            if (amt <= remainingFunQuota) {
                remainingFunQuota -= amt;
                cout << "Got it! Your spending is recorded.";

            } else {
                cout << "This is over your quota. Do you want to continue anyway? (y/n): ";
                char opt;
                cin >> opt;
                if (opt == 'y' || opt == 'Y') remainingFunQuota -= amt;
            }
        } else if (choice == 3) {
            cout << "Current day: " << currentDay + 1 << "\nSavings: $" << savings << "\nRemaining Fun Quota: $" << remainingFunQuota << "\n";
        } else if (choice == 4) {
            savings += remainingFunQuota;
            remainingFunQuota = funQuota;
            currentDay = 0;
            for (int i = 0; i < 31; i++) dailySavings[i] = 0;
            cout << "End of the month! Unused Fun Quota moved to savings.";

        } else if (choice == 5) break;
        else cout << "Oops, that's not a valid choice. Try again.";

    }
}

// Main Program
int main() {
    float savings = 1000.0, funQuota = 300.0, coreExpense = 500.0;

    while (true) {
        cout << "====== XPense Tracker: Main Menu ======"<<endl;


        cout << "1. Loan Manager\n2. Goal Planner\n3. Bill Splitter\n4. Expense Tracker\n5. View Balances\n6. Exit\nEnter choice: ";
        int choice;
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "\n--- Loan Manager ---\n";
                int subChoice;
                cout << "1. Lend Money\n2. Receive Money\n3. View Loans\nEnter choice: ";
                cin >> subChoice;
                if (subChoice == 1) lendMoney(savings, funQuota, coreExpense);
                else if (subChoice == 2) receiveMoney(savings, funQuota, coreExpense);
                else if (subChoice == 3) viewAllLoans();
                break;
            case 2:
                goalPlanner();
                break;
            case 3:
                splitBill(funQuota, savings);
                break;
            case 4:
                expenseTracker();
                break;
            case 5:
                cout << "--- Your Balance---"<<endl;

cout<<"Savings: Rs. " << savings << "\nFun Quota: Rs " << funQuota << "\nCore Expenses: Rs " << coreExpense << endl;
                break;
            case 6:
                cout << "Thanks for using Budget Buddy. Goodbye!";

                return 0;
            default:
                cout << "Invalid option.\n";
        }
    }

    return 0;
}
//finalll


