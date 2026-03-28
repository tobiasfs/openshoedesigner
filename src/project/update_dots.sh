#!/bin/bash
inotifywait -m -e close_write --format %w%f /tmp/*.dot | while read dotfile;
do
	pngfile=$(echo "$dotfile" | sed 's/\.dot$/.png/')
	dot $dotfile -Tpng > $pngfile;  
done 
