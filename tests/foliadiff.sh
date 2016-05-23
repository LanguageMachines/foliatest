# /bin/sh -x

if [ "$my_bin" = "" ];
then
    my_bin="/usr/local/bin"
fi

if [ $# -ne 2 ]
  then
      echo "two arguments expected"
      exit 1
fi

t1=/tmp/${1##*/}.s1
t2=/tmp/${2##*/}.s2
$my_bin/folialint --strip "$1" > $t1
$my_bin/folialint --strip "$2" > $t2
xmldiff $t1 $t2
if [ $? -ne 0 ]
then
    echo "$1 and $2 are different"
    exit 1
fi

exit 0
