#! /bin/bash

RED="\033[1;31m"
BLUE="\033[1;34m"
NORMAL="\033[0m"
OK="\033[1;32m OK  \033[0m"
FAIL="\033[1;31m  FAILED  \033[0m"

echo "Running and comparing foliavalidator and folialint"

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
    exit
fi

validate=`command -v foliavalidator`
#echo "validate=$validate"
if [ ! $validate ]
then
    echo "$validate not found"
    exit
fi

checkdir="../FoLiA/examples"
if [ ! -e $checkdir ]
then
    echo "$checkdir not found"
    exit
fi

check_files () {
    total=0;
    for app in $1
    do
	total=$((total+1))
	$lint $app > /dev/null 2>&1
	lint_ok=1
	if [ $? -ne 0 ]
	then
	    lint_ok=0
	fi
	$validate $app > /dev/null 2>&1
	val_ok=1
	if [ $? -ne 0 ]
	then
	    val_ok=0
	fi
	if [ $lint_ok ] && [ $val_ok ]
	then
	    echo -e "[$total] $app $OK"
	else
	    if [ $lint_ok ]
	    then
		echo -e "[$total] foliavalidator $FAIL on $app"
	    else
		if [ $val_ok ]
		then
		    echo -e "[$total] folialint $FAIL on $app"
		else
		    echo -e "[$total] BOTH $FAIL on $app"
		fi
	    fi
	fi
    done
}

check_files "$checkdir/*.folia.xml"

check_files "$checkdir/erroneous/*.folia.xml"
