#!/bin/sh

#The icon that would change color
icon=""

if pgrep -x "compton" > /dev/null
then
	echo "%{F#000000}$icon" #Green
else
	echo "%{F#a83739}$icon" #Gray
fi
