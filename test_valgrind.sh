#!/bin/bash

# Valgrind Testing Script for Minishell
# This script runs a comprehensive memory leak test

echo "🔧 Building minishell..."
make clean && make

if [ ! -f "./minishell" ]; then
    echo "❌ Build failed!"
    exit 1
fi

echo "🧪 Running Valgrind memory leak test..."
echo "This will test basic commands and check for memory leaks"
echo ""

# Create a test input file
cat > test_input.txt << 'EOF'
echo hello
pwd
export VAR=test
echo $VAR
unset VAR
ls -l
exit
EOF

# Run Valgrind with the test commands
valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --log-file=valgrind_test.log \
         ./minishell < test_input.txt

echo ""
echo "📊 Valgrind Results Summary:"
echo "================================"

# Extract key information from the log
if [ -f "valgrind_test.log" ]; then
    echo "✅ Definitely lost: $(grep -o 'definitely lost: [0-9,]* bytes' valgrind_test.log)"
    echo "✅ Indirectly lost: $(grep -o 'indirectly lost: [0-9,]* bytes' valgrind_test.log)"
    echo "⚠️  Still reachable: $(grep -o 'still reachable: [0-9,]* bytes' valgrind_test.log)"
    echo ""
    
    # Check if there are any actual leaks (not just readline)
    definitely_lost=$(grep -o 'definitely lost: [0-9,]* bytes' valgrind_test.log | grep -o '[0-9,]*' | tr -d ',')
    indirectly_lost=$(grep -o 'indirectly lost: [0-9,]* bytes' valgrind_test.log | grep -o '[0-9,]*' | tr -d ',')
    
    if [ "$definitely_lost" = "0" ] && [ "$indirectly_lost" = "0" ]; then
        echo "🎉 SUCCESS: No memory leaks detected in your code!"
        echo "   (Readline library leaks are expected and acceptable)"
    else
        echo "❌ WARNING: Memory leaks detected in your code!"
        echo "   Check valgrind_test.log for details"
    fi
else
    echo "❌ Valgrind log file not found"
fi

# Clean up
rm -f test_input.txt

echo ""
echo "📁 Full report saved in: valgrind_test.log"
echo "💡 To view detailed report: cat valgrind_test.log"



