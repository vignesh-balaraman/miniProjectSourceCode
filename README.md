# Transaction Processing System - Enhanced Banking Application
### Mini Project - 24UCS271 Lab (Enhanced Edition)

---

## Objective
Comprehensive **Transaction Processing System** with file I/O, data structures, searching, sorting, pointers, and error handling. Enhanced version includes modern banking features, robust input validation, and optimized data access patterns.

---

## Features Implemented

### Core Features (Original)
- Create formatted text file (accounts.txt) from binary data
- Update account balance
- Add new account
- Delete account

### Enhanced Features (New)
- Search by Account Number - Direct binary search using pointer-based record access
- Search by Customer Name - Linear search with substring matching
- Display All Accounts - Tabular format showing all active accounts
- Sort by Account Number - Selection sort algorithm implementation
- Mini Statement - Display account details and balance
- Balance Transfer - Transfer funds between accounts with validation
- Input Validation - Error handling for invalid inputs, insufficient balance, file errors
- Automatic File Initialization - Creates credit.dat with 100 blank records if missing
- Extended Structure - Added fields: mobile number (10 digits) and account type (Saving/Current)
- Pointer-based Operations - Functions use pointers for structure access and parameter passing
- Error Handling - Minimum balance checks, file I/O validation, input buffer flushing

---

## File Structure
```
miniProjectSourceCode/
├── trans.c                   # Enhanced main program (600+ lines)
├── credit.dat               # Binary file (auto-created with 100 records)
├── accounts.txt             # Generated text file for export
└── README.md               # This file
```

---

## Compilation & Execution

### Prerequisites
- GCC Compiler or Clang or MSVC (any C99 or later compiler)
- Linux/macOS/Windows system with terminal

### Step 1: Compile the Program
```bash
gcc -o trans trans.c -Wall -Wextra
```

### Step 2: Run the Program
```bash
./trans          # Linux/macOS
```

### Step 3: On First Run
Program automatically creates `credit.dat` with 100 blank records:
```
INFO: Account file not found. Creating new file with 100 blank records...
INFO: Successfully created file with 100 blank records.
```

---

## Menu Operations (11 Options)

```
========================================
          MAIN MENU
========================================
1  - Export accounts to text file (accounts.txt)
2  - Update existing account balance
3  - Add new account
4  - Delete account
5  - Search account by account number
6  - Search account by customer name
7  - Display all accounts (tabular format)
8  - Sort accounts by account number
9  - Mini statement (recent transaction)
10 - Balance transfer between accounts
11 - Exit program
========================================
```

---

## Usage Examples

### Example 1: Add a New Account
```
Enter your choice (1-11): 3

--- ADD NEW ACCOUNT ---
Enter new account number (1-100): 1
Enter last name: Smith
Enter first name: John
Enter mobile number (10 digits): 9876543210
Enter account type (Saving/Current): Saving
Enter opening balance (minimum 100.00): 5000

NEW ACCOUNT CREATED SUCCESSFULLY:
Acct #    | Last Name       | First Name | Mobile     | Type     | Balance
----------|-----------------|------------|------------|----------|----------
1         | Smith           | John       | 9876543210 | Saving   | 5000.00
```

### Example 2: Update Account Balance
```
Enter your choice (1-11): 2

--- UPDATE ACCOUNT BALANCE ---
Enter account number to update (1-100): 1

Current Account Details:
Acct #    | Last Name       | First Name | Mobile     | Type     | Balance
----------|-----------------|------------|------------|----------|----------
1         | Smith           | John       | 9876543210 | Saving   | 5000.00

Enter transaction amount (positive for deposit, negative for withdrawal): 500

Account Updated Successfully:
Acct #    | Last Name       | First Name | Mobile     | Type     | Balance
----------|-----------------|------------|------------|----------|----------
1         | Smith           | John       | 9876543210 | Saving   | 5500.00
```

### Example 3: Search by Name
```
Enter your choice (1-11): 6

--- SEARCH BY CUSTOMER NAME ---
Enter customer name (or part of name): Smith

Search Results for "Smith":
Acct#     | Last Name       | First Name | Mobile     | Type     | Balance
----------|-----------------|------------|------------|----------|----------
1         | Smith           | John       | 9876543210 | Saving   | 5500.00

Total matches found: 1
```

### Example 4: Balance Transfer
```
Enter your choice (1-11): 10

--- BALANCE TRANSFER ---
Enter FROM account number (1-100): 1
Enter TO account number (1-100): 2
Enter transfer amount: Rs. 500

========== TRANSFER SUCCESSFUL ==========
From Account #1: 5500.00 -> 5000.00
To Account   #2: 3000.00 -> 3500.00
Transfer Amount: Rs. 500.00
==========================================
```

---

## Course Outcome Mapping

### CO1: Compilation, Execution & Debugging
- Q1a: Program compilation with GCC, menu-driven operations demonstration
- Q1b: Testing with different inputs, identifying and fixing logical issues
- Evidence: All functions tested, proper error messages, input validation

### CO2: Searching & Sorting (Arrays/Strings)
- Q2a: Search by account number and customer name (linear search with arrays)
- Q2b: Sort accounts using Selection Sort algorithm
- Evidence: searchByAccountNumber(), searchByName(), sortAccountsByNumber()

### CO3: Functional Decomposition & Pointers
- Q3a: Refactored into 20+ separate functions for modularity
- Q3b: Pointer-based operations for structure access
- Evidence: Modular design, pointer parameter passing, structure access via pointers

### CO4: Structures & New Features
- Q4a: Extended struct with: mobileNumber[11], accountType[10]
- Q4b: New features: Mini-statement, Balance transfer with validation
- Evidence: Enhanced struct definition, new menu options 9-10

### CO5: File I/O, Optimization & Error Handling
- Q5a: Auto-initialization of credit.dat, file update verification
- Q5b: Optimizations (pointer-based access), error handling (minimum balance, file operations)
- Evidence: initializeAccountFile(), validation functions, comprehensive error messages

---

## Technical Highlights

### Memory-Efficient Design
- Uses binary file I/O with random access (O(1) lookup time)
- Pointer-based parameter passing to avoid unnecessary data copying
- Efficient sorting with in-memory arrays

### Error Handling Strategy
1. Input Validation: All numeric inputs checked with scanf() return value validation
2. Buffer Management: flushInputBuffer() prevents input overflow
3. File I/O: All fread(), fwrite(), fseek() return values checked
4. Business Logic: Minimum balance validation, duplicate account checks

### Sorting Algorithm (Selection Sort)
- Time Complexity: O(n^2)
- Space Complexity: O(1) - in-place sorting
- Suitable for small to medium datasets

### Search Algorithms
- By Account Number: Direct binary search with fseek() - O(1)
- By Name: Linear search with substring matching - O(n)

---

## Code Quality Standards

✓ Descriptive variable names: acctNum, customerName, mobileNumber
✓ Clear function names: readAccountRecord(), balanceTransfer()
✓ Constants in UPPERCASE: MAX_ACCOUNTS, MIN_BALANCE
✓ Comprehensive comments and documentation
✓ Single responsibility principle for functions
✓ Consistent 4-space indentation
✓ Proper struct initialization

---

## Testing Checklist

- [ ] Compilation succeeds without errors or warnings
- [ ] Program creates credit.dat on first run
- [ ] Can add multiple accounts successfully
- [ ] Can update balances with minimum balance validation
- [ ] Can delete accounts with confirmation
- [ ] Search functions return correct results
- [ ] Sort displays records in correct order
- [ ] Balance transfer rejects insufficient balance
- [ ] Export to accounts.txt shows all records
- [ ] Mini statement displays correct format
- [ ] Invalid input handled gracefully
- [ ] Program exits cleanly with option 11

---

## Sample Test Flow

1. Create account #1 with 5000 balance
2. Create account #2 with 3000 balance
3. Export to accounts.txt
4. Update account #1 balance (+500)
5. Search for account by number
6. Search for account by name
7. Display all accounts
8. Sort accounts by number
9. Show mini statement
10. Transfer 500 from acc#1 to acc#2

---

## Important Notes

- Minimum balance: Rs. 100.00 (enforced in all transactions)
- Account range: 1-100 (managed by binary file)
- Mobile validation: 10 digits numeric only
- Deletion: Requires confirmation prompt
- File formats: Binary (credit.dat), Text (accounts.txt)

---

## Reference Links
- Original base code: https://github.com/24UCS271-MiniProject/miniProjectSourceCode/
- Replit Template: https://replit.com/@ashokb/Unit5Programs#trans.c
- Online GDB Compiler: https://www.onlinegdb.com

---

**Grade Mapping (80 Marks Total)**
- Q1A (8) - Compilation, execution, menu operations
- Q1B (8) - Debugging and issue identification
- Q2A (8) - Search by account/name
- Q2B (8) - Sorting implementation
- Q3A (8) - Functional decomposition
- Q3B (8) - Pointer-based operations
- Q4A (8) - Extended structures
- Q4B (8) - New banking features
- Q5A (8) - File generation/update
- Q5B (8) - Optimization & error handling

**Enhanced Edition - May 2026**
