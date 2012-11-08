#!/bin/sh
cd fs
gcc genera_pixmap.c -o genera_pixmap
./genera_pixmap
./genera_initrd hello pixmap.sk
cd ..
make 
