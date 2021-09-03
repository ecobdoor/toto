source env.sh
echo ""
echo "╭────────────────────────────────────────────────────╮"
echo "│ Copie câble USB du PILOTE sur carte SD smartphone  │"
echo "╰────────────────────────────────────────────────────╯"
echo ""
cd ${SRC_PILOT_FOLDER}
error=$?
#echo "$error"; pwd
if [ $error != 0 ];then
    echo    "    ERROR $error! source folder not found:"
    echo    "        - Edit the file 'env.sh' & make SRC_PILOT_FOLDER ok."
    echo -n "        - Hit the Enter key to exit, edit & restart"; read car
    exit
fi
#ls -als --color=auto
echo "    Termux must be launched on the smartphone & running sshd"
echo -n "    Hit the Enter key to upload"; read car
scp -p -P ${GUEST_PORT} *.* ${GUEST_USER}@${GUEST_IP}:${USR_PILOT_FOLDER}
error=$?
if [ $error != 0 ];then
    echo    "    ERROR $error! "
    exit
fi

echo -n "=== Hit the Enter key to exit"; read car
exit
    echo    "        - Verify your USB cable is correctly connected"

╭──────╮
│ test │  
╰──────╯
┌┐└┘═╔╗╚╝║