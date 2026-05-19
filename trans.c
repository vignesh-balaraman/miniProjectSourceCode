// Enhanced Bank Transaction Processing System
// Features: File I/O, Arrays, Searching, Sorting, Pointers, Error Handling, New Banking Features
// Courses: CO1 (Compilation, Debugging), CO2 (Arrays, Searching, Sorting), CO3 (Functions, Pointers),
//          CO4 (Structures, Features), CO5 (File I/O, Optimization, Error Handling)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_ACCOUNTS 100
#define ACCOUNT_FILE "credit.dat"
#define TEXT_FILE "accounts.txt"
#define MIN_BALANCE 100.0

// Enhanced structure definition with additional fields
struct clientData
{
    unsigned int acctNum;        // account number
    char lastName[15];           // account last name
    char firstName[10];          // account first name
    double balance;              // account balance
    char mobileNumber[11];       // mobile number (10 digits + null)
    char accountType[10];        // account type (Saving/Current)
};

// Function Prototypes
unsigned int enterChoice(void);
void initializeAccountFile(FILE **fPtr);
void displayMenu(void);
int validateAccountNumber(unsigned int acctNum);
int validateBalance(double balance);
void textFile(FILE *readPtr);
void updateRecord(FILE *fPtr);
void newRecord(FILE *fPtr);
void deleteRecord(FILE *fPtr);
void searchByAccountNumber(FILE *fPtr);
void searchByName(FILE *fPtr);
void displayAllAccounts(FILE *fPtr);
void sortAccountsByNumber(FILE *fPtr);
void sortAccountsByBalance(FILE *fPtr);
void miniStatement(FILE *fPtr);
void balanceTransfer(FILE *fPtr);
void selectionSortByField(struct clientData *accounts, int count, int sortBy);
void displayAccountRecord(struct clientData *client);
void readAccountRecord(FILE *fPtr, unsigned int acctNum, struct clientData *client);
void writeAccountRecord(FILE *fPtr, unsigned int acctNum, struct clientData *client);
int isValidMobileNumber(const char *mobile);
void flushInputBuffer(void);

// Main Program
int main(int argc, char *argv[])
{
    FILE *cfPtr = NULL;          // credit.dat file pointer
    unsigned int choice;          // user's choice

    printf("\n========================================\n");
    printf("   BANK TRANSACTION PROCESSING SYSTEM\n");
    printf("========================================\n\n");

    // Initialize file - create if not exists
    initializeAccountFile(&cfPtr);

    if (cfPtr == NULL)
    {
        printf("FATAL ERROR: Could not initialize account file.\n");
        return -1;
    }

    // Main menu loop
    while ((choice = enterChoice()) != 11)
    {
        switch (choice)
        {
        case 1:
            textFile(cfPtr);
            break;
        case 2:
            updateRecord(cfPtr);
            break;
        case 3:
            newRecord(cfPtr);
            break;
        case 4:
            deleteRecord(cfPtr);
            break;
        case 5:
            searchByAccountNumber(cfPtr);
            break;
        case 6:
            searchByName(cfPtr);
            break;
        case 7:
            displayAllAccounts(cfPtr);
            break;
        case 8:
            sortAccountsByNumber(cfPtr);
            break;
        case 9:
            miniStatement(cfPtr);
            break;
        case 10:
            balanceTransfer(cfPtr);
            break;
        case 11:
            printf("\nThank you for using Bank System. Goodbye!\n\n");
            break;
        default:
            printf("ERROR: Invalid choice. Please select 1-11.\n");
            break;
        }
    }

    fclose(cfPtr);
    return 0;
}

// Initialize account file - create with blank records if not exists
void initializeAccountFile(FILE **fPtr)
{
    struct clientData blankClient = {0, "", "", 0.0, "", ""};

    // Try to open existing file
    *fPtr = fopen(ACCOUNT_FILE, "rb+");

    if (*fPtr == NULL)
    {
        printf("INFO: Account file not found. Creating new file with 100 blank records...\n");
        *fPtr = fopen(ACCOUNT_FILE, "wb+");

        if (*fPtr == NULL)
        {
            printf("ERROR: Could not create account file.\n");
            return;
        }

        // Write 100 blank records
        for (int i = 0; i < MAX_ACCOUNTS; i++)
        {
            if (fwrite(&blankClient, sizeof(struct clientData), 1, *fPtr) != 1)
            {
                printf("ERROR: Failed to write blank record at position %d.\n", i);
                fclose(*fPtr);
                *fPtr = NULL;
                return;
            }
        }

        printf("INFO: Successfully created file with %d blank records.\n\n", MAX_ACCOUNTS);
        rewind(*fPtr);
    }
    else
    {
        printf("INFO: Existing account file loaded successfully.\n\n");
    }
}

// Display main menu
void displayMenu(void)
{
    printf("\n========================================\n");
    printf("          MAIN MENU\n");
    printf("========================================\n");
    printf("1  - Export accounts to text file (accounts.txt)\n");
    printf("2  - Update existing account balance\n");
    printf("3  - Add new account\n");
    printf("4  - Delete account\n");
    printf("5  - Search account by account number\n");
    printf("6  - Search account by customer name\n");
    printf("7  - Display all accounts (tabular format)\n");
    printf("8  - Sort accounts by account number\n");
    printf("9  - Mini statement (recent transaction)\n");
    printf("10 - Balance transfer between accounts\n");
    printf("11 - Exit program\n");
    printf("========================================\n");
    printf("Enter your choice (1-11): ");
}

// Input validation for account number
int validateAccountNumber(unsigned int acctNum)
{
    if (acctNum < 1 || acctNum > MAX_ACCOUNTS)
    {
        printf("ERROR: Account number must be between 1 and %d.\n", MAX_ACCOUNTS);
        return 0;
    }
    return 1;
}

// Input validation for balance
int validateBalance(double balance)
{
    if (balance < MIN_BALANCE)
    {
        printf("ERROR: Minimum balance must be %.2f\n", MIN_BALANCE);
        return 0;
    }
    return 1;
}

// Validate mobile number
int isValidMobileNumber(const char *mobile)
{
    if (strlen(mobile) != 10)
        return 0;
    for (int i = 0; i < 10; i++)
    {
        if (!isdigit(mobile[i]))
            return 0;
    }
    return 1;
}

// Flush input buffer - removes leftover characters in stdin
void flushInputBuffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

// Read single account record using pointer parameter
void readAccountRecord(FILE *fPtr, unsigned int acctNum, struct clientData *client)
{
    if (!validateAccountNumber(acctNum))
    {
        client->acctNum = 0;
        return;
    }

    fseek(fPtr, (acctNum - 1) * sizeof(struct clientData), SEEK_SET);

    if (fread(client, sizeof(struct clientData), 1, fPtr) != 1)
    {
        printf("ERROR: Failed to read record at account number %d.\n", acctNum);
        client->acctNum = 0;
    }
}

// Write single account record using pointer parameter
void writeAccountRecord(FILE *fPtr, unsigned int acctNum, struct clientData *client)
{
    if (!validateAccountNumber(acctNum))
        return;

    fseek(fPtr, (acctNum - 1) * sizeof(struct clientData), SEEK_SET);

    if (fwrite(client, sizeof(struct clientData), 1, fPtr) != 1)
    {
        printf("ERROR: Failed to write record for account %d.\n", acctNum);
    }
    else
    {
        fflush(fPtr); // Ensure data is written to disk
    }
}

// Display single account record in formatted manner
void displayAccountRecord(struct clientData *client)
{
    printf("Acct #    | Last Name       | First Name | Mobile     | Type     | Balance\n");
    printf("----------|-----------------|------------|------------|----------|----------\n");
    printf("%-9u | %-15s | %-10s | %-10s | %-8s | %.2f\n",
           client->acctNum, client->lastName, client->firstName,
           client->mobileNumber, client->accountType, client->balance);
}

// Create formatted text file for printing
void textFile(FILE *readPtr)
{
    FILE *writePtr;
    struct clientData client = {0, "", "", 0.0, "", ""};

    if ((writePtr = fopen(TEXT_FILE, "w")) == NULL)
    {
        printf("ERROR: Could not open %s for writing.\n", TEXT_FILE);
        return;
    }

    rewind(readPtr);
    fprintf(writePtr, "%-9s | %-15s | %-10s | %-10s | %-8s | %-10s\n",
            "Acct#", "Last Name", "First Name", "Mobile", "Type", "Balance");
    fprintf(writePtr, "----------|-----------------|------------|------------|----------|----------\n");

    int recordCount = 0;
    while (fread(&client, sizeof(struct clientData), 1, readPtr) == 1)
    {
        if (client.acctNum != 0)
        {
            fprintf(writePtr, "%-9u | %-15s | %-10s | %-10s | %-8s | %.2f\n",
                    client.acctNum, client.lastName, client.firstName,
                    client.mobileNumber, client.accountType, client.balance);
            recordCount++;
        }
    }

    fprintf(writePtr, "\nTotal Records: %d\n", recordCount);
    fclose(writePtr);
    printf("SUCCESS: Exported %d accounts to %s\n", recordCount, TEXT_FILE);
}

// Update balance in existing record
void updateRecord(FILE *fPtr)
{
    unsigned int acctNum;
    double transaction;
    struct clientData client = {0, "", "", 0.0, "", ""};

    printf("\n--- UPDATE ACCOUNT BALANCE ---\n");
    printf("Enter account number to update (1-%d): ", MAX_ACCOUNTS);
    if (scanf("%u", &acctNum) != 1)
    {
        printf("ERROR: Invalid input.\n");
        flushInputBuffer();
        return;
    }
    flushInputBuffer();

    readAccountRecord(fPtr, acctNum, &client);

    if (client.acctNum == 0)
    {
        printf("ERROR: Account #%u does not exist.\n", acctNum);
        return;
    }

    printf("\nCurrent Account Details:\n");
    displayAccountRecord(&client);

    printf("\nEnter transaction amount (positive for deposit, negative for withdrawal): ");
    if (scanf("%lf", &transaction) != 1)
    {
        printf("ERROR: Invalid input.\n");
        flushInputBuffer();
        return;
    }
    flushInputBuffer();

    double newBalance = client.balance + transaction;

    // Error handling: check minimum balance
    if (newBalance < MIN_BALANCE)
    {
        printf("ERROR: Insufficient balance. Transaction would result in %.2f\n", newBalance);
        printf("       Minimum balance required: %.2f\n", MIN_BALANCE);
        return;
    }

    client.balance = newBalance;
    writeAccountRecord(fPtr, acctNum, &client);

    printf("\nAccount Updated Successfully:\n");
    displayAccountRecord(&client);
}

// Delete existing account record
void deleteRecord(FILE *fPtr)
{
    unsigned int acctNum;
    struct clientData client = {0, "", "", 0.0, "", ""};
    struct clientData blankClient = {0, "", "", 0.0, "", ""};

    printf("\n--- DELETE ACCOUNT ---\n");
    printf("Enter account number to delete (1-%d): ", MAX_ACCOUNTS);
    if (scanf("%u", &acctNum) != 1)
    {
        printf("ERROR: Invalid input.\n");
        flushInputBuffer();
        return;
    }
    flushInputBuffer();

    readAccountRecord(fPtr, acctNum, &client);

    if (client.acctNum == 0)
    {
        printf("ERROR: Account #%u does not exist.\n", acctNum);
        return;
    }

    printf("\nAccount to be deleted:\n");
    displayAccountRecord(&client);

    printf("\nConfirm deletion? (y/n): ");
    char confirm;
    if (scanf("%c", &confirm) != 1)
    {
        printf("ERROR: Invalid input.\n");
        flushInputBuffer();
        return;
    }
    flushInputBuffer();

    if (confirm != 'y' && confirm != 'Y')
    {
        printf("Deletion cancelled.\n");
        return;
    }

    writeAccountRecord(fPtr, acctNum, &blankClient);
    printf("Account #%u DELETED successfully.\n", acctNum);
}

// Create new account record
void newRecord(FILE *fPtr)
{
    struct clientData client = {0, "", "", 0.0, "", ""};
    unsigned int acctNum;

    printf("\n--- ADD NEW ACCOUNT ---\n");
    printf("Enter new account number (1-%d): ", MAX_ACCOUNTS);
    if (scanf("%u", &acctNum) != 1)
    {
        printf("ERROR: Invalid input.\n");
        flushInputBuffer();
        return;
    }
    flushInputBuffer();

    if (!validateAccountNumber(acctNum))
        return;

    readAccountRecord(fPtr, acctNum, &client);

    if (client.acctNum != 0)
    {
        printf("ERROR: Account #%u already exists.\n", acctNum);
        return;
    }

    printf("Enter last name: ");
    if (fgets(client.lastName, sizeof(client.lastName), stdin) == NULL)
    {
        printf("ERROR: Failed to read last name.\n");
        return;
    }
    client.lastName[strcspn(client.lastName, "\n")] = '\0';

    printf("Enter first name: ");
    if (fgets(client.firstName, sizeof(client.firstName), stdin) == NULL)
    {
        printf("ERROR: Failed to read first name.\n");
        return;
    }
    client.firstName[strcspn(client.firstName, "\n")] = '\0';

    printf("Enter mobile number (10 digits): ");
    if (fgets(client.mobileNumber, sizeof(client.mobileNumber), stdin) == NULL)
    {
        printf("ERROR: Failed to read mobile number.\n");
        return;
    }
    client.mobileNumber[strcspn(client.mobileNumber, "\n")] = '\0';

    if (!isValidMobileNumber(client.mobileNumber))
    {
        printf("ERROR: Invalid mobile number. Must be 10 digits.\n");
        return;
    }

    printf("Enter account type (Saving/Current): ");
    if (fgets(client.accountType, sizeof(client.accountType), stdin) == NULL)
    {
        printf("ERROR: Failed to read account type.\n");
        return;
    }
    client.accountType[strcspn(client.accountType, "\n")] = '\0';

    printf("Enter opening balance (minimum %.2f): ", MIN_BALANCE);
    if (scanf("%lf", &client.balance) != 1)
    {
        printf("ERROR: Invalid balance input.\n");
        flushInputBuffer();
        return;
    }
    flushInputBuffer();

    if (!validateBalance(client.balance))
        return;

    client.acctNum = acctNum;
    writeAccountRecord(fPtr, acctNum, &client);

    printf("\nNEW ACCOUNT CREATED SUCCESSFULLY:\n");
    displayAccountRecord(&client);
}

// Search account by account number
void searchByAccountNumber(FILE *fPtr)
{
    unsigned int acctNum;
    struct clientData client = {0, "", "", 0.0, "", ""};

    printf("\n--- SEARCH BY ACCOUNT NUMBER ---\n");
    printf("Enter account number to search (1-%d): ", MAX_ACCOUNTS);
    if (scanf("%u", &acctNum) != 1)
    {
        printf("ERROR: Invalid input.\n");
        flushInputBuffer();
        return;
    }
    flushInputBuffer();

    readAccountRecord(fPtr, acctNum, &client);

    if (client.acctNum == 0)
    {
        printf("ERROR: Account #%u not found.\n", acctNum);
        return;
    }

    printf("\nAccount Found:\n");
    displayAccountRecord(&client);
}

// Search accounts by customer name (Linear search with partial match)
void searchByName(FILE *fPtr)
{
    char searchName[25];
    struct clientData client = {0, "", "", 0.0, "", ""};
    int found = 0;

    printf("\n--- SEARCH BY CUSTOMER NAME ---\n");
    printf("Enter customer name (or part of name): ");
    if (fgets(searchName, sizeof(searchName), stdin) == NULL)
    {
        printf("ERROR: Failed to read name.\n");
        return;
    }
    searchName[strcspn(searchName, "\n")] = '\0';

    rewind(fPtr);

    printf("\nSearch Results for \"%s\":\n", searchName);
    printf("%-9s | %-15s | %-10s | %-10s | %-8s | %-10s\n",
           "Acct#", "Last Name", "First Name", "Mobile", "Type", "Balance");
    printf("----------|-----------------|------------|------------|----------|----------\n");

    while (fread(&client, sizeof(struct clientData), 1, fPtr) == 1)
    {
        if (client.acctNum != 0)
        {
            // Search in both first and last name (case-insensitive)
            char fullName[30];
            sprintf(fullName, "%s %s", client.firstName, client.lastName);

            // Simple substring search (case-insensitive would require strcasecmp)
            if (strstr(client.lastName, searchName) || strstr(client.firstName, searchName))
            {
                printf("%-9u | %-15s | %-10s | %-10s | %-8s | %.2f\n",
                       client.acctNum, client.lastName, client.firstName,
                       client.mobileNumber, client.accountType, client.balance);
                found++;
            }
        }
    }

    if (found == 0)
    {
        printf("\nNo accounts found matching \"%s\".\n", searchName);
    }
    else
    {
        printf("\nTotal matches found: %d\n", found);
    }
}

// Display all accounts in tabular format
void displayAllAccounts(FILE *fPtr)
{
    struct clientData client = {0, "", "", 0.0, "", ""};
    int count = 0;

    printf("\n--- ALL ACCOUNTS (TABULAR FORMAT) ---\n");
    printf("%-9s | %-15s | %-10s | %-10s | %-8s | %-10s\n",
           "Acct#", "Last Name", "First Name", "Mobile", "Type", "Balance");
    printf("----------|-----------------|------------|------------|----------|----------\n");

    rewind(fPtr);

    while (fread(&client, sizeof(struct clientData), 1, fPtr) == 1)
    {
        if (client.acctNum != 0)
        {
            printf("%-9u | %-15s | %-10s | %-10s | %-8s | %.2f\n",
                   client.acctNum, client.lastName, client.firstName,
                   client.mobileNumber, client.accountType, client.balance);
            count++;
        }
    }

    printf("----------|-----------------|------------|------------|----------|----------\n");
    printf("Total Accounts: %d\n", count);
}

// Selection sort for array of accounts - used by sort functions
void selectionSortByField(struct clientData *accounts, int count, int sortBy)
{
    // sortBy: 0 = account number, 1 = balance
    for (int i = 0; i < count - 1; i++)
    {
        int minIdx = i;
        for (int j = i + 1; j < count; j++)
        {
            int shouldSwap = 0;

            if (sortBy == 0)
            {
                // Sort by account number
                if (accounts[j].acctNum < accounts[minIdx].acctNum)
                    shouldSwap = 1;
            }
            else if (sortBy == 1)
            {
                // Sort by balance (descending)
                if (accounts[j].balance > accounts[minIdx].balance)
                    shouldSwap = 1;
            }

            if (shouldSwap)
                minIdx = j;
        }

        // Swap if needed
        if (minIdx != i)
        {
            struct clientData temp = accounts[i];
            accounts[i] = accounts[minIdx];
            accounts[minIdx] = temp;
        }
    }
}

// Sort accounts by account number (using selection sort)
void sortAccountsByNumber(FILE *fPtr)
{
    struct clientData client = {0, "", "", 0.0, "", ""};
    struct clientData accounts[MAX_ACCOUNTS];
    int count = 0;

    printf("\n--- SORT ACCOUNTS BY ACCOUNT NUMBER ---\n");

    rewind(fPtr);

    // Read all active accounts into memory
    while (fread(&client, sizeof(struct clientData), 1, fPtr) == 1)
    {
        if (client.acctNum != 0)
        {
            accounts[count++] = client;
        }
    }

    if (count == 0)
    {
        printf("No accounts to sort.\n");
        return;
    }

    // Sort by account number
    selectionSortByField(accounts, count, 0);

    // Display sorted results
    printf("%-9s | %-15s | %-10s | %-10s | %-8s | %-10s\n",
           "Acct#", "Last Name", "First Name", "Mobile", "Type", "Balance");
    printf("----------|-----------------|------------|------------|----------|----------\n");

    for (int i = 0; i < count; i++)
    {
        printf("%-9u | %-15s | %-10s | %-10s | %-8s | %.2f\n",
               accounts[i].acctNum, accounts[i].lastName, accounts[i].firstName,
               accounts[i].mobileNumber, accounts[i].accountType, accounts[i].balance);
    }

    printf("\nTotal Accounts Sorted: %d\n", count);
}

// Mini statement - show account balance (simulating recent transaction)
void miniStatement(FILE *fPtr)
{
    unsigned int acctNum;
    struct clientData client = {0, "", "", 0.0, "", ""};

    printf("\n--- MINI STATEMENT ---\n");
    printf("Enter account number (1-%d): ", MAX_ACCOUNTS);
    if (scanf("%u", &acctNum) != 1)
    {
        printf("ERROR: Invalid input.\n");
        flushInputBuffer();
        return;
    }
    flushInputBuffer();

    readAccountRecord(fPtr, acctNum, &client);

    if (client.acctNum == 0)
    {
        printf("ERROR: Account #%u not found.\n", acctNum);
        return;
    }

    printf("\n========== MINI STATEMENT ==========\n");
    printf("Account Number : %u\n", client.acctNum);
    printf("Customer Name  : %s, %s\n", client.lastName, client.firstName);
    printf("Mobile Number  : %s\n", client.mobileNumber);
    printf("Account Type   : %s\n", client.accountType);
    printf("Current Balance: Rs. %.2f\n", client.balance);
    printf("Minimum Balance: Rs. %.2f\n", MIN_BALANCE);
    printf("====================================\n");
}

// Balance transfer between two accounts with error handling
void balanceTransfer(FILE *fPtr)
{
    unsigned int fromAcct, toAcct;
    double transferAmount;
    struct clientData fromClient = {0, "", "", 0.0, "", ""};
    struct clientData toClient = {0, "", "", 0.0, "", ""};

    printf("\n--- BALANCE TRANSFER ---\n");
    printf("Enter FROM account number (1-%d): ", MAX_ACCOUNTS);
    if (scanf("%u", &fromAcct) != 1)
    {
        printf("ERROR: Invalid input.\n");
        flushInputBuffer();
        return;
    }
    flushInputBuffer();

    readAccountRecord(fPtr, fromAcct, &fromClient);

    if (fromClient.acctNum == 0)
    {
        printf("ERROR: From Account #%u not found.\n", fromAcct);
        return;
    }

    printf("Enter TO account number (1-%d): ", MAX_ACCOUNTS);
    if (scanf("%u", &toAcct) != 1)
    {
        printf("ERROR: Invalid input.\n");
        flushInputBuffer();
        return;
    }
    flushInputBuffer();

    if (fromAcct == toAcct)
    {
        printf("ERROR: Cannot transfer to the same account.\n");
        return;
    }

    readAccountRecord(fPtr, toAcct, &toClient);

    if (toClient.acctNum == 0)
    {
        printf("ERROR: To Account #%u not found.\n", toAcct);
        return;
    }

    printf("Enter transfer amount: Rs. ");
    if (scanf("%lf", &transferAmount) != 1)
    {
        printf("ERROR: Invalid amount.\n");
        flushInputBuffer();
        return;
    }
    flushInputBuffer();

    if (transferAmount <= 0)
    {
        printf("ERROR: Transfer amount must be positive.\n");
        return;
    }

    // Error handling: check sufficient balance
    if (fromClient.balance - transferAmount < MIN_BALANCE)
    {
        printf("ERROR: Insufficient balance. Current balance: %.2f\n", fromClient.balance);
        printf("       Cannot transfer %.2f (would drop below minimum %.2f).\n",
               transferAmount, MIN_BALANCE);
        return;
    }

    // Perform transfer
    fromClient.balance -= transferAmount;
    toClient.balance += transferAmount;

    // Write both records back
    writeAccountRecord(fPtr, fromAcct, &fromClient);
    writeAccountRecord(fPtr, toAcct, &toClient);

    printf("\n========== TRANSFER SUCCESSFUL ==========\n");
    printf("From Account #%u: %.2f -> %.2f\n", fromAcct, fromClient.balance + transferAmount, fromClient.balance);
    printf("To Account   #%u: %.2f -> %.2f\n", toAcct, toClient.balance - transferAmount, toClient.balance);
    printf("Transfer Amount: Rs. %.2f\n", transferAmount);
    printf("==========================================\n");
}

// Get menu choice from user with validation
unsigned int enterChoice(void)
{
    unsigned int menuChoice;

    displayMenu();

    if (scanf("%u", &menuChoice) != 1)
    {
        flushInputBuffer();
        return 0; // Return invalid choice
    }
    flushInputBuffer();

    return menuChoice;
}