#include <stdio.h>
#include <stdlib.h>
#include <string.h>



// function to validate month input. 
// Returns 0 if month is invalid. Otherwise returns num_of_month

int monthNumber (char * month) {
    if (strcmp(month, "Jan") == 0) return 1;
    if (strcmp(month, "Feb") == 0) return 2;
    if (strcmp(month, "Mar") == 0) return 3;
    if (strcmp(month, "Apr") == 0) return 4;
    if (strcmp(month, "May") == 0) return 5;
    if (strcmp(month, "Jun") == 0) return 6;
    if (strcmp(month, "Jul") == 0) return 7;
    if (strcmp(month, "Aug") == 0) return 8;
    if (strcmp(month, "Sep") == 0) return 9;
    if (strcmp(month, "Oct") == 0) return 10;
    if (strcmp(month, "Nov") == 0) return 11;
    if (strcmp(month, "Dec") == 0) return 12;

    else return 0;
}

// function to validate inputed date.
// Return number of month if input is correct. 0 otherwise.
int validateDate (char * month, int day) {
    // check for long monthes. If #days > 31 => incorrect input
    if ( (strcmp(month, "Jan") == 0 || strcmp(month, "Mar") == 0 || strcmp(month, "May") == 0 || strcmp(month, "Jul") == 0
          || strcmp(month, "Oct") == 0 || strcmp(month, "Dec") == 0) && day > 31 ) {
        return 0;
    }
    // same check for shorts monthes. If #days > 30 => incorrect input
    if ( (strcmp(month, "Apr") == 0 || strcmp(month, "Jun") == 0 || strcmp(month, "Sep") == 0 || strcmp(month, "Nov") == 0)
         && day > 30) {
        return 0;
    }
    // separate check for Feb.
    if ( (strcmp(month, "Feb") == 0) && day > 28 ) {
        return 0;
    }


    return monthNumber(month);



}

// structs to store data from cameras.
typedef struct T_CAMERAS {

    char m_auto_rz[1001]; // data in disc. order to safe some memory
    int m_camera_id;	  // char arr (1000B) first, ints (8B) then, small char arr (4B) last
    int m_month_num;		
    int m_day;
    int m_hour;
    int m_minute;
    char m_month[4];

} TCAMERAS;


// comparator for the qsort function to sort array of structs
// order of sorting: auto_rz, month_number, m_day, m_hour, m_minute, m_camera_id
int md_comparator(const void *v1, const void *v2)
{
    TCAMERAS * p1 = (TCAMERAS *)v1;
    TCAMERAS * p2 = (TCAMERAS *)v2;
    int rc;
    if ((rc = strcmp(p1->m_auto_rz, p2->m_auto_rz)) < 0)
        return -1;
    else if (rc > 0)
        return +1;
    else if (p1->m_month_num < p2->m_month_num)
        return -1;
    else if (p1->m_month_num > p2->m_month_num)
        return +1;
    else if (p1->m_day < p2->m_day)
        return -1;
    else if (p1->m_day > p2->m_day)
        return +1;
    else if (p1->m_hour < p2->m_hour)
        return -1;
    else if (p1->m_hour > p2->m_hour)
        return +1;
    else if (p1->m_minute < p2->m_minute)
        return -1;
    else if (p1->m_minute > p2->m_minute)
        return +1;
    else if (p1->m_camera_id < p2->m_camera_id)
        return +1;
    else if (p1->m_camera_id > p2->m_camera_id)
        return -1;
    else
        return 0;
}



// function to read data from user.
// Returns NULL if read failes. TCAMERAS * data otherwise.
TCAMERAS * readData( int * cntDt) {

    // variables to store 1 object.
    TCAMERAS * data = NULL;
    char month[10], auto_rz[1001];
    
    int camera_id, day, hour, minute, month_num;
    char oddel1;
    char oddel2;
    int max = 0;

    printf("Data z kamer:\n");

    if ( scanf (" %c", &oddel1)!= 1 || // check if data starts correctly (with opening '{')
         oddel1 != '{') {
        return NULL;
    }
    // read the data while it's possible
    while ( scanf (" %d : %1000s %3s %d %d : %d %c", &camera_id, auto_rz, month, &day, &hour, &minute, &oddel2) == 7)  {
        if ( day <= 0 || hour > 23 || hour < 0
             || minute > 59 || minute < 0 || (oddel2 != '}' && oddel2 != ',')
             || (!(month_num = validateDate(month, day)))){
            free(data); // if object is invalid - free memory + input error;
            return NULL;
        }
        if ( *cntDt >= max ) { // give some memory to array of structs
            if ( max < 1000 ) { max += 100; }
            else { max += max / 2; }
            data = (TCAMERAS*) realloc ( data, max * sizeof ( *data ) );
        }

        // if all ok - write data into array of structs, counter++
        data[*cntDt].m_camera_id = camera_id;
        data[*cntDt].m_month_num = month_num;
        strcpy(data[*cntDt].m_auto_rz, auto_rz);
        strcpy(data[*cntDt].m_month, month);
        data[*cntDt].m_day = day;
        data[*cntDt].m_hour = hour;
        data[*cntDt].m_minute = minute;
        (*cntDt)++;

        // read data until closing braket
        if (oddel2 == '}') {
            break;
        }
    }
   	// if data was ended incorrectly - free memory + input error 
    if (oddel2 != '}') {
    	free(data);
    	return NULL;
    }
    return data;
}
// final function in branch that searches duplicates of the elements in DB
// returns array of indexes with duplicates (or only 1 matched element if weren't found)
int * searchDups(TCAMERAS * data, int * counter, int lo, int hi, char order) {
    // printf("Searching dups in: lo %d hi %d\n", lo, hi);
    int max = 100;
    
    if (order == 1) { // ascending order. Starting from higher border to bottom. Searching for 1 element that doesn't match
    	int * matches_asc = NULL;
   		matches_asc = (int*)malloc(max * sizeof(int));
    	matches_asc[0] = hi+1; // write first element that was found in previous function
    	(*counter)++; 
        while(hi - lo >= 0) {
            if (data[hi+1].m_month_num == data[hi].m_month_num && data[hi+1].m_day == data[hi].m_day
                && data[hi+1].m_hour == data[hi].m_hour && data[hi+1].m_minute == data[hi].m_minute) {
                if (*counter >= max) { // if duplicate was found - give memory to the array if needed
                	if ( max < 1000 ) { max += 100; }
           			else { max += max / 2; }
                    matches_asc = (int*)realloc(matches_asc, max * sizeof(int));
                }

                matches_asc[*counter] = hi;
                (*counter)++;
                hi--;
             }
            
            else
            	break;
            	
         }

     	return matches_asc;
            
    }
    // absolutly same but starting from lower border to top
    if (order == -1) {
    	int * matches_disc = NULL;
   		matches_disc = (int*)malloc(max * sizeof(int));
    	matches_disc[0] = lo - 1; 
    	(*counter)++;
    	while (hi - lo >= 0) {
    		if(data[lo-1].m_month_num == data[lo].m_month_num && data[lo-1].m_day == data[lo].m_day
                && data[lo-1].m_hour == data[lo].m_hour && data[lo-1].m_minute == data[lo].m_minute) {
    			if (*counter >= max) {
    				if ( max < 1000 ) { max += 100; }
		            else { max += max / 2; }
                    matches_disc = (int*)realloc(matches_disc, max * sizeof(int));
                }

                matches_disc[*counter] = lo;
                (*counter)++;
                lo++;
    		}
    		else
    			break;
    			

    	}

    	return matches_disc; 
    }

    return NULL; 
}
// TODO: convert into minutes on reading data stage. Instead of long ifs shorter and better
// Function to locate matching element by time
// returns NULL if right time wasn't found
int * compareTime(TCAMERAS * data, TCAMERAS search, int lo, int hi, char order, int * counter, char * flag) {
    // if order is ascending - looking for first record that was shooted earlier then higher border
    if (order == 1) {
    	//printf("Searching in: lo %d hi %d\n", lo, hi);
        while ( hi - lo >= 0 ) {
            if(data[hi].m_month_num < search.m_month_num 
            	|| (data[hi].m_month_num == search.m_month_num && data[hi].m_day < search.m_day)
            	|| (data[hi].m_month_num == search.m_month_num && data[hi].m_day == search.m_day && data[hi].m_hour < search.m_hour)
            	|| (data[hi].m_month_num == search.m_month_num && data[hi].m_day == search.m_day && data[hi].m_hour == search.m_hour && data[hi].m_minute < search.m_minute)) {;
                //printf("found in lo %d hi %d\n", lo, hi);
                return searchDups(data, counter, lo, hi-1, order);
            }
            else if (data[hi].m_month_num == search.m_month_num && data[hi].m_day == search.m_day && data[hi].m_hour == search.m_hour && data[hi].m_minute == search.m_minute) {
            	*flag = 1; 											// 'exact' search triggersflag
                return searchDups(data, counter, lo, hi-1, order);
            }
            else
       			hi--;
        }        
    }    

    

    if ( order == -1 ) {
    	// same as ascending, but starting from buttom index and searching for 1 record that was shooted sooner
    	//printf("Searching in: lo %d hi %d\n", lo, hi);
    	while ( hi - lo >= 0) {
    		if (data[lo].m_month_num > search.m_month_num
    			|| (data[lo].m_month_num == search.m_month_num && data[lo].m_day > search.m_day)
    			|| (data[lo].m_month_num == search.m_month_num && data[lo].m_day == search.m_day && data[lo].m_hour > search.m_hour)
    			|| (data[lo].m_month_num == search.m_month_num && data[lo].m_day == search.m_day && data[lo].m_hour == search.m_hour && data[lo].m_minute > search.m_minute)) {
    			return searchDups(data, counter, lo+1, hi, order);
    		}
    		else if (data[lo].m_month_num == search.m_month_num && data[lo].m_day == search.m_day && data[lo].m_hour == search.m_hour && data[lo].m_minute == search.m_minute) {
    			*flag = 1;
    			return searchDups(data, counter, lo+1, hi, order);
    		}
    		else
    			lo++;

    	}
    }





    return NULL;

}
// function to find indexes that matches auto_rz. Finding i as lower element in DB with same name. j as highest
// returns filled array with indexes of matched elements if found. NULL otherwise 

int * searchMatching(TCAMERAS * data, TCAMERAS search, int cntDt, char order, int * counter, char * flag) {
        
        for (int i = 0; i < cntDt; i++) {							// DB is sorted by auto_rz. iterating from 0 until found 1 match.
            if (strcmp(data[i].m_auto_rz, search.m_auto_rz) == 0) {
            	if ( i == cntDt - 1 )
            		return compareTime(data, search, i, i, order, counter, flag); // if it's only last one - sending lower and higher border as that element
                for (int j = i; j < cntDt - 1; j++) { // starting count matched elements
                    if (strcmp(data[j+1].m_auto_rz, search.m_auto_rz) != 0 || j + 1 == cntDt - 1) { // if we met mismatch or penultimate element - go inside
                    	if (strcmp(data[j+1].m_auto_rz, search.m_auto_rz) == 0) // if last element is matching - then we should add it to calculations too
                    		return compareTime(data,search, i, j+1, order, counter, flag); // same as 1 line down + include last element
                        return compareTime(data, search, i, j, order, counter, flag); // i - lower index of matched element
                        															  // j - is higher
                    }
                }
            }
        }

    return NULL;
}


// function for all main calculations and reading search requests 
// returns 0 if all gone ok, 1 otherwise 
char readMatching(TCAMERAS * data, int cntDt) {
    TCAMERAS search;
    int counter_as = 0, counter_dis = 0; /* counters and arrays of inndexes of matched elements. Acs - ascending order calculation for 'previous' and 'exact' value
    										disc - discending order for 'next' value */
    int * acs = NULL, *disc = NULL;     
    char flag = 0; 						// indicator for right printing of "exact" value
    int readed = 0;						// counter of readed search requests 
    printf("Hledani:\n");

    while (scanf (" %1000s %3s %d %d : %d ", search.m_auto_rz, // read until it's possible. Return 1 if something incorrect
                  search.m_month, &search.m_day, &search.m_hour, &search.m_minute) == 5) {
        if (search.m_day <= 0 || search.m_hour > 23 || search.m_hour < 0
             || search.m_minute > 59 || search.m_minute < 0
             || (!(search.m_month_num = validateDate(search.m_month, search.m_day)))){
            return 1; 							
        }
        acs = NULL; // pointers to NULL to allocate memory again
        disc = NULL;
        readed++;

        acs = searchMatching(data, search, cntDt, 1, &counter_as, &flag);
        disc = searchMatching(data, search, cntDt, -1, &counter_dis, &flag);

        
        // ??? TODO: rewrite to normal print function. Strange cascade of prints with hard code
        if (!counter_as && !counter_dis) { // if no elements were added - auto wasn't found
        	printf("> Automobil nenalezen.\n");
        	flag = -1; 
        }
        else if (flag == 1) { // if search is 'exact'

        	if (counter_as == 1) {
	        	printf("> Presne: %s %d %02d:%02d, 1x [%d]\n", data[acs[0]].m_month, data[acs[0]].m_day, data[acs[0]].m_hour,
	        			data[acs[0]].m_minute, data[acs[0]].m_camera_id);
        	}
        	if (counter_as > 1) {
	        	printf("> Presne: %s %d %02d:%02d, %dx [", data[acs[0]].m_month, data[acs[0]].m_day, data[acs[0]].m_hour,
	        			data[acs[0]].m_minute, counter_as);
	        	for (int i = 0; i < counter_as; i++) {
	        		if (i == counter_as - 1) {
	        			printf("%d]\n", data[acs[i]].m_camera_id);
	        			break;
	        		}
	        	printf("%d, ", data[acs[i]].m_camera_id);	
	        	}
	        }

	        flag = -1; // to 'get in' into right if branch

        }

        else if (counter_as == 1) { // if 1 element wes found in 'previous'
        	printf("> Predchazejici: %s %d %02d:%02d, 1x [%d]\n", data[acs[0]].m_month, data[acs[0]].m_day, data[acs[0]].m_hour,
        			data[acs[0]].m_minute, data[acs[0]].m_camera_id);
        }

        else if (counter_as > 1) { // if previous elemennts contains duplicates 
        	printf("> Predchazejici: %s %d %02d:%02d, %dx [", data[acs[0]].m_month, data[acs[0]].m_day, data[acs[0]].m_hour,
        			data[acs[0]].m_minute, counter_as);
        	for (int i = 0; i < counter_as; i++) {
        		if (i == counter_as - 1) {
        			printf("%d]\n", data[acs[i]].m_camera_id);
        			break;
        		}
        	printf("%d, ", data[acs[i]].m_camera_id);	
        	}
        }


        else if (counter_as == 0) { // if no elements in 1 array - no previous record
        	printf("> Predchazejici: N/A\n");
        }
        // ??? 
        if (flag == -1) {
        	flag = 0; 
        }

        else if (counter_dis == 1) { // same as previous
        	printf("> Pozdejsi: %s %d %02d:%02d, 1x [%d]\n", data[disc[0]].m_month, data[disc[0]].m_day, data[disc[0]].m_hour,
        			data[disc[0]].m_minute, data[disc[0]].m_camera_id);
        }

        else if (counter_dis > 1) { // same, but print camera_id in ascending order ( were written in discending )
        	printf("> Pozdejsi: %s %d %02d:%02d, %dx [", data[disc[0]].m_month, data[disc[0]].m_day, data[disc[0]].m_hour,
        			data[disc[0]].m_minute, counter_dis);
        	for (int i = counter_dis-1; i >= 0; i--) {
        		if (i == 0) {
        			printf("%d]\n", data[disc[i]].m_camera_id);
        			break;
        		}
        	printf("%d, ", data[disc[i]].m_camera_id);	
        	}
        }
        // same
        else if (counter_dis == 0) {
        	printf("> Pozdejsi: N/A\n");
        }

        counter_as = 0;		// free memory, zero out counters;
        counter_dis = 0;
        free(acs);
    	free(disc);





        // printf("%s %s m_num %d %d %d %d \n", search.m_auto_rz, search.m_month, search.m_month_num,
        //        search.m_day, search.m_hour, search.m_minute);



    }

    
    if (!feof(stdin) || readed == 0)
        return 1;
    return 0;
}

int main() {

    TCAMERAS * data; // array of structs to store cameras records database
    int cntDt = 0; // counter of records in db 

    data = readData ( &cntDt ); 
    if ( !data ){ 						// if NULL was retorned - data read error. Incorrect input
        printf("Nespravny vstup.\n");
        return 1;
    }
    qsort(data, cntDt, sizeof(TCAMERAS), md_comparator); // sorting database
    if (readMatching(data, cntDt)) { 
    	free(data);
        printf("Nespravny vstup.\n");
        return 1;
    }
   // printf("Number of elemnts: %d\n", cntDt);
   // for (int i = 0; i < cntDt; i++) {
   // 		 printf("[%d] %s %s m_num %d %d %d %d \n", data[i].m_camera_id, data[i].m_auto_rz, data[i].m_month, data[i].m_month_num,
   //              data[i].m_day, data[i].m_hour, data[i].m_minute);
   // }

    free(data);








    return 0;
}
