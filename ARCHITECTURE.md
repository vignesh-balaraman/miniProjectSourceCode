# Bank Transaction System - Architecture & Implementation Guide

## Table of Contents
1. Program Architecture
2. Data Structure Design
3. Function Design Patterns
4. Key Algorithms
5. Error Handling Strategy
6. Code Walkthrough

---

## 1. Program Architecture

### High-Level Overview
```
┌─────────────────────────────────────────────────────────┐
│         BANK TRANSACTION PROCESSING SYSTEM             │
├─────────────────────────────────────────────────────────┤
│                      main()                             │
├─────────────────────────────────────────────────────────┤
│  ┌──────────────────────────────────────────────────┐  │
│  │  initializeAccountFile()                         │  │
│  │  [Creates credit.dat with 100 blank records]    │  │
│  └──────────────────────────────────────────────────┘  │
├─────────────────────────────────────────────────────────┤
│  ┌──────────────────────────────────────────────────┐  │
│  │  Menu Loop: enterChoice() -> Switch Statement   │  │
│  └──────────────────────────────────────────────────┘  │
├─────────────────────────────────────────────────────────┤
│  Core Operations (11 menu options):                     │
│  1. textFile()          - Export to accounts.txt       │
│  2. updateRecord()      - Modify balance               │
│  3. newRecord()         - Create new account           │
│  4. deleteRecord()      - Remove account               │
│  5. searchByAccountNumber() - Find by ID               │
│  6. searchByName()      - Find by customer name        │
│  7. displayAllAccounts() - Show all records            │
│  8. sortAccountsByNumber() - Sort & display            │
│  9. miniStatement()     - Show account summary         │
│  10. balanceTransfer()  - Transfer between accounts    │
│  11. Exit               - Graceful termination         │
├─────────────────────────────────────────────────────────┤
│  Helper Functions:                                      │
│  readAccountRecord()    - Read from binary file        │
│  writeAccountRecord()   - Write to binary file         │
│  validateAccountNumber() - Input validation           │
│  validateBalance()      - Balance validation          │
│  isValidMobileNumber()  - Mobile format validation    │
│  flushInputBuffer()     - Clear stdin buffer          │
│  displayAccountRecord() - Format & print record       │
│  selectionSortByField() - Sort array in memory       │
└─────────────────────────────────────────────────────────┘
```

### Data Flow Diagram
```
User Input
    |
    v
enterChoice() [Menu Selection]
    |
    +-----> textFile() ---------> fread() ----> fprintf()
    |
    +-----> updateRecord() -----> readAccountRecord()
    |                                 |
    |                                 v
    |                             [Validate]
    |                                 |
    |                                 v
    |                            writeAccountRecord()
    |
    +-----> newRecord() ---------> [Validate Input]
    |                                 |
    |                                 v
    |                            writeAccountRecord()
    |
    +-----> deleteRecord() ------> readAccountRecord()
    |                                 |
    |                                 v
    |                            [Confirmation]
    |                                 |
    |                                 v
    |                            writeAccountRecord()
    |
    +-----> searchByAccountNumber() -> readAccountRecord()
    |
    +-----> searchByName() ------> Linear Search (fread loop)
    |
    +-----> displayAllAccounts() -> Sequential fread()
    |
    +-----> sortAccountsByNumber() -> Load All -> selectionSort()
    |
    +-----> miniStatement() ------> readAccountRecord()
    |
    +-----> balanceTransfer() -----> readAccountRecord() x2
                                        |
                                        v
                                    [Validate]
                                        |
                                        v
                                    writeAccountRecord() x2
```

---

## 2. Data Structure Design

### Structure Definition
```c
struct clientData {
    unsigned int acctNum;      // 4 bytes - Unique account identifier (1-100)
    char lastName[15];         // 15 bytes - Customer last name (max 14 chars + null)
    char firstName[10];        // 10 bytes - Customer first name (max 9 chars + null)
    double balance;            // 8 bytes - Account balance (minimum 100.00)
    char mobileNumber[11];     // 11 bytes - 10-digit mobile (exactly 10 + null)
    char accountType[10];      // 10 bytes - "Saving" or "Current" (max 9 chars + null)
};
// Total: 58 bytes per record
```

### Memory Layout
```
Offset  Size    Field               Description
------  ----    -----               -----------
0       4       acctNum             Account number (0 = blank record)
4       15      lastName            Family name
19      10      firstName           Given name
29      8       balance             Account balance in Rs.
37      11      mobileNumber        10-digit phone
48      10      accountType         Type of account
58      -       TOTAL               One record
```

### File Organization
```
credit.dat (Binary File)
┌─────────────────────┐
│  Record 1 (Acct 1)  │  58 bytes
├─────────────────────┤
│  Record 2 (Acct 2)  │  58 bytes
├─────────────────────┤
│  Record 3 (Acct 3)  │  58 bytes
├─────────────────────┤
│      ...            │
├─────────────────────┤
│ Record 100 (Acct 100)  │  58 bytes
└─────────────────────┘
Total: 100 × 58 = 5800 bytes

Random Access:
To access account N: seek to (N-1) × 58 bytes
Example: Account 5 starts at byte 232 (4 × 58)
```

---

## 3. Function Design Patterns

### Pattern 1: Pointer-Based Record Access
```c
void readAccountRecord(FILE *fPtr, unsigned int acctNum, struct clientData *client)
{
    // Use POINTER parameter to avoid copying large struct
    // Advantage: O(1) memory overhead, direct modification
    
    // Validate input
    if (!validateAccountNumber(acctNum)) {
        client->acctNum = 0;  // Signal error by setting acctNum = 0
        return;
    }
    
    // Seek to record location
    fseek(fPtr, (acctNum - 1) * sizeof(struct clientData), SEEK_SET);
    
    // Read single record
    if (fread(client, sizeof(struct clientData), 1, fPtr) != 1) {
        printf("ERROR: Failed to read record at account number %d.\n", acctNum);
        client->acctNum = 0;  // Signal read failure
    }
}
```

**Design Rationale:**
- Pointer parameter avoids struct copying (58 bytes)
- Error indication via struct field (acctNum = 0)
- Single responsibility: just read

### Pattern 2: Input Validation Wrapper
```c
int validateAccountNumber(unsigned int acctNum)
{
    // Centralized validation logic
    // Returns 1 (true) if valid, 0 (false) if invalid
    
    if (acctNum < 1 || acctNum > MAX_ACCOUNTS) {
        printf("ERROR: Account number must be between 1 and %d.\n", MAX_ACCOUNTS);
        return 0;
    }
    return 1;
}
```

**Benefits:**
- DRY principle: validation logic used across 10+ functions
- Consistent error messages
- Easy to modify validation rules

### Pattern 3: Buffer Flush After Input
```c
unsigned int enterChoice(void)
{
    unsigned int menuChoice;
    
    displayMenu();
    
    // Check scanf return value
    if (scanf("%u", &menuChoice) != 1) {
        flushInputBuffer();  // Clear bad input
        return 0;  // Return invalid choice
    }
    flushInputBuffer();  // Always flush after scanf
    
    return menuChoice;
}

void flushInputBuffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;  // Consume characters until newline
}
```

**Why Important:**
- Prevents "leftover" characters in input buffer
- Avoids cascading input errors
- fgets() still works correctly after scanf()

### Pattern 4: Structured Record Display
```c
void displayAccountRecord(struct clientData *client)
{
    // Consistent formatting across entire program
    // Single function for all record displays
    
    printf("Acct #    | Last Name       | First Name | Mobile     | Type     | Balance\n");
    printf("----------|-----------------|------------|------------|----------|----------\n");
    printf("%-9u | %-15s | %-10s | %-10s | %-8s | %.2f\n",
           client->acctNum, client->lastName, client->firstName,
           client->mobileNumber, client->accountType, client->balance);
}
```

---

## 4. Key Algorithms

### Algorithm 1: Selection Sort
```c
void selectionSortByField(struct clientData *accounts, int count, int sortBy)
{
    // sortBy: 0 = account number, 1 = balance
    
    // Outer loop: iterate through array
    for (int i = 0; i < count - 1; i++) {
        int minIdx = i;
        
        // Inner loop: find minimum element
        for (int j = i + 1; j < count; j++) {
            int shouldSwap = 0;
            
            if (sortBy == 0) {
                // Sort by account number (ascending)
                if (accounts[j].acctNum < accounts[minIdx].acctNum)
                    shouldSwap = 1;
            }
            else if (sortBy == 1) {
                // Sort by balance (descending)
                if (accounts[j].balance > accounts[minIdx].balance)
                    shouldSwap = 1;
            }
            
            if (shouldSwap)
                minIdx = j;
        }
        
        // Swap if needed
        if (minIdx != i) {
            struct clientData temp = accounts[i];
            accounts[i] = accounts[minIdx];
            accounts[minIdx] = temp;
        }
    }
}
```

**Complexity Analysis:**
- Time: O(n²) - Two nested loops
- Space: O(1) - In-place sorting (only temp variable)
- Best for: Small to medium datasets (< 1000 records)

**Example:**
```
Initial: [5, 2, 8, 1, 3]
Pass 1:  [1, 2, 8, 5, 3]  (1 is minimum)
Pass 2:  [1, 2, 8, 5, 3]  (2 is minimum)
Pass 3:  [1, 2, 3, 5, 8]  (3 is minimum, 5 and 8 already sorted)
Pass 4:  [1, 2, 3, 5, 8]  (5 is minimum)
Final:   [1, 2, 3, 5, 8]  (Sorted!)
```

### Algorithm 2: Linear Search with Substring Matching
```c
void searchByName(FILE *fPtr)
{
    char searchName[25];
    struct clientData client = {0, "", "", 0.0, "", ""};
    int found = 0;
    
    // Get search term
    printf("Enter customer name (or part of name): ");
    fgets(searchName, sizeof(searchName), stdin);
    searchName[strcspn(searchName, "\n")] = '\0';  // Remove newline
    
    rewind(fPtr);  // Go to start of file
    
    // Linear scan: O(n) complexity
    while (fread(&client, sizeof(struct clientData), 1, fPtr) == 1) {
        if (client.acctNum != 0) {  // Skip blank records
            
            // Substring search using strstr()
            // strstr returns non-NULL if substring found
            if (strstr(client.lastName, searchName) || 
                strstr(client.firstName, searchName)) {
                
                printf("%-9u | %-15s | %-10s | ...\n",
                       client.acctNum, client.lastName, client.firstName, ...);
                found++;
            }
        }
    }
}
```

**Complexity Analysis:**
- Time: O(n × m) where n = records, m = average string length
- Space: O(1) - Single record in memory at a time
- Suitable for: Flexible substring matching

### Algorithm 3: Random Access Lookup (O(1))
```c
void readAccountRecord(FILE *fPtr, unsigned int acctNum, struct clientData *client)
{
    // Direct calculation of file position
    // No need to scan file sequentially!
    
    fseek(fPtr, (acctNum - 1) * sizeof(struct clientData), SEEK_SET);
    // Position = (account_number - 1) × 58 bytes
    
    fread(client, sizeof(struct clientData), 1, fPtr);
}
```

**Why O(1)?**
- File seek is constant time (disk operation)
- No loops or iterations needed
- Compare: Linear search needs up to 100 file reads

**Example:**
```
To access account 50:
seek(file, (50-1) * 58) = seek(file, 2842)
Then fread() one record
Total: 2 disk operations, regardless of account number!
```

---

## 5. Error Handling Strategy

### Error Handling Hierarchy
```
Level 1: Input Validation
├── scanf() return value check
├── Account number range check (1-100)
├── Balance range check (>= 100.00)
├── Mobile number format check (10 digits)
└── String length validation (buffer overflow prevention)

Level 2: Business Logic Validation
├── Account existence verification
├── Duplicate account check
├── Minimum balance after transaction check
├── Same account transfer check
└── Invalid file operation check

Level 3: File I/O Error Handling
├── fopen() null pointer check
├── fread() return value check (should be 1)
├── fwrite() return value check (should be 1)
├── fseek() before all file operations
└── fflush() after all writes

Level 4: Graceful Degradation
├── User-friendly error messages
├── Suggest corrective actions
├── Return to main menu (don't crash)
└── Maintain program state
```

### Error Message Examples

#### Category: Input Validation
```c
printf("ERROR: Invalid input.\n");
printf("ERROR: Account number must be between 1 and 100.\n");
printf("ERROR: Minimum balance must be 100.00\n");
printf("ERROR: Invalid mobile number. Must be 10 digits.\n");
```

#### Category: Business Logic
```c
printf("ERROR: Account #%u does not exist.\n", acctNum);
printf("ERROR: Account #%u already exists.\n", acctNum);
printf("ERROR: Insufficient balance. Current balance: %.2f\n", balance);
printf("ERROR: Cannot transfer to the same account.\n");
```

#### Category: File Operations
```c
printf("ERROR: Could not create account file.\n");
printf("ERROR: Failed to read record at account number %d.\n", acctNum);
printf("ERROR: Failed to write record for account %d.\n", acctNum);
printf("ERROR: Could not open %s for writing.\n", TEXT_FILE);
```

---

## 6. Code Walkthrough

### Scenario: User Creates Account #5

```c
// User selects menu option 3
case 3:
    newRecord(cfPtr);  // Call with file pointer
    break;

// Inside newRecord():
void newRecord(FILE *fPtr)
{
    struct clientData client = {0, "", "", 0.0, "", ""};  // Initialize to zero
    unsigned int acctNum;
    
    printf("\n--- ADD NEW ACCOUNT ---\n");
    printf("Enter new account number (1-%d): ", MAX_ACCOUNTS);
    
    // Input: 5
    if (scanf("%u", &acctNum) != 1)  // Check scanf succeeded
    {
        printf("ERROR: Invalid input.\n");
        flushInputBuffer();
        return;
    }
    flushInputBuffer();  // Clear any leftover input
    
    // acctNum = 5, validate
    if (!validateAccountNumber(acctNum))  // Check 1 <= 5 <= 100
        return;  // Exit if invalid
    
    // Read existing record to check if account exists
    readAccountRecord(fPtr, acctNum, &client);
    // fseek(fPtr, (5-1)*58, SEEK_SET) = fseek(fPtr, 232, SEEK_SET)
    // fread one 58-byte record into &client
    
    if (client.acctNum != 0)  // If acctNum != 0, account exists
    {
        printf("ERROR: Account #%u already exists.\n", acctNum);
        return;
    }
    
    // Account doesn't exist, collect data
    printf("Enter last name: ");
    
    // Input: "Smith"
    if (fgets(client.lastName, sizeof(client.lastName), stdin) == NULL)
    {
        printf("ERROR: Failed to read last name.\n");
        return;
    }
    client.lastName[strcspn(client.lastName, "\n")] = '\0';  // Remove newline
    // client.lastName = "Smith"
    
    // Similar for firstName, mobile, accountType...
    
    printf("Enter opening balance (minimum %.2f): ", MIN_BALANCE);
    
    // Input: 5000
    if (scanf("%lf", &client.balance) != 1)
    {
        printf("ERROR: Invalid balance input.\n");
        flushInputBuffer();
        return;
    }
    flushInputBuffer();
    // client.balance = 5000.00
    
    // Validate balance
    if (!validateBalance(client.balance))  // Check 5000 >= 100
        return;  // Already passed
    
    // Set account number
    client.acctNum = acctNum;  // client.acctNum = 5
    
    // Write to file
    writeAccountRecord(fPtr, acctNum, &client);
    // fseek(fPtr, (5-1)*58, SEEK_SET) = fseek(fPtr, 232, SEEK_SET)
    // fwrite 58-byte record from &client
    // fflush to ensure written to disk
    
    // Display success
    printf("\nNEW ACCOUNT CREATED SUCCESSFULLY:\n");
    displayAccountRecord(&client);
    // Prints formatted table with account #5 data
}
```

### Scenario: User Transfers 500 from Account 5 to 2

```c
// User selects menu option 10
case 10:
    balanceTransfer(cfPtr);  // Call with file pointer
    break;

// Inside balanceTransfer():
void balanceTransfer(FILE *fPtr)
{
    unsigned int fromAcct, toAcct;
    double transferAmount;
    struct clientData fromClient = {0, "", "", 0.0, "", ""};
    struct clientData toClient = {0, "", "", 0.0, "", ""};
    
    printf("\n--- BALANCE TRANSFER ---\n");
    
    // Input: 5 (from account)
    printf("Enter FROM account number (1-%d): ", MAX_ACCOUNTS);
    if (scanf("%u", &fromAcct) != 1)  // fromAcct = 5
    {
        printf("ERROR: Invalid input.\n");
        flushInputBuffer();
        return;
    }
    flushInputBuffer();
    
    // Read account 5
    readAccountRecord(fPtr, fromAcct, &fromClient);
    // fromClient now has: acctNum=5, lastName="Smith", balance=5000.00
    
    if (fromClient.acctNum == 0)  // Account doesn't exist
    {
        printf("ERROR: From Account #%u not found.\n", fromAcct);
        return;
    }
    
    // Input: 2 (to account)
    printf("Enter TO account number (1-%d): ", MAX_ACCOUNTS);
    if (scanf("%u", &toAcct) != 1)  // toAcct = 2
    {
        printf("ERROR: Invalid input.\n");
        flushInputBuffer();
        return;
    }
    flushInputBuffer();
    
    if (fromAcct == toAcct)  // Same account check
    {
        printf("ERROR: Cannot transfer to the same account.\n");
        return;
    }
    
    // Read account 2
    readAccountRecord(fPtr, toAcct, &toClient);
    // toClient now has: acctNum=2, lastName="Brown", balance=3000.00
    
    if (toClient.acctNum == 0)  // Account doesn't exist
    {
        printf("ERROR: To Account #%u not found.\n", toAcct);
        return;
    }
    
    // Input: 500 (amount)
    printf("Enter transfer amount: Rs. ");
    if (scanf("%lf", &transferAmount) != 1)  // transferAmount = 500
    {
        printf("ERROR: Invalid amount.\n");
        flushInputBuffer();
        return;
    }
    flushInputBuffer();
    
    if (transferAmount <= 0)  // Positive amount check
    {
        printf("ERROR: Transfer amount must be positive.\n");
        return;
    }
    
    // Calculate new balance for from account
    double newFromBalance = fromClient.balance - transferAmount;
    // newFromBalance = 5000 - 500 = 4500
    
    // Check minimum balance
    if (newFromBalance < MIN_BALANCE)  // 4500 < 100? No
    {
        printf("ERROR: Insufficient balance...\n");
        return;
    }
    
    // Perform transfer (update both balances)
    fromClient.balance = newFromBalance;  // 4500
    toClient.balance += transferAmount;   // 3000 + 500 = 3500
    
    // Write both records back to file
    writeAccountRecord(fPtr, fromAcct, &fromClient);
    // Write account 5 with new balance 4500
    
    writeAccountRecord(fPtr, toAcct, &toClient);
    // Write account 2 with new balance 3500
    
    // Display success
    printf("\n========== TRANSFER SUCCESSFUL ==========\n");
    printf("From Account #%u: %.2f -> %.2f\n", 
           fromAcct, fromClient.balance + transferAmount, fromClient.balance);
    // "From Account #5: 5000.00 -> 4500.00"
    printf("To Account   #%u: %.2f -> %.2f\n",
           toAcct, toClient.balance - transferAmount, toClient.balance);
    // "To Account   #2: 3000.00 -> 3500.00"
    printf("Transfer Amount: Rs. %.2f\n", transferAmount);  // "Rs. 500.00"
}
```

---

## Summary of Key Design Decisions

| Decision | Rationale |
|----------|-----------|
| **Binary File I/O** | O(1) random access, compact storage |
| **Fixed 100 Records** | Predictable file size (5800 bytes) |
| **Pointer Parameters** | Avoid struct copying overhead |
| **Error via acctNum=0** | Consistent error signaling |
| **Buffer Flush Pattern** | Prevent cascading input errors |
| **Selection Sort** | Simple O(n²), suitable for 100 records |
| **Linear Name Search** | Flexible substring matching |
| **Validation Functions** | DRY principle, consistent validation |
| **Structured Display** | Consistent formatting across program |

---

**Document Version:** 1.0  
**Last Updated:** May 2026  
**Total Program Lines:** 600+  
**Functions:** 20+  
**Algorithms:** 3 (Select Sort, Linear Search, Random Access Lookup)
