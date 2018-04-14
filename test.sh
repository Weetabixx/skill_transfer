	#!/bin/bash
	
	
	#worlds=(grabbing_book2 grabbing_book2 grabbing_book2 grabbing_book2 grabbing_book2 grabbing_book2 grabbing_book2 grabbing_book2 grabbing_book2 grabbing_book2
		#freezer_box freezer_box freezer_box freezer_box freezer_box freezer_box freezer_box freezer_box freezer_box freezer_box freezer_box 
		#freezer_box2 freezer_box2 freezer_box2 freezer_box2 freezer_box2 freezer_box2 freezer_box2 freezer_box2 freezer_box2 freezer_box2 freezer_box2 
		#freezer_box3 freezer_box3 freezer_box3 freezer_box3 freezer_box3 freezer_box3 freezer_box3 freezer_box3 freezer_box3 freezer_box3 freezer_box3 
		#freezer_box4 freezer_box4 freezer_box4 freezer_box4 freezer_box4 freezer_box4 freezer_box4 freezer_box4 freezer_box4 freezer_box4 freezer_box4 
		#freezer_box5 freezer_box5 freezer_box5 freezer_box5 freezer_box5 freezer_box5 freezer_box5 freezer_box5 freezer_box5 freezer_box5 freezer_box5 
		#freezer_box6 freezer_box6 freezer_box6 freezer_box6 freezer_box6 freezer_box6 freezer_box6 freezer_box6 freezer_box6 freezer_box6 freezer_box6 
		#freezer_box7 freezer_box7 freezer_box7 freezer_box7 freezer_box7 freezer_box7 freezer_box7 freezer_box7 freezer_box7 freezer_box7 freezer_box7 ) 
	worlds=(grabbing_book grabbing_book grabbing_book grabbing_book grabbing_book grabbing_book grabbing_book grabbing_book grabbing_book grabbing_book grabbing_book 
		grabbing_book2 grabbing_book2 grabbing_book2 grabbing_book2 grabbing_book2 grabbing_book2 grabbing_book2 grabbing_book2 grabbing_book2 grabbing_book2 grabbing_book2 
		grabbing_book3 grabbing_book3 grabbing_book3 grabbing_book3 grabbing_book3 grabbing_book3 grabbing_book3 grabbing_book3 grabbing_book3 grabbing_book3 grabbing_book3 
		grabbing_book4 grabbing_book4 grabbing_book4 grabbing_book4 grabbing_book4 grabbing_book4 grabbing_book4 grabbing_book4 grabbing_book4 grabbing_book4 grabbing_book4 
		grabbing_book5 grabbing_book5 grabbing_book5 grabbing_book5 grabbing_book5 grabbing_book5 grabbing_book5 grabbing_book5 grabbing_book5 grabbing_book5 grabbing_book5 
		grabbing_book6 grabbing_book6 grabbing_book6 grabbing_book6 grabbing_book6 grabbing_book6 grabbing_book6 grabbing_book6 grabbing_book6 grabbing_book6 grabbing_book6 
		grabbing_book7 grabbing_book7 grabbing_book7 grabbing_book7 grabbing_book7 grabbing_book7 grabbing_book7 grabbing_book7 grabbing_book7 grabbing_book7 grabbing_book7 
		grabbing_book8 grabbing_book8 grabbing_book8 grabbing_book8 grabbing_book8 grabbing_book8 grabbing_book8 grabbing_book8 grabbing_book8 grabbing_book8 grabbing_book8 ) 

	
	#experiments=(book_on_shelf book_on_shelf book_on_shelf book_on_shelf book_on_shelf book_on_shelf book_on_shelf book_on_shelf book_on_shelf book_on_shelf book_on_shelf freezer_box freezer_box freezer_box freezer_box freezer_box freezer_box freezer_box freezer_box freezer_box freezer_box freezer_box 
		#freezer_box2 freezer_box2 freezer_box2 freezer_box2 freezer_box2 freezer_box2 freezer_box2 freezer_box2 freezer_box2 freezer_box2 freezer_box2 
		#freezer_box3 freezer_box3 freezer_box3 freezer_box3 freezer_box3 freezer_box3 freezer_box3 freezer_box3 freezer_box3 freezer_box3 freezer_box3 
		#freezer_box4 freezer_box4 freezer_box4 freezer_box4 freezer_box4 freezer_box4 freezer_box4 freezer_box4 freezer_box4 freezer_box4 freezer_box4 
		#freezer_box5 freezer_box5 freezer_box5 freezer_box5 freezer_box5 freezer_box5 freezer_box5 freezer_box5 freezer_box5 freezer_box5 freezer_box5 
		#freezer_box6 freezer_box6 freezer_box6 freezer_box6 freezer_box6 freezer_box6 freezer_box6 freezer_box6 freezer_box6 freezer_box6 freezer_box6 
		#freezer_box7 freezer_box7 freezer_box7 freezer_box7 freezer_box7 freezer_box7 freezer_box7 freezer_box7 freezer_box7 freezer_box7 freezer_box7 )
	experiments=(book_on_shelf book_on_shelf book_on_shelf book_on_shelf book_on_shelf book_on_shelf book_on_shelf book_on_shelf book_on_shelf book_on_shelf book_on_shelf 
		book_on_shelf2 book_on_shelf2 book_on_shelf2 book_on_shelf2 book_on_shelf2 book_on_shelf2 book_on_shelf2 book_on_shelf2 book_on_shelf2 book_on_shelf2 book_on_shelf2 
		book_on_shelf3 book_on_shelf3 book_on_shelf3 book_on_shelf3 book_on_shelf3 book_on_shelf3 book_on_shelf3 book_on_shelf3 book_on_shelf3 book_on_shelf3 book_on_shelf3 
		book_on_shelf4 book_on_shelf4 book_on_shelf4 book_on_shelf4 book_on_shelf4 book_on_shelf4 book_on_shelf4 book_on_shelf4 book_on_shelf4 book_on_shelf4 book_on_shelf4 
		book_on_shelf5 book_on_shelf5 book_on_shelf5 book_on_shelf5 book_on_shelf5 book_on_shelf5 book_on_shelf5 book_on_shelf5 book_on_shelf5 book_on_shelf5 book_on_shelf5 
		book_on_shelf6 book_on_shelf6 book_on_shelf6 book_on_shelf6 book_on_shelf6 book_on_shelf6 book_on_shelf6 book_on_shelf6 book_on_shelf6 book_on_shelf6 book_on_shelf6 
		book_on_shelf7 book_on_shelf7 book_on_shelf7 book_on_shelf7 book_on_shelf7 book_on_shelf7 book_on_shelf7 book_on_shelf7 book_on_shelf7 book_on_shelf7 book_on_shelf7 
		book_on_shelf8 book_on_shelf8 book_on_shelf8 book_on_shelf8 book_on_shelf8 book_on_shelf8 book_on_shelf8 book_on_shelf8 book_on_shelf8 book_on_shelf8 book_on_shelf8 )
	index=0


	while [ $index -lt 88 ]; do

	gnome-terminal -e "timeout 180s roslaunch skill_transfer simulation.launch world:=${worlds[$index]}" #kill node?
	sleep 10s;
	gnome-terminal -e "timeout 100s roslaunch skill_transfer experiment.launch task:=tiltgrabbing robot:=free_ees setup:=${experiments[$index]}"
	sleep 190;
	
	echo -e " $index : \n" >>output.txt
	echo -e " ${worlds[$index]} : \n" >>bookoutput.txt	
	echo -e " ${experiments[$index]} : \n" >>bookoutput.txt
	while read -r row; do
	echo -e "$row \n" >>output.txt #depends on the format
	done < ~/.gazebo/server-11345/default.log
	echo -e "**************************************************************************************\n " >>output.txt
	echo -e "\n\n\n " >>output.txt
	index=$((index+1))
	
#	echo "Trial NO.$index accomplished"
#	sleep 5s;
	done
