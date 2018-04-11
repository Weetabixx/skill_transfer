	#!/bin/bash
	
	
	worlds=(scraping_a_bowlchild_a_chineseknife_v scraping_b_big_bowl_a_chineseknife_v scraping_b_bucket_a_chineseknife_v scraping_b_pot_a_chineseknife_v scraping_b_wildo_bowl_a_chineseknife_v 
scraping_a_bowlchild_a_forkbig_v scraping_b_big_bowl_a_forkbig_v scraping_b_bucket_a_forkbig_v scraping_b_pot_a_forkbig_v scraping_b_wildo_bowl_a_forkbig_v 
scraping_a_bowlchild_a_knifekitchen2_v scraping_b_big_bowl_a_knifekitchen2_v scraping_b_bucket_a_knifekitchen2_v scraping_b_pot_a_knifekitchen2_v scraping_b_wildo_bowl_a_knifekitchen2_v 
scraping_a_bowlchild_a_knifekitchen3_v scraping_b_big_bowl_a_knifekitchen3_v scraping_b_bucket_a_knifekitchen3_v scraping_b_pot_a_knifekitchen3_v scraping_b_wildo_bowl_a_knifekitchen3_v 
scraping_a_bowlchild_a_scraper_v scraping_b_big_bowl_a_scraper_v scraping_b_bucket_a_scraper_v scraping_b_pot_a_scraper_v scraping_b_wildo_bowl_a_scraper_v 
scraping_a_bowlchild_a_siliconespatula_v scraping_b_big_bowl_a_siliconespatula_v scraping_b_bucket_a_siliconespatula_v scraping_b_pot_a_siliconespatula_v scraping_b_wildo_bowl_a_siliconespatula_v 
scraping_a_bowlchild_a_spatulawoodgap2_v scraping_b_big_bowl_a_spatulawoodgap2_v scraping_b_bucket_a_spatulawoodgap2_v scraping_b_pot_a_spatulawoodgap2_v scraping_b_wildo_bowl_a_spatulawoodgap2_v 
scraping_a_bowlchild_a_woodenspoon1_v scraping_b_big_bowl_a_woodenspoon1_v scraping_b_bucket_a_woodenspoon1_v scraping_b_pot_a_woodenspoon1_v scraping_b_wildo_bowl_a_woodenspoon1_v) 

	
	experiments=(a_bowlchild_a_chineseknife b_big_bowl_a_chineseknife b_bucket_a_chineseknife b_pot_a_chineseknife b_wildo_bowl_a_chineseknife
a_bowlchild_a_forkbig b_big_bowl_a_forkbig b_bucket_a_forkbig b_pot_a_forkbig b_wildo_bowl_a_forkbig
a_bowlchild_a_knifekitchen2 b_big_bowl_a_knifekitchen2 b_bucket_a_knifekitchen2 b_pot_a_knifekitchen2 b_wildo_bowl_a_knifekitchen2
a_bowlchild_a_knifekitchen3 b_big_bowl_a_knifekitchen3 b_bucket_a_knifekitchen3 b_pot_a_knifekitchen3 b_wildo_bowl_a_knifekitchen3
a_bowlchild_a_scraper b_big_bowl_a_scraper b_bucket_a_scraper b_pot_a_scraper b_wildo_bowl_a_scraper
a_bowlchild_a_siliconespatula b_big_bowl_a_siliconespatula b_bucket_a_siliconespatula b_pot_a_siliconespatula b_wildo_bowl_a_siliconespatula
a_bowlchild_a_spatulawoodgap2 b_big_bowl_a_spatulawoodgap2 b_bucket_a_spatulawoodgap2 b_pot_a_spatulawoodgap2 b_wildo_bowl_a_spatulawoodgap2
a_bowlchild_a_woodenspoon1 b_big_bowl_a_woodenspoon1 b_bucket_a_woodenspoon1 b_pot_a_woodenspoon1 b_wildo_bowl_a_woodenspoon1)
	index=21


	while [ $index -lt 40 ]; do

	gnome-terminal -e "timeout 180s roslaunch skill_transfer simulation.launch world:=${worlds[$index]}" #kill node?
	sleep 10s;
	gnome-terminal -e "timeout 100s ./test1.sh ${experiments[$index]}"
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
