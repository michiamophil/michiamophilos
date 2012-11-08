#!/bin/bash
# Script che prende in imput un immagine (*.C) e restituisce il formato raw nella cartella tmpfs
if [ ! -a "$1" -o -z "$2" -o "$1" = "-h" -o "$1" = "--help" ]; then
	echo "usage: ./c2raw.sh path/file.c file.raw"
	exit 1
fi

echo "Creazione file temporaneo..."
cat ./header > ./tmp.c
cat $1 >> ./tmp.c
cat ./footer >> ./tmp.c

echo "Compilazione..."
gcc ./tmp.c -o ./image

DESTPATH=../../tmpfs/$2
echo "Esecuzione..."
./image $DESTPATH


rm ./tmp.c ./image
echo "Done."
exit 0
