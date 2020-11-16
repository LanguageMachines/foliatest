# /bin/sh -x
if [ -z "$exe" ]
then
    if which folialint >/dev/null 2>/dev/null; then
        exe=$(which folialint)
    fi
fi

if [ -z "$exe" ]; then
    "cannot locate folialint"
    exit 1
fi

if [ $# -ne 2 ]
  then
      echo "two arguments expected"
      exit 1
fi

t1=/tmp/${1##*/}.s1
t2=/tmp/${2##*/}.s2
$exe --strip --KANON "$1" --output $t1
$exe --strip --KANON "$2" --output $t2
diff $t1 $t2
if [ $? -ne 0 ]
then
    echo "foliadiff.sh: differences in: $1 $2"
    exit 1
fi

exit 0
