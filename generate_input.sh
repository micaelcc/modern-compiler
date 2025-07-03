#!/bin/bash

INSTRUCTIONS=(
  "let a = b + c * 5 / 10 * (4 + 5);::18"
  "a = d + c * 5 / 10 * (c + b);::17"
  "let result = 42;::6"
  "x = y - 2 * (z + 1);::13"
  "let x = (a + b) * c;::12"
  "let arr = [1, 2, 3, 4];::14"
  "let b = [1, 2, 3] + a[1] + 1;::19"
  "let msg = \"TEXT\";::6"
  "let pow = (x + y) ^ 2;::12"
  "arr[2] = x * y;::10"
)

BOOL_CONDITIONS=(
  "x > 3::4"
  "y < 5::4"
  "z == 10::4"
  "x + y > z::6"
  "x > 3 && y < 5 || z > 10::12"
  "x > z || y < 5 || z > 10 && z >= 10::16"
)

extract_code() {
  echo "${1%%::*}"
}

extract_tokens() {
  echo "${1##*::}"
}

random_instr() {
  local entry="${INSTRUCTIONS[$((RANDOM % ${#INSTRUCTIONS[@]}))]}"
  echo "$entry"
}

random_bool() {
  local entry="${BOOL_CONDITIONS[$((RANDOM % ${#BOOL_CONDITIONS[@]}))]}"
  echo "$entry"
}

generate_block() {
  local max_instrs=$1
  local result=""
  local block_tokens=0

  for ((i = 0; i < max_instrs; i++)); do
    local line1=$(random_instr)
    result+="$(extract_code "$line1")"
    block_tokens=$((block_tokens + $(extract_tokens "$line1")))

    case $((RANDOM % 5)) in
      0|1)
        local line2=$(random_instr)
        result+="$(extract_code "$line2")"
        block_tokens=$((block_tokens + $(extract_tokens "$line2")))
        ;;
      2)
        local cond1=$(random_bool)
        local cond2=$(random_bool)
        local lineA=$(random_instr)
        local lineB=$(random_instr)
        local lineC=$(random_instr)
        result+="if ($(extract_code "$cond1")) {$(extract_code "$lineA")}"
        result+=" elseif ($(extract_code "$cond2")) {$(extract_code "$lineB")}"
        result+=" else {$(extract_code "$lineC")}"
        block_tokens=$((block_tokens + $(extract_tokens "$cond1") + $(extract_tokens "$cond2") + $(extract_tokens "$lineA") + $(extract_tokens "$lineB") + $(extract_tokens "$lineC") + 10)) # +10 pelos tokens de controle
        ;;
      3)
        local cond=$(random_bool)
        local line=$(random_instr)
        result+="while ($(extract_code "$cond")) {$(extract_code "$line")}"
        block_tokens=$((block_tokens + $(extract_tokens "$cond") + $(extract_tokens "$line") + 4))
        ;;
      4)
        local line=$(random_instr)
        result+="for (i = 0; i < 10; i = i + 1) {$(extract_code "$line")}"
        block_tokens=$((block_tokens + $(extract_tokens "$line") + 10))
        ;;
    esac
  done

  echo "$result::$block_tokens"
}

generate_until_max_tokens() {
  local max_tokens=$1
  local current_tokens=0
  local code=""

  while (( current_tokens < max_tokens )); do
    local chunk=""
    local chunk_tokens=0

    case $((RANDOM % 5)) in
      0|1|2)
        chunk=$(random_instr)
        ;;
      3)
        local cond=$(random_bool)
        local block=$(generate_block $((RANDOM % 3 + 1)))
        local block_code=$(extract_code "$block")
        local block_tokens=$(extract_tokens "$block")
        chunk="if ($(extract_code "$cond")) {$block_code}"
        chunk_tokens=$(( $(extract_tokens "$cond") + block_tokens + 3 ))
        ;;
      4)
        local cond=$(random_bool)
        local block=$(generate_block $((RANDOM % 3 + 1)))
        local block_code=$(extract_code "$block")
        local block_tokens=$(extract_tokens "$block")

        chunk="while ($(extract_code "$cond")) {$block_code}"
        chunk_tokens=$(( $(extract_tokens "$cond") + block_tokens + 3 ))
        ;;
    esac

    if [[ -z "$chunk_tokens" || "$chunk_tokens" == "0" ]]; then
      chunk_tokens=$(extract_tokens "$chunk")
      chunk=$(extract_code "$chunk")
    fi

    current_tokens=$((current_tokens + chunk_tokens))
    code+="$chunk"
  done

  echo "$code"
}

if [[ "$1" == "--max-tokens" && "$2" =~ ^[0-9]+$ ]]; then
  generate_until_max_tokens "$2"
elif [[ "$1" =~ ^[0-9]+$ ]]; then
  generate_block "$1" | cut -d'::' -f1
else
  echo "Usage:"
  echo "  $0 <n_instructions>            # Generates n instructions"
  echo "  $0 --max-tokens <n_tokens>     # Generates code with up to ~n_tokens tokens"
  exit 1
fi