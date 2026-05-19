# Bank Transaction System - Testing & Development Guide

## Overview
This guide provides comprehensive testing strategies, expected outputs, and troubleshooting for the Bank Transaction Processing System.

---

## Part 1: Compilation & Setup

### System Requirements
- **Compiler:** GCC 4.8+, Clang 3.4+, or MSVC 2015+
- **OS:** Linux, macOS, Windows (with MinGW/MSVC)
- **Memory:** 1MB minimum
- **Disk:** 100KB for binary files

### Compilation Steps

#### Linux/macOS
```bash
# Navigate to project directory
cd path/to/miniProjectSourceCode

# Compile with warnings
gcc -o trans trans.c -Wall -Wextra

# Compile with debug symbols (for gdb)
gcc -g -o trans trans.c -Wall -Wextra

# Run
./trans
```

#### Windows (with MinGW)
```bash
# Compile
gcc -o trans.exe trans.c -Wall -Wextra

# Run
trans.exe
```

#### Windows (with MSVC)
```bash
# Open Developer Command Prompt for Visual Studio
cl /W4 trans.c
trans.exe
```

### Compilation Flags Explained
- `-Wall` : Enable all common warnings
- `-Wextra` : Enable extra warnings
- `-g` : Include debug symbols for GDB
- `-O2` : Enable optimization (optional)

### Expected Output on First Run
```
========================================
   BANK TRANSACTION PROCESSING SYSTEM
========================================

INFO: Account file not found. Creating new file with 100 blank records...
INFO: Successfully created file with 100 blank records.

INFO: Existing account file loaded successfully.

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
Enter your choice (1-11): 
```

---

## Part 2: Functional Test Cases

### Test Suite 1: Account Creation & Validation

#### Test Case 1.1: Valid Account Creation
```
Menu Choice: 3 (Add new account)
Input:
- Account number: 1
- Last name: Smith
- First name: John
- Mobile: 9876543210
- Account type: Saving
- Balance: 5000

Expected Output:
NEW ACCOUNT CREATED SUCCESSFULLY:
Acct #    | Last Name       | First Name | Mobile     | Type     | Balance
----------|-----------------|------------|------------|----------|----------
1         | Smith           | John       | 9876543210 | Saving   | 5000.00

Result: ✓ PASS / ✗ FAIL
```

#### Test Case 1.2: Invalid Mobile Number (too short)
```
Menu Choice: 3
Input:
- Account number: 2
- Last name: Brown
- First name: David
- Mobile: 123456789 (9 digits instead of 10)
- Balance: 5000

Expected Output:
ERROR: Invalid mobile number. Must be 10 digits.

Result: ✓ PASS / ✗ FAIL
```

#### Test Case 1.3: Invalid Balance (below minimum)
```
Menu Choice: 3
Input:
- Account number: 3
- Last name: Davis
- First name: Sarah
- Mobile: 9876543211
- Account type: Current
- Balance: 50 (below 100 minimum)

Expected Output:
ERROR: Minimum balance must be 100.00

Result: ✓ PASS / ✗ FAIL
```

#### Test Case 1.4: Duplicate Account
```
Menu Choice: 3
Input:
- Account number: 1 (already exists from Test 1.1)

Expected Output:
ERROR: Account #1 already exists.

Result: ✓ PASS / ✗ FAIL
```

#### Test Case 1.5: Invalid Account Number
```
Menu Choice: 3
Input:
- Account number: 101 (exceeds 1-100 range)

Expected Output:
ERROR: Account number must be between 1 and 100.

Result: ✓ PASS / ✗ FAIL
```

---

### Test Suite 2: Update & Transaction Operations

#### Test Case 2.1: Valid Deposit
```
Menu Choice: 2 (Update balance)
Initial Account: #1 (Balance: 5000.00)
Input:
- Account to update: 1
- Transaction amount: +500 (positive for deposit)

Expected Output:
Current Account Details:
Acct #    | Last Name       | First Name | Mobile     | Type     | Balance
----------|-----------------|------------|------------|----------|----------
1         | Smith           | John       | 9876543210 | Saving   | 5000.00

Account Updated Successfully:
Acct #    | Last Name       | First Name | Mobile     | Type     | Balance
----------|-----------------|------------|------------|----------|----------
1         | Smith           | John       | 9876543210 | Saving   | 5500.00

Result: ✓ PASS / ✗ FAIL
```

#### Test Case 2.2: Valid Withdrawal
```
Menu Choice: 2
Initial Account: #1 (Balance: 5500.00)
Input:
- Account to update: 1
- Transaction amount: -400 (negative for withdrawal)

Expected Output:
Account Updated Successfully:
Acct #    | Last Name       | First Name | Mobile     | Type     | Balance
----------|-----------------|------------|------------|----------|----------
1         | Smith           | John       | 9876543210 | Saving   | 5100.00

Result: ✓ PASS / ✗ FAIL
```

#### Test Case 2.3: Withdrawal Below Minimum Balance
```
Menu Choice: 2
Initial Account: #1 (Balance: 5100.00)
Input:
- Account to update: 1
- Transaction amount: -5100 (would result in 0, below 100 minimum)

Expected Output:
ERROR: Insufficient balance. Transaction would result in 0.00
       Minimum balance required: 100.00

Result: ✓ PASS / ✗ FAIL
```

#### Test Case 2.4: Update Non-existent Account
```
Menu Choice: 2
Input:
- Account to update: 99 (if never created)

Expected Output:
ERROR: Account #99 does not exist.

Result: ✓ PASS / ✗ FAIL
```

---

### Test Suite 3: Search Operations

#### Test Case 3.1: Search by Account Number (Found)
```
Menu Choice: 5 (Search by account number)
Input:
- Account number: 1

Expected Output:
Account Found:
Acct #    | Last Name       | First Name | Mobile     | Type     | Balance
----------|-----------------|------------|------------|----------|----------
1         | Smith           | John       | 9876543210 | Saving   | 5100.00

Result: ✓ PASS / ✗ FAIL
```

#### Test Case 3.2: Search by Account Number (Not Found)
```
Menu Choice: 5
Input:
- Account number: 50 (never created)

Expected Output:
ERROR: Account #50 not found.

Result: ✓ PASS / ✗ FAIL
```

#### Test Case 3.3: Search by Name (Partial Match)
```
Menu Choice: 6 (Search by name)
Created accounts: Smith (1), Davis (2), Jones (3)
Input:
- Customer name: Smith

Expected Output:
Search Results for "Smith":
Acct#     | Last Name       | First Name | Mobile     | Type     | Balance
----------|-----------------|------------|------------|----------|----------
1         | Smith           | John       | 9876543210 | Saving   | 5100.00

Total matches found: 1

Result: ✓ PASS / ✗ FAIL
```

#### Test Case 3.4: Search by Name (Multiple Matches)
```
Menu Choice: 6
Created accounts: Smith John, Smith Mary
Input:
- Customer name: Smith

Expected Output:
Search Results for "Smith":
Acct#     | Last Name       | First Name | Mobile     | Type     | Balance
----------|-----------------|------------|------------|----------|----------
1         | Smith           | John       | 9876543210 | Saving   | 5100.00
2         | Smith           | Mary       | 9876543211 | Current  | 3000.00

Total matches found: 2

Result: ✓ PASS / ✗ FAIL
```

#### Test Case 3.5: Search by Name (No Match)
```
Menu Choice: 6
Input:
- Customer name: Nonexistent

Expected Output:
No accounts found matching "Nonexistent".

Result: ✓ PASS / ✗ FAIL
```

---

### Test Suite 4: Display & Sort Operations

#### Test Case 4.1: Display All Accounts
```
Menu Choice: 7 (Display all accounts)
Created accounts: 1, 2, 3
Input: (none)

Expected Output:
--- ALL ACCOUNTS (TABULAR FORMAT) ---
Acct#     | Last Name       | First Name | Mobile     | Type     | Balance
----------|-----------------|------------|------------|----------|----------
1         | Smith           | John       | 9876543210 | Saving   | 5100.00
2         | Brown           | David      | 9876543211 | Current  | 3000.00
3         | Davis           | Sarah      | 9876543212 | Saving   | 7500.00
----------|-----------------|------------|------------|----------|----------
Total Accounts: 3

Result: ✓ PASS / ✗ FAIL
```

#### Test Case 4.2: Sort Accounts by Number
```
Menu Choice: 8 (Sort by account number)
Created accounts: 5, 2, 8, 1
Input: (none)

Expected Output:
--- SORT ACCOUNTS BY ACCOUNT NUMBER ---
Acct#     | Last Name       | First Name | Mobile     | Type     | Balance
----------|-----------------|------------|------------|----------|----------
1         | Smith           | John       | 9876543210 | Saving   | 5100.00
2         | Brown           | David      | 9876543211 | Current  | 3000.00
5         | Jones           | Michael    | 9876543214 | Saving   | 10000.00
8         | Davis           | Barbara    | 9876543217 | Current  | 3500.00
----------|-----------------|------------|------------|----------|----------

Total Accounts Sorted: 4

Result: ✓ PASS / ✗ FAIL
```

#### Test Case 4.3: Sort Empty Database
```
Menu Choice: 8
Input: (none)
No accounts created

Expected Output:
--- SORT ACCOUNTS BY ACCOUNT NUMBER ---
No accounts to sort.

Result: ✓ PASS / ✗ FAIL
```

---

### Test Suite 5: Balance Transfer

#### Test Case 5.1: Valid Transfer
```
Menu Choice: 10 (Balance transfer)
Account #1: Balance 5100.00
Account #2: Balance 3000.00
Input:
- From account: 1
- To account: 2
- Amount: 500

Expected Output:
========== TRANSFER SUCCESSFUL ==========
From Account #1: 5100.00 -> 4600.00
To Account   #2: 3000.00 -> 3500.00
Transfer Amount: Rs. 500.00
==========================================

Result: ✓ PASS / ✗ FAIL
```

#### Test Case 5.2: Transfer Insufficient Balance
```
Menu Choice: 10
Account #1: Balance 4600.00
Account #3: Balance 7500.00
Input:
- From account: 1
- To account: 3
- Amount: 4700 (would leave 1000, but still above 100 minimum)

Expected Output:
========== TRANSFER SUCCESSFUL ==========
...

Actually, this should pass since 4600 - 4700 would be negative, 
ERROR message expected:
ERROR: Insufficient balance. Current balance: 4600.00
       Cannot transfer 4700.00 (would drop below minimum 100.00).

Result: ✓ PASS / ✗ FAIL
```

#### Test Case 5.3: Transfer to Same Account
```
Menu Choice: 10
Input:
- From account: 1
- To account: 1
- Amount: 100

Expected Output:
ERROR: Cannot transfer to the same account.

Result: ✓ PASS / ✗ FAIL
```

#### Test Case 5.4: Transfer from Non-existent Account
```
Menu Choice: 10
Input:
- From account: 99
- To account: 1
- Amount: 100

Expected Output:
ERROR: From Account #99 not found.

Result: ✓ PASS / ✗ FAIL
```

---

### Test Suite 6: Delete & Mini Statement

#### Test Case 6.1: Delete Account (Confirmed)
```
Menu Choice: 4 (Delete account)
Input:
- Account to delete: 1
- Confirmation: y

Expected Output:
Account #1 DELETED successfully.

Verify with Menu Choice 5:
- Search for account 1: ERROR: Account #1 not found.

Result: ✓ PASS / ✗ FAIL
```

#### Test Case 6.2: Delete Account (Cancelled)
```
Menu Choice: 4
Input:
- Account to delete: 2
- Confirmation: n

Expected Output:
Deletion cancelled.

Verify with Menu Choice 5:
- Search for account 2: Should still exist

Result: ✓ PASS / ✗ FAIL
```

#### Test Case 6.3: Mini Statement
```
Menu Choice: 9 (Mini statement)
Input:
- Account number: 2

Expected Output:
========== MINI STATEMENT ==========
Account Number : 2
Customer Name  : Brown, David
Mobile Number  : 9876543211
Account Type   : Current
Current Balance: Rs. 3500.00
Minimum Balance: Rs. 100.00
====================================

Result: ✓ PASS / ✗ FAIL
```

---

### Test Suite 7: File Operations

#### Test Case 7.1: Export to Text File
```
Menu Choice: 1 (Export to accounts.txt)
Input: (none)

Expected Output:
SUCCESS: Exported 5 accounts to accounts.txt

File Contents (accounts.txt):
Acct#     | Last Name       | First Name | Mobile     | Type     | Balance
----------|-----------------|------------|------------|----------|----------
1         | Smith           | John       | 9876543210 | Saving   | 5000.00
2         | Brown           | David      | 9876543211 | Current  | 3000.00
...
Total Records: 5

Result: ✓ PASS / ✗ FAIL
```

#### Test Case 7.2: File Persistence
```
Step 1: Create account #1 (Balance: 5000)
Step 2: Exit program (Menu choice 11)
Step 3: Run program again
Step 4: Search for account #1

Expected Output:
Account Found:
Acct #    | Last Name       | First Name | Mobile     | Type     | Balance
----------|-----------------|------------|------------|----------|----------
1         | Smith           | John       | 9876543210 | Saving   | 5000.00

Result: ✓ PASS / ✗ FAIL
```

---

### Test Suite 8: Input Validation

#### Test Case 8.1: Invalid Menu Choice
```
Menu Choice: 15 (out of range)

Expected Output:
ERROR: Invalid choice. Please select 1-11.

Result: ✓ PASS / ✗ FAIL
```

#### Test Case 8.2: Non-numeric Menu Input
```
Menu Choice: abc (text input)

Expected Output:
ERROR: Invalid choice. Please select 1-11.

Result: ✓ PASS / ✗ FAIL
```

#### Test Case 8.3: Non-numeric Account Number
```
Menu Choice: 5 (Search)
Input: abc (text instead of number)

Expected Output:
ERROR: Invalid input.

Result: ✓ PASS / ✗ FAIL
```

---

## Part 3: Performance Testing

### Test Case P.1: Large Data Set
```
Step 1: Create 50 accounts with varying balances
Step 2: Sort accounts
Expected: Sorting completes in < 100ms
Result: ✓ PASS / ✗ FAIL
```

### Test Case P.2: Search Performance
```
Step 1: Create 100 accounts
Step 2: Search for account #100
Expected: Search completes instantly (O(1) with random access)
Result: ✓ PASS / ✗ FAIL
```

---

## Part 4: Debugging Checklist

### Compilation Errors
```
Error: "gcc: command not found"
Solution: Install MinGW (Windows) or GCC (Linux/macOS)

Error: "undefined reference to `__main__'"
Solution: Ensure you're compiling a C program, not linking improperly

Error: "conflicting types for 'function'"
Solution: Check function prototypes match definitions
```

### Runtime Errors
```
Segmentation Fault (Core dumped)
Possible causes:
- Buffer overflow in string operations
- Invalid file pointer operations
- Null pointer dereference

Infinite Loop
Possible causes:
- feof() misuse in file operations
- Incorrect loop conditions
```

### Logical Errors
```
Transfers showing negative balance:
- Check minimum balance validation

Search returning wrong results:
- Verify substring matching logic
- Check record reading with fseek()

Sort not working:
- Verify selection sort algorithm
- Check comparison conditions
```

---

## Part 5: Optimization Opportunities

1. **Binary Search** - Current: O(n) linear search by name, could use hash table
2. **Indexing** - Create index file for faster lookups
3. **Caching** - Cache frequently accessed accounts in memory
4. **Batch Operations** - Support bulk account creation/deletion
5. **Transaction Log** - Keep audit trail of all operations

---

## Summary Table

| Test Suite | Test Cases | Expected Pass | Actual Pass | Notes |
|-----------|-----------|--------------|-----------|-------|
| Account Creation | 5 | 5 | _ | Validation critical |
| Update/Transaction | 4 | 4 | _ | Balance checks essential |
| Search | 5 | 5 | _ | Both search types needed |
| Display/Sort | 3 | 3 | _ | Sorting algorithm correctness |
| Transfer | 4 | 4 | _ | Error handling critical |
| Delete/Mini | 3 | 3 | _ | Confirmation required |
| File Operations | 2 | 2 | _ | Persistence verification |
| Input Validation | 3 | 3 | _ | Error handling |
| **TOTAL** | **29** | **29** | **_** | Complete test coverage |

---

**Document Version:** 1.0  
**Last Updated:** May 2026
