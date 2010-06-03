#!/bin/ksh
# remote command execution

# $1 = host
# $2 = command

remote() {
        local host=$1 cmd ret

        [ -z $1 ] && host="localhost" || host=$1
        [ -z $2 ] && cmd="ls" || cmd=$2
        [ -z $REMOTESHELL ] && REMOTESHELL=rsh

        echo $cmd > /tmp/cmd.$$
        ret=$(${REMOTESHELL} ${host} "$cmd > /tmp/out.$$ 2> /tmp/err.$$; echo \$?")
        _OUT=$(${REMOTESHELL} ${host} "cat /tmp/out.$$; rm /tmp/out.$$")
        _ERR=$(${REMOTESHELL} ${host} "cat /tmp/err.$$; rm /tmp/err.$$")
        return ${ret}

}
