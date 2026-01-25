#!/bin/bash

# Clean up previous compilation results
rm -f *.class *.java *.jar 2>/dev/null

# Generate list of all COBOL files
files=""
for i in {1..100}; do
  filename=$(printf "p%03d.cbl" $i)
  files="$files $filename"
done

echo "Compiling 100 COBOL programs with single cobj command..."
echo "Command: ../cobj/cobj $files"
echo ""

# Measure compilation time
time ../cobj/cobj $files

echo ""
echo "Compilation complete!"
