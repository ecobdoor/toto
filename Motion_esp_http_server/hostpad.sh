#!/bin/bash

#nm-connection-editor

iptables -A FORWARD -i $ETH -o $VLAN -j ACCEPT
iptables -A FORWARD -o $ETH -i $VLAN -j ACCEPT
iptables -A FORWARD -o $ETH -i $ETH -j ACCEPT
iptables -A FORWARD -o $VLAN -i $VLAN -j ACCEPT
iptables -t nat -A POSTROUTING -o $ETH -j MASQUERADE
iptables -t nat -A POSTROUTING -o $VLAN -j MASQUERADE
read car 
exit
WLAN="wlx801f02e69bf8"
ETH="eno1"
if [ $UID == "0" ]
then
	#kill des processus
	killall hostapd
	#Attente de Xs pour être sur que le serveur est démarré
	sleep 5
	#reinitialisation wifi
	ifconfig $VLAN down
	ifconfig mon.$VLAN down
	ifconfig $VLAN 10.42.0.0 netmask 255.255.255.0 up
	#reinitialisation des serveurs dhcp et dns
	service isc-dhcp-server restart
	service bind9 restart
	#demarrage du point d'acces
	hostapd /etc/hostapd-1.0/hostapd/hostapd.conf >> /hostapd
	ifconfig $VLAN down
else
	echo "Vous devez etre root pour lancer ce script !";
fi

exit



iptables -A FORWARD -i $ETH -o $VLAN -j ACCEPT
iptables -A FORWARD -o $ETH -i $VLAN -j ACCEPT
iptables -A FORWARD -o $ETH -i $ETH -j ACCEPT
iptables -A FORWARD -o $VLAN -i $VLAN -j ACCEPT
iptables -t nat -A POSTROUTING -o $ETH -j MASQUERADE
iptables -t nat -A POSTROUTING -o $VLAN -j MASQUERADE

https://doc.ubuntu-fr.org/hostapd

https://ubuntu.pkgs.org/20.04/ubuntu-universe-arm64/hostapd_2.9-1ubuntu4_arm64.deb.html

https://forum.ubuntu-fr.org/viewtopic.php?id=1235271

J'ai fini par trouver a force de chercher et faire des tests:
Je vous explique en fait ce que j'ai configuré avec mon rc.local et d'autres petit scripts perso capable de gérer tout ça, et ce que j'ai modifié dans la config du serveur maintenant fonctionnel.
Vous pouvez ré-utiliser tout ceci librement, ça pourrait je pense aider plus d'une personne si quelqu'un avec un certain talent de pédagogue le rédige simplement dans le Wiki d'Ubuntu-fr smile
Pour rappel, le serveur qui fait NAT est un DHCP, la freebox ne le fait plus (coupé) et il fait aussi serveur DNS maître, donc je n'ai aucun serveur de ce genre a rajouter, juste un petit peu de config a modifier. Si vous n'en avez pas, il faudra configurer ça en conséquences.
Donc, il faut:

route add default gw 192.168.1.1

Sur le serveur pour s'assurer que eth0 aille bien vers ma box. (passerelle, car le reste du réseau aura le serveur en passerelle, il faut donc pas que le serveur se connecte a lui même comme passerelle)
Ensuite:

iptables -A FORWARD -i eth0 -o wlan0 -j ACCEPT
iptables -A FORWARD -o eth0 -i wlan0 -j ACCEPT
iptables -A FORWARD -o eth0 -i eth0 -j ACCEPT
iptables -A FORWARD -o wlan0 -i wlan0 -j ACCEPT
iptables -t nat -A POSTROUTING -o eth0 -j MASQUERADE
iptables -t nat -A POSTROUTING -o wlan0 -j MASQUERADE

Autoriser les connexions dans tous les sens entre les interfaces du serveur, et activer le MASQUERADE
Note: Souvent les gens sur le net oublient l'option "-t nat" et iptables ne connais pas de table POSTROUTING dans "-t filter" utilisée par défaut.

Ensuite seulement, le Wifi est démarré:

#!/bin/bash
if [ $UID == "0" ]
then
	#kill des processus
	killall hostapd
	#Attente de Xs pour être sur que le serveur est démarré
	sleep $1
	#reinitialisation wifi
	ifconfig wlan0 down
	ifconfig mon.wlan0 down
	ifconfig wlan0 192.168.2.11 netmask 255.255.255.0 up
	#reinitialisation des serveurs dhcp et dns
	service isc-dhcp-server restart
	service bind9 restart
	#demarrage du point d'acces
	hostapd /etc/hostapd-1.0/hostapd/hostapd.conf >> /hostapd
	ifconfig wlan0 down
else
	echo "Vous devez etre root pour lancer ce script !";
fi

L'attente avec sleep est fixée a 30s dans le rc.local, et le script permet de relancer le wifi en cas de bug.
Les 30s permettent de s'assurer que le serveur est bien démarré et les services tels que network-manager, etc.. prêts.
Je redirige les sorties vers des fichiers pour pouvoir suivre l'évolution depuis une connexion ssh.
→ Ici je ne l'ai pas fait, mais j'ai certains de mes services qui se relancent automatiquement en cas de crash/kill

Le script coupe l'interface et le relance avec une configuration d'IP (celle du démarrage ne conviens pas)
La ligne "ifconfig mon.wlan0 down" est un interface qui apparaît et qui ne sert a rien donc je le coupe (bug?)

Les serveurs DHCP et DNS sont relancés pour être sur qu'ils écoutent sur tous les interfaces. J'avais remarqué que le DHCP par exemple n'écoutait pas sur wlan0 si je démarrais le wifi après.

Au niveau de la configuration DHCP, il faut que le subnet 192.168.1.0/24 aie pour route 192.168.1.11 (l'ip du serveur sur eth0) et que le subnet 192.168.2.0/24 aie pour route 192.168.2.11 (l'ip du serveur sur wlan0)
Comme le serveur a été précédemment configuré pour avoir 192.168.1.1 (freebox) en passerelle, toutes les connexions du réseau convergent vers le serveur (au lieu de la box) avant de repartir vers l'endroit voulu.
Le problème était que les clients de wlan0 cherchaient 192.168.1.1 en passerelle qui n'est pas sur leurs réseau.

On pourrait penser que eth0 doit utiliser 192.168.1.1 en passerelle, mais c'est faux. Si on fait ça, certes on a internet sur ces clients, mais il seront alors incapable d'engager une connexion TCP ou UDP vers un client de wlan0 (ils pourront par contre répondre a une connexion TCP ou ICMP venant de wlan0). C'est pour ça qu'il faut mettre comme route 192.168.1.11 sur les clients de eth0 pour que leurs demande du réseau de wlan0 arrive au serveur et soit bien redirigée.

Voilà, si vous avez bien configuré les forward d'IPV4 dans syscl.conf vous avez maintenant eth0 et wlan0 qui peuvent communiquer ensembles. (Note: Après modification du serveur DHCP: Relancer, et reconnecter tous les périphériques connectés au réseau pour appliquer les modifications)

Je termine juste en rajoutant que le rc.local est une très bonne invention de Linux, rien de tel pour gérer des tâches au démarrage d'un serveur smile
Et voici l'endroit dont je me suis inspiré: http://www.commentcamarche.net/forum/af … c-iptables (note: J'ai l'impression que leurs soluce ne fonctionne pas, certaines informations sont fausses d'où mes modifications)

PS: Il m'arrive dans certains cas que redémarrer le DHCP et la DNS empêche aux clients d'accéder au réseau. Commentez ces lignes au besoin, et laissez quelques secondes entre le démarrage du Wifi et le lancement du point d'accès.


