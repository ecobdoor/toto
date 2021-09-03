
mkdir /storage/4419-5442/Android/data/com.termux/files/PILOT
cd /storage/4419-5442/Android/data/com.termux/files/PILOT
#echo " pid `cat pid`"
#cat pid
echo "    Copying `cp -v pilot.sh ~`"
echo "    Copying `cp -v pilot.cfg ~`"
echo "    Copying `cp -v menu.sh ~`"
cd ~
echo ""
source pilot.cfg
echo "mmmmmmmmmmmmmmmmmm"
read car
exit #ls -als
ps -flax
#while read process; do; echo "$process"; done
#./rkill.sh < /storage/4419-5442/Android/data/com.termux/files/PILOT/pid
./menu.sh
exit 
./pilot.sh
echo ""
echo ""
echo ""
echo ""
echo "=== Updating Termux ================"
echo ""
apt update
echo ""
echo "=== Updating Termux ================"
echo ""
apt upgrade
echo ""
echo "=== Installing PILOT interface... ==="
echo ""
