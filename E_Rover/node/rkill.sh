#!/bin/bash
echo "==== Searching for $1 children"
ps -flax
# ps --forest -o pid=,ppid=,tty=,stat=,time=,cmd= -g $(ps -o sid= -p 16111)

### Current PID, if this script try to self search, then will loop forever
pid=$$

### Recursive function
search()
{
    if [ $1 == $pid ]; then
        break
    fi

    pgrep -P $1 |
    while read process; do
        echo "`ps w -p $process | tail -n1`" $process ### Print PID to stdout for debug, replace by your kill command (kill -9, pkill, etc)
        search $process
    done
}

search $1