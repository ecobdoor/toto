if [ $# == 0 ];then
    source env.sh
else
    source $1
fi
error=$?
#echo "$error"; pwd
ltcyan="\e[1;36m"
quit=false
relaunch=false
while(true);do
    clear
    echo "╭────────────────────────────────────────────────╮"
    echo "│        ROVER utilities 1.02   (user:`whoami`) "
    echo "╰────────────────────────────────────────────────╯"
    PS3=$'\e[01;33mSélection : \e[0m'

    select opt in "QUITTER" "tm update" "tm upgrade" "Release" "PILOT"  "SERVER" ml ml ml ml ml m l "lmkj"  ml ml ml ml ml ml m QUIT; do
        error=false
        case $opt in
            "Release")
                default="YESOUI"
                echo -en "Mettre à jour les services rover ? [Y,n]";  read car
                #if [ "$default" == *"${car}"* ]; then
                if [ "y" == "$car" ]; then
                    cd "${SRC_PILOT_FOLDER}"
                    echo "    Copying `cp -v install_remote.sh ~`"
                    cd ~
                    ./install_remote.sh
                    pwd
                    relaunch=true
                fi
                ;;
            "QUITTER")
                quit=true
                ;;
            "PILOT")
                ./pilot.sh
                ;;
            "SERVER")
                cd "${USR_PILOT_FOLDER}"
                pwd
                echo "pwd='`pwd`'"
                node server.js
                cd ~
                ;;
            "tm update")
                apt update
                ;;
            "tm upgrade")
                apt upgrade
                ;;
            *) 
            error=true
            ;;
        esac
        #echo -en "'$error' "
        if [ "$error" == true ]; then
            echo -en "Choix invalide '$REPLY'. Tapez <─┘";  read car 
        else 
            echo -en "Terminé. Tapez <─┘";  read car 
            if [ "$quit" == true ]; then
                exit
            fi
            if [ "$relaunch" == true ]; then
                exec $0
            fi
        fi
        break
    done
done
exit

#ssh u0_a738@192.168.13.12 -p 8022
╭──────╮
if [ $error != 0 ];then
    echo    "    ERROR: you are not in the good folder"
    echo    "        Edit this script file & make the first line ok."
    echo -n "        Hit the Enter key to exit"; read car
    exit
fi
#ls -als --color=auto
echo "    Termux must be launched on the pilot smartphone & running sshd"
echo -n "    Hit the Enter key to upload"; read car
scp -p -P 8022 *.* u0_a738@192.168.13.12:/data/data/com.termux/files/home/storage/external-1/PILOT

echo -n "=== Hit the Enter key to exit"; read car
exit
╭──────╮
│ test │  
╰──────╯
┌┐└┘═╔╗╚╝║
─│┌┐└┘├┤┬┴┼═║╒╓╔╕╖╗╘╙╚╛╜╝╞╟╠╡╢╣╤╥╦╧╨╩╪╫╬