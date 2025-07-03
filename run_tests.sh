#!/bin/bash

if [ ! -d "./tests" ]; then
    echo "Creating ./tests directory..."
    mkdir ./tests
fi

token_counts=(100000 1000000 10000000 20000000 50000000) # High load testing

declare -a inputs=()

for i in {0..5}; do
    tokens=${token_counts[$((i-1))]}
    filepath="./tests/input-${i}.easyc"

    if [ -f "$filepath" ]; then
        echo "Input $i already exists at $filepath. Skipping generation."
    else
        echo -e "\nGenerating Input $i with $tokens tokens..."
        time bash generate_input.sh --max-tokens "$tokens" > "$filepath"
    fi

    inputs+=("$filepath")
done

echo "Inputs created."

echo "Building project..."
if ! (make clean > /dev/null && make -B > /dev/null); then
    echo "Build failed. Exiting."
    exit 1
fi
echo "Build completed."

for file in "${inputs[@]}"; do
    echo "Processing file: $file"
    echo "OnlySyntaxCheck"
    ./bin/main --only-syntax-check --execute-table-driven -i "$file"
    echo ""
    ./bin/main --only-syntax-check --execute-recursive-descent -i "$file"
    echo ""
    echo "WithParseTree"
    ./bin/main  --execute-table-driven -i "$file"
    echo ""
    ./bin/main  --execute-recursive-descent -i "$file"
done