FILES="$1"
OPTIONS=$"$2"

exit_status=0

for filename in $FILES; do
  echo "Calling nm on" "'$filename'" "with options $OPTIONS"
  if [[ $OPTIONS ]]; then
    ./ft_nm "$OPTIONS" "$filename" > diff1.txt 2> /dev/null
    nm "$OPTIONS" "$filename" > diff2.txt 2> /dev/null
  else
    ./ft_nm "$filename" > diff1.txt 2> /dev/null
    nm "$filename" > diff2.txt 2> /dev/null
  fi

  diff diff1.txt diff2.txt
  diff_statuscode=$?
  if [[ $diff_statuscode -ne 0 ]]; then
    exit_status=$diff_statuscode
  fi
done

exit $exit_status

#for filename in $1; do
#  echo "$filename"
#  ./ft_nm "$2" "$filename" > diff1.txt 2> /dev/null
#  nm -p "$2" "$filename" > diff2.txt 2> /dev/null
#  diff diff1.txt diff2.txt
#done
