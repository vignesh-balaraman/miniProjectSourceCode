#!/bin/bash
# Quick Start Guide for Bank Transaction System
# This script helps compile and run the program

echo "========================================="
echo " Bank Transaction System - Quick Start"
echo "========================================="
echo ""

# Check if trans.c exists
if [ ! -f "trans.c" ]; then
    echo "ERROR: trans.c not found in current directory!"
    echo "Please ensure you're in the correct directory:"
    echo "  cd path/to/miniProjectSourceCode"
    exit 1
fi

# Step 1: Compile
echo "[STEP 1] Compiling trans.c..."
echo "Command: gcc -o trans trans.c -Wall -Wextra"
echo ""

if gcc -o trans trans.c -Wall -Wextra 2>&1; then
    echo "✓ Compilation SUCCESSFUL!"
    echo ""
    
    # Check if executable was created
    if [ -f "trans" ]; then
        echo "[STEP 2] Running the program..."
        echo "Command: ./trans"
        echo ""
        echo "========================================="
        echo ""
        
        # Run the program
        ./trans
        
        echo ""
        echo "========================================="
        echo "Program terminated successfully!"
        
        # Check for generated files
        if [ -f "credit.dat" ]; then
            echo "✓ Data file created: credit.dat"
            ls -lh credit.dat
        fi
        
        if [ -f "accounts.txt" ]; then
            echo "✓ Export file created: accounts.txt"
            ls -lh accounts.txt
        fi
    else
        echo "ERROR: Compilation succeeded but executable not found!"
        exit 1
    fi
else
    echo "✗ Compilation FAILED!"
    echo ""
    echo "Common solutions:"
    echo "1. Ensure GCC is installed:"
    echo "   Linux/macOS: brew install gcc"
    echo "   Windows: Install MinGW or Dev-C++"
    echo ""
    echo "2. Check file permissions:"
    echo "   chmod +x trans.c"
    echo ""
    echo "3. For Windows users:"
    echo "   - Use MinGW command prompt"
    echo "   - Or compile with: gcc -o trans.exe trans.c"
    exit 1
fi
