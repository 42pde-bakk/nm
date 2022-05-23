FILES="$1"
OPTIONS=$"$2"

exit_status=0

for filename in $FILES; do
  echo "Calling nm on" "'$filename'" "with options $OPTIONS"
  if [[ $OPTIONS ]]; then
    ./ft_nm "$OPTIONS" "$filename" > /tmp/diff1.txt 2> /dev/null
    nm "$OPTIONS" "$filename" > /tmp/diff2.txt 2> /dev/null
  else
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
