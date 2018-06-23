#!/bin/bash

# Types your chess.com username
username='instudy'

# Set prefix icon

icon=''

# Set prefix icon color with and without unread messages
gamesColor='#e06c75'
noGamesColor='#a1abaf'

games=$(curl -s "https://api.chess.com/pub/player/$username/games/to-move" | jq ".[] | length")
(( "$games" > "0" )) && echo "%{F$gamesColor}$icon $games" || echo "%{F$noGamesColor}$icon $games"
