fix=0
scroll=999
fix_TITLE=3
headScroll=15
footScroll=30 # can be more than window's end line
scroll_TITLE=$((headScroll-1))
printf "\033[${fix_TITLE};1HFIX ROW:"
printf "\033[${headScroll};${footScroll}r" #scrollrool area
printf "\033[${scroll_TITLE};1HSCROLL AREA:"
printf "\033[s" #store
while(true); do
##########################
printf "\033[${fix_TITLE};1HFIX ROW:"
printf "\033[${headScroll};${footScroll}r" #scrollrool area
printf "\033[${scroll_TITLE};1HSCROLL AREA:"
#printf "\033[s" #store
##########################
	printf "\033[${fix_TITLE};1H\n$fix fix *********************"
	fix=$((fix+1));  printf "\n$fix fix *********************"
	printf "\033[u" #restore
	printf "\n$scroll scroll \033[6n--------------------------------------------"
	printf "\033[s" #store
	sleep 1
	fix=$((fix+1))
	scroll=$((scroll-1))
done;
read car
