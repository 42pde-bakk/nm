MAGENTA=$'\e[1;35m'
END=$'\e[0m'

lib="libftprintf.a"

mkdir -p bin log
set -e
function run_test() {
  TYPE=$1
  FLAG=$2
  mkdir -p "bin/${TYPE}" "log/${TYPE}"
  testFile="tests/${TYPE}/${FLAG}.c"
  ogEXE="bin/${TYPE}/${FLAG}_og.out"
  ftEXE="bin/${TYPE}/${FLAG}_ft.out"
  ogTxt="log/${TYPE}/${FLAG}_og.log"
  ftTxt="log/${TYPE}/${FLAG}_ft.log"

  cc ${lib} "${testFile}" -o "${ogEXE}" -I include
  cc ${lib} "${testFile}" -o "${ftEXE}" -D MINE -I include
  ./"$ogEXE" > "$ogTxt"
  ./"${ftEXE}" > "$ftTxt"
  diff "$ogTxt" "$ftTxt" || (echo "${TYPE}_${FLAG} failed" && exit 1)
}

# Run String Tests
run_test "string" "easy"
run_test "string" "minus"
run_test "string" "width"
run_test "string" "precision"
run_test "string" "asterisk"
run_test "string" "mix"

# Run Char Tests
run_test "char" "easy"
run_test "char" "minus"
run_test "char" "width"
run_test "char" "asterisk"
run_test "char" "mix"

echo "${MAGENTA}Congratulations, you passed the tests!${END}"
