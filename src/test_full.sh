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
    reversed=$2
#    echo "reversed=$reversed"
    for app in $1
    do
	total=$((total+1))
	lint_ok=1
	$lint $app > /dev/null 2>&1
	if [ $? -ne 0 ]
	then
	    lint_ok=0
	fi
	val_ok=1
	$validate $app > /dev/null 2>&1
	if [ $? -ne 0 ]
	then
	    val_ok=0
	fi
#	echo "lint=$lint_ok and val=$val_ok"
	if [ $reversed ]
	then
	    if [ $lint_ok = 1 ]
	    then
		lint_ok=0
	    else
		lint_ok=1
	    fi
	    if [ $val_ok = 1 ]
	    then
		val_ok=0
	    else
		val_ok=1
	    fi
	fi
#	echo "NOW lint=$lint_ok and val=$val_ok"
	if [ $lint_ok = 1 ] && [ $val_ok = 1 ]
	then
	    echo -n "."
	else
	    err_count=$((err_count+1))
	    if [ $lint_ok = 1 ]
	    then
		if [ $reversed ]
		then
		    echo -e "\n[$total] foliavalidator $FAIL to reject $app"
		else
		    echo -e "\n[$total] foliavalidator $FAIL to accept $app"
		fi
	    else
		if [ $val_ok = 1 ]
		then
		    if [ $reversed ]
		    then
			echo -e "\n[$total] folialint $FAIL to reject $app"
		    else
			echo -e \n"[$total] folialint $FAIL to accept $app"
		    fi
		else
		    if [ $reversed ]
		    then
			echo -e "\n[$total] BOTH $FAIL to reject $app"
		    else
			echo -e "\n[$total] BOTH $FAIL to accept $app"
		    fi
		fi
	    fi
	fi
    done
}

echo "Comparing foliavalidator and folialint results. Both should accept all"
check_files "$checkdir/*.folia.xml"
if [ ! $verbose ]
then
    echo " "
fi
echo "Comparing foliavalidator and folialint results. Both should reject all"
check_files "$checkdir/erroneous/*.folia.xml" "1"

if [ $err_count -ne 0 ]
then
    echo -e "\nthere were $err_count problems"
    exit 1
else
    exit 0
fi
