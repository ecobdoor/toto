echo "╭────────────────────────────────────────────────╮"
echo "│          Lance le serveur PILOT                │"
echo "╰────────────────────────────────────────────────╯"
echo ""
./env.sh
cd "${USR_PILOT_FOLDER}"
echo "pwd='`pwd`'"
node server.js
#ls -als
exit
while (true); do
    echo ""; echo "--- Killing old node server... ---------------"; echo ""
    ps -ax -o pid=,ppid=,cmd=,ruser=RealUser,args=,exe=,pmem=,start= | grep "node server.js" | grep -v "grep" > pids
    cat pids
    cat pids | cut -d" " -f 1 | xargs -I % sh -c 'echo "Killing pid -> %"; kill %'
    echo ""; echo "--- Launching new node server... -------------"; echo ""
    node server.js
    if [ $? != 0 ]; then
        echo "    > Server launched right"
    else
        echo "    > Server launching error"
        ps -flax
        exit
    fi
    echo ""; echo "----------------------------------------------"; echo ""
done
