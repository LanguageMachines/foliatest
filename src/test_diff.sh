#! /bin/bash

RED="\033[1;31m"
BLUE="\033[1;34m"
NORMAL="\033[0m"
OK="\033[1;32m OK  \033[0m"
FAIL="\033[1;31m  FAILED  \033[0m"

host=`hostname`
if [ "$host" == "bonus" ] || [ "$host" == "kokos" ]
then
    prefix="/home/sloot/usr/local/bin"
else
    wd=`pwd`
    prefix="${wd}/../usr/local/bin"
fi

lint="$prefix/folialint"
#echo "lint=$lint"
if [ ! -e $lint ]
then
    echo "$lint not found"
    exit 1
fi

validate=`command -v foliavalidator`
#echo "validate=$validate"
if [ ! $validate ]
then
    echo "$validate not found"
    exit 1
fi

checkdir="../FoLiA/examples"
if [ ! -e $checkdir ]
then
    echo "$checkdir not found"
    exit 1
fi

err_count=0;

check_files () {
    total=0;
    for app in $1
    do
	total=$((total+1))
	lint_ok=1
	base="$(basename -- $app)"
	$lint $app > /tmp/lint.$base.1  2> /dev/null
	grep -v "libfolia" /tmp/lint.$base.1 > /tmp/lint.$base.2
	grep -v "folialint" /tmp/lint.$base.2 > /tmp/lint.$base
	$validate --keepversion -o $app > /tmp/val.$base.1  2>/dev/null
	grep -v "foliapy" /tmp/val.$base.1 > /tmp/val.$base.2
	grep -v "foliavalidator" /tmp/val.$base.2 > /tmp/val.$base
	diff -w -B --ignore-matching-lines=".*encoding=.*" --ignore-matching-lines=".*stylesheet .*" --ignore-matching-lines=".*<FoLiA .*" --ignore-matching-lines=".*<!--.*"  --ignore-matching-lines=".*datetime=.*"  --ignore-matching-lines=".*annotation .*" /tmp/lint.$base  /tmp/val.$base > /tmp/$base.diff
	if [ $? = 0 ]
	then
	    rm /tmp/$base.diff
	    echo -n "."
	else
	    err_count=$((err_count+1))
	    echo -e "\ndifferences for $app"
	fi
    done
}

echo -e "\nComparing foliavalidator and folialint XML results."
check_files "$checkdir/*.folia.xml"

if [ $err_count -ne 0 ]
then
    echo -e "\nthere were $err_count problems"
    exit 1
else
    exit 0
fi
