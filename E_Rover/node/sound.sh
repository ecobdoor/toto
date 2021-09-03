echo "USAGE:"
#https://gtts.readthedocs.io/en/latest/module.html#languages-gtts-lang
echo "    USAGE: ./sound.sh lang file text"
python sound.py "$1" "$2" "$3"
ffplay $2