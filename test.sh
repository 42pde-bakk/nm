FILES=$@
OPTIONS="-r"

exit_status=0

echo "Files: $FILES"

# Good testfile: /usr/lib/llvm-11/lib/libLLVMAArch64CodeGen.a
for filename in $FILES; do
  if [[ $OPTIONS ]]; then
    echo "Calling nm on" "'$filename'" "with options $OPTIONS"
    ./ft_nm "$OPTIONS" "$filename" > /tmp/diff1.txt 2> /dev/null
    nm "$OPTIONS" "$filename" > /tmp/diff2.txt 2> /dev/null
  else
    echo "Calling nm on" "'$filename'"
    ./ft_nm "$filename" > /tmp/diff1.txt 2> /dev/null
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
