#!/bin/ksh
# trivial file-based locking

lock()
{
	if [ $1 ]; then
		lockfile=.${1}-lock
	else
		lockfile=.${function}-lock
	fi

	[[ -f $lockfile ]] && return 1
	touch $lockfile
	echo lock $function
	return 0 
}

f1()
{
	local function="f1"
	lock
	f2
	unlock
}

f2()
{
	local function="f2"
		
}

unlock()
{
	lockfile=.${function}-lock
	[[ -f $lockfile ]] && echo "unlocking" && rm -f $lockfile
	echo unlock $function
}

use()
{
	local function="use"
	lock
	use
	f1
	unlock
}

use
