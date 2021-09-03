echo ""
echo "╭────────────────────────────────────────────────────╮"
echo "│ Installation SSH des scripts PILOT sur smartphone  │"
echo "╰────────────────────────────────────────────────────╯"
echo ""
error=""
if [ ! -d "$1" ]; then
	error=${error}"SD card '$1' not found on smartphone\n"
  	error=${error}"Edit the source script 'install_host.sh' & adapt the last argument"
  	error=${error}"    -For Samsung:  it's      storage/external-1\n"
  	error=${error}"    -For .......:  it's      ??????????????????\n"
else
  echo -e "Installing scripts from smartphone '$1' SD card...\n"
fi
if [  ! -d "$1/PILOT" ] && [ "$error" == "" ];then
  	error=${error}"folder 'PILOT' not found on smartphone SD card root\n"
  	error=${error}"You must first copy the PILOT source folder to the smartphone's SD card:\n"
  	error=${error}"    -Either with an USB cable (accept mtp mode when connecting)\n"
  	error=${error}"    -Or with FTP (client on your PC & server on smartphone)\n"
  	error=${error}"    -Or with SCP update.sh script (not recursive for sub folders!!!)\n"
  	error=${error}"    -Or on the PC by removing the SD card from the smartphone"
fi
if [ "$error" == "" ];then
	#echo "-----------  pwd";pwd
	#rm pilot.cfg
	#rm install_remote.sh
	#echo "-----------  ls -als ~";ls -als ~

	# Update env.sh !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! with $1
	cp "$1/PILOT/env.sh" "$1/PILOT/env.sh.bak"
	sed -i "1s|.*|GUEST_SDCARD=\"$1\" # MUST BE THE FIRST LINE !!!|" "$1/PILOT/env.sh"
	#ls "$1/PILOT"
	#cat "$1/PILOT/env.sh"

	cd "$1/PILOT"
	echo "    Copying `cp -v env.sh ~`"
	source env.sh

	#echo "----------------------------"
	#cat env.sh
	#echo "----------------------------"
	#cat "${USR_PILOT_FOLDER}/env.sh"
	#echo "----------------------------"
	#echo " !!!   USR_PILOT_FOLDER='${USR_PILOT_FOLDER}'"
	#ls -C --color=always "${USR_PILOT_FOLDER}"
	echo "    Copying `cp -v pilot.sh ~`"
	echo "    Copying `cp -v menu.sh ~`"
	cd ~
fi
if [ "$error" == "" ];then
	echo ""
	echo "╭────────────────────────────────────────────────────╮"
	echo "│ Scripts utilitaires correctement installés         │"
	echo "│ Entrez dans Termux et lancer la commande ./menu.sh │"
	echo "╰────────────────────────────────────────────────────╯"
	echo ""
else
	echo ""
	echo "╭────────────────────────────────────────────────────╮"
	echo -e "ERROR $error"
	echo "╰────────────────────────────────────────────────────╯"
	echo ""
fi