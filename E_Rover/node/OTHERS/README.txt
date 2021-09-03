

O2 use and|or configure ROVER client interface...

USER INSTALL:
=============
Smartphone


	Google Play Store
	Install Termux
	Lauch Termux

	$ apt update && apt upgrade
	$ apt install coreutils
	$ mkdir test-node && cd test-node
	$ apt install nano  #(vim)
	$ touch server.js 
	$ nano server.js #	console.log('This is Node.js running on Android.')
	$ cat server.js #$ console.log('This is Node.js running on Android.')
	$ apt install nodejs
	$ npm init
	$ apt update && apt upgrade # because of !!!! cannot link executable node
	$ node server.js

	Copy node folder on your externa SDcard for example /storage/4419-5442/Android/data/com.termux/files


	$ cd ~/storage/external-1

	$ ls -als 
	total 384
	128 drwxr-x--- 3 u0_a738 everybody 131072 Jan  7 19:00 .
	128 drwxr-x--- 3 u0_a738 everybody 131072 Dec  8 14:31 ..
	128 drwxr-x--- 2 u0_a738 everybody 131072 Jan  7 19:28 ROVER
	$ ls -als ROVER
	total 1664
	128 drwxr-x--- 2 u0_a738 everybody 131072 Jan  7 19:28 .
	128 drwxr-x--- 3 u0_a738 everybody 131072 Jan  7 19:00 ..
	128 -rwxr-x--- 1 u0_a738 everybody  14233 Jan  7 18:58 CENTER.jpg
	128 -rwxr-x--- 1 u0_a738 everybody   1074 Jan  7 18:58 EMERGENCY.gif
	128 -rwxr-x--- 1 u0_a738 everybody   1150 Jan  7 18:58 favicon.ico
	128 -rwxr-x--- 1 u0_a738 everybody   4145 Jan  7 18:58 index.html
	128 -rwxr-x--- 1 u0_a738 everybody   8850 Jan  7 18:58 pil.css
	128 -rwxr-x--- 1 u0_a738 everybody  12682 Jan  7 18:58 pil.html
	128 -rwxr-x--- 1 u0_a738 everybody 104785 Jan  7 18:58 pil.js
	128 -rwxr-x--- 1 u0_a738 everybody     13 Jan  7 18:58 rover.sh
	128 -rwxr-x--- 1 u0_a738 everybody    106 Jan  7 18:58 roverTestNodeServer_000.sh
	128 -rwxr-x--- 1 u0_a738 everybody   3267 Jan  7 18:58 server.js
	128 -rwxr-x--- 1 u0_a738 everybody    188 Jan  7 18:58 testNodeServer.sh
	$ 


	Wifi bdoor
	Launch Termux
	./rover.sh # Termux launch server.sh with while true (if server crash...)
	Chrome localhost:3001/pil.html  # not 192.168.13.12/...


USER DRIVE:
===========
Smartphone
	Wifi bdoor
	Launch Termux
	./rover.sh # Termux launch server.sh with while true (if server crash...)
	Chrome localhost:3001/pil.html  # not 192.168.13.12/...


DEVELOPPER:
===========
Smartphone
	Wifi bdoor
	Launch FTP server
	Launch Termux
	Termux sshd # Launch ssh server
	Connect USB cable
	Accept data access
Host desktop
	Nautilus via USB to upload|download to|from host to mtp://SAMSUNG_SAMSUNG_Android_520041e0ececc493/Card/Android/data/com.termux/files/ROVER

Host console tab 1 
	Run scrcpy
Host console tab 2 Host console 
	ssh u0_a738@192.168.13.12 -p8022 # 3655
	df # to see
	ls # to see
	tree -puh


	$ cd ~
	$ pwd
		/data/data/com.termux/files/home
	$ ls -als
		total 64
		4 drwx------ 9 u0_a738 u0_a738 4096 Jan  7 17:23 .
		4 drwxrwx--x 4 u0_a738 u0_a738 4096 Dec  8 12:33 ..
		4 -rw------- 1 u0_a738 u0_a738 3613 Jan  4 17:17 .bash_history
		4 drwx------ 4 u0_a738 u0_a738 4096 Dec 27 17:44 .npm
		4 -rw------- 1 u0_a738 u0_a738   24 Jan  7 17:23 .npmrc
		4 drwx------ 2 u0_a738 u0_a738 4096 Dec 24 13:39 .ssh
		4 drwx------ 3 u0_a738 u0_a738 4096 Dec 27 15:57 .suroot
		4 drwx------ 2 u0_a738 u0_a738 4096 Dec 15 01:09 .termux
		4 -rw------- 1 u0_a738 u0_a738   20 Dec 24 13:52 .termux_authinfo
		4 drwx------ 4 u0_a738 u0_a738 4096 Dec 27 17:44 node_modules
		4 -rw------- 1 u0_a738 u0_a738  768 Dec 27 17:44 package-lock.json
		4 -rw------- 1 u0_a738 u0_a738   49 Dec 27 17:44 package.json
		4 -rwxrwxrwx 1 u0_a738 u0_a738   85 Jan  4 16:54 rover.sh
		4 -rw------- 1 u0_a738 u0_a738   57 Jan  4 17:03 server.js
		4 drwx------ 3 u0_a738 u0_a738 4096 Dec  8 14:35 storage
		4 drwx------ 2 u0_a738 u0_a738 4096 Dec  8 12:56 test-node
	$


#	cd ~/storage/4419-5442/NODE.JS

	cd ~/storage/external-1
	tree -puh

	./rover.sh # launch server.sh with while true (if server crash...)
Smartphone
	Chrome localhost:3001/pil.html  # no 192.168.13.12/...




echo "edit to understand"
	NON: FileZilla 192.168.13.12 # goto /storage/4419-5442/NODE.JS
read car
exit
https://smart-tech.mg/comment-controler-et-mettre-en-miroir-un-telephone-android-dans-ubuntu/
sudo snap install scrcpy
connect usb smartphone
scrcpy  # controle du smartphone à ditance
ls /storage/4419-5442
