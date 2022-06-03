FILES=$@
OPTIONS=""

exit_status=0
executable="ft_nm"

echo "Executable: ${executable}"
echo "Files: $FILES"

# Good testfile: /usr/lib/llvm-11/lib/libLLVMAArch64CodeGen.a
for filename in $FILES; do
  if [[ $OPTIONS ]]; then
    echo "Calling nm on" "'$filename'" "with options $OPTIONS"
    ./${executable} "$OPTIONS" "$filename" > /tmp/diff1.txt 2> /dev/null
    nm "$OPTIONS" "$filename" > /tmp/diff2.txt 2> /dev/null
  else
    echo "Calling nm on" "'$filename'"
    ./${executable} "$filename" > /tmp/diff1.txt 2> /dev/null
    nm "$filename" > /tmp/diff2.txt 2> /dev/null
  fi

  cp /tmp/diff{1,2}.txt .
  diff diff1.txt diff2.txt
  diff_statuscode=$?
  if [[ $diff_statuscode -ne 0 ]]; then
    exit_status=$diff_statuscode
  fi
done

exit $exit_status
