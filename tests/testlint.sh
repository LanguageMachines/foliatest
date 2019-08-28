#!/bin/bash
RED="\033[1;31m"
BLUE="\033[1;34m"
NORMAL="\033[0m"
OK="-e \033[1;32m Both OK  \033[0m"
AGREE="-e \033[1;34m  Both REJECT  \033[0m"
DISAGREE="-e \033[1;33m  DISAGREE  \033[0m"

if [ "$folialint" = "" ]
then
    folialint="$HOME/usr/local/bin/folialint"
    if [ ! -f $folialint ]
    then
	folialint="/exp/sloot/usr/local/bin/folialint"
	if [ ! -f $folialint ]
	then
	    folialint="/usr/local/bin/folialint"
	    if [ ! -f $folialint ]
	    then
		folialint="/usr/bin/folialint"
	    fi
	fi
    fi
fi

if [ "$folialint" = "" ];
then
    "cannot locate folialint"
    exit 1
fi

if [ "`command -v foliavalidator`" = "" ];
then
    "cannot locate foliavalidator"
    exit 1
fi

function compare {
    lcnt=0
    vcnt=0
    opts=$2
    for f in $1
    do
	echo -n "checking $f"
	f_base=${f##*/}
	$folialint $opts --nooutput $f &> $f_base.l.err
	l_stat=$?
	if [ $l_stat -ne 0 ]
	then
#	    echo "folialint: trouble with: " $f
	    lcnt=$((lcnt+1))
	fi
	foliavalidator $opts -W $f &> $f_base.v.err
	v_stat=$?
	if [ $v_stat -ne 0 ]
	then
#	    echo "foliavalidator: trouble with: " $f
	    vcnt=$((vcnt+1))
	fi
	if [ $l_stat == 0 -a $v_stat == 0 ]
	then
	    rm $f_base.l.err
	    rm $f_base.v.err
	    echo $OK
	else
	    if [ $l_stat != $v_stat ]
	    then
		echo $DISAGREE
		echo "see $f_base.l.err"
		echo "see $f_base.v.err"
	    else
		rm $f_base.l.err
		rm $f_base.v.err
		echo $AGREE
	    fi
	fi
    done
}

#
# first a check were folialint and foliavalidator shouldn't find any problems
#
FILES=../../FoLiApy/folia-repo/examples/*.xml

compare "${FILES[@]}"

#
# now a check were folialint and foliavalidator should find problems
#
FILES=../../FoLiApy/folia-repo/examples/erroneous/*.xml

compare "${FILES[@]}"

#
# now a check were folialint and foliavalidator should fix the problems
#
FILES=../../FoLiApy/folia-repo/examples/erroneous/*.xml

compare "${FILES[@]}" -a

#
# now a check were folialint and foliavalidator should be fine
#
FILES=../../FoLiApy/folia-repo/examples/tests/*.xml

compare "${FILES[@]}"
