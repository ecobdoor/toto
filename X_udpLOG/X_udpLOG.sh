#export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/hgf/KAM_000.DEBUG/LIB/opencv40/lib/
export DISPLAY=:0.0
TITLE="UDP telemetry"
echo -ne "\033]0;$TITLE\007"
resize -s 46 150 > /dev/null  # resize gnome
#sleep 1
#wmctrl -l  # list all windows
# Cf. http://manpages.ubuntu.com/manpages/trusty/man1/xdotool.1.html
X=`xdotool getactivewindow` #;echo $X
#xdotool getwindowname $X # display the name
#xdotool getwindowgeometry $X # display the geometry
#--usehints char columns & rows dimensions
#--sync wait for executed command
#xdotool windowsize --usehints --sync $X 148 46 # resize xdotool (unused here)
xdotool windowmove --sync $X 2000 30

#cd ../KAM_000.DEBUG
cd "/media/DEV/ROB_000/GIT_EclipseWS/X_udpLOG/Debug 2004"
./X_udpLOG
echo "out of africa";read car
cd ../KAM_000.SH
