	#!/bin/bash
	
	
	#worlds=(grabbing_book grabbing_book grabbing_book grabbing_book grabbing_book grabbing_book grabbing_book grabbing_book grabbing_book grabbing_book) 
	worlds=(freezer_box2 freezer_box2 freezer_box2 freezer_box2 freezer_box2 freezer_box2 freezer_box2 freezer_box2 freezer_box2 freezer_box2 freezer_box2) 

	
	#experiments=(book_on_shelf book_on_shelf book_on_shelf book_on_shelf book_on_shelf book_on_shelf book_on_shelf book_on_shelf book_on_shelf book_on_shelf book_on_shelf )
	experiments=(freezer_box2 freezer_box2 freezer_box2 freezer_box2 freezer_box2 freezer_box2 freezer_box2 freezer_box2 freezer_box2 freezer_box2 freezer_box2)
	index=0


	while [ $index -lt 10 ]; do

	gnome-terminal -e "timeout 180s roslaunch skill_transfer simulation.launch world:=${worlds[$index]}" #kill node?
	sleep 10s;
	gnome-terminal -e "timeout 100s roslaunch skill_transfer experiment.launch task:=tiltgrabbing robot:=free_ees setup:=${experiments[$index]}"
	sleep 190;
	
	echo -e " $index : \n" >>output.txt
	echo -e " ${worlds[$index]} : \n" >>output.txt	
	echo -e " ${experiments[$index]} : \n" >>output.txt
	while read -r row; do
	echo -e "$row \n" >>output.txt #depends on the format
	done < ~/.gazebo/server-11345/default.log
	echo -e "**************************************************************************************\n " >>output.txt
	echo -e "\n\n\n " >>output.txt
	index=$((index+1))
	
#	echo "Trial NO.$index accomplished"
#	sleep 5s;
	done
