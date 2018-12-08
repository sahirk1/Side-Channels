/*******************************************************
 CSE127 Project
 User routines file

  You can change anything in this file, just make sure 
  that when you have found the password, you call 
  hack_system() function on it.

 CSE127 Password guesser using timing

 *******************************************************/

#include <stdio.h>
#include <string.h>
#include <unistd.h>

// include our "system" header
#include "sysapp.h"

// Read cycle counter
#define rdtsc() ({ unsigned long a, d; asm volatile("rdtsc":"=a" (a), "=d" (d)) ; a; })

int main(int argc, char **argv) {
    char guess[33];

    // set guess to zeros
    bzero(guess, sizeof(guess));
 
    //
    // do the guessing (this is where your code goes)
    //   we suggest a loop over the size of the possible
    //   password, each time trying all possible characters
    //   and looking for time deviations
    //
    int i;
    int j;
    int flag = 0;
    int count = 0;

    for (i = 0; i < 32; i++)
    {
       long longArr[400];
       char charArr[400];

       int j;
       
 	for (j = 0; j < 400; j++)
	{
	  int q;
          long max = 0;
	  char maxChar = 'a';

          for (q = 33; q <= 126; q++)
          {
             char ch = (char) q;
	     guess[i] = ch;

             long t1 = rdtsc();
	     int val = check_pass(guess);
	     long t2 = rdtsc();

	     if (val == 1)
	     {
		flag = 1;
		break;
	     }
		
	     long diff = t2 - t1;

	     if (diff > max)
	     {
		max = diff;
		maxChar = ch;
	     }	     	      	   
	  }
	
	  if (flag == 1)
	  {
	     break;
	  }

          longArr[j] = max;
	  charArr[j] = maxChar;
         }
	
	if (flag == 1)
	{
	     break;
	}

	int a;
	int b;

	for (a = 0; a < 400; a++)
	{
	   for (b = 0; b < 399; b++)
	   {
		if (longArr[b] > longArr[b+1])
		{
		   long temp = longArr[b];
		   longArr[b] = longArr[b+1];
		   longArr[b+1] = temp;

		   char tempCh = charArr[b];
		   charArr[b] = charArr[b+1];
		   charArr[b+1] = tempCh;
		}
	   }
	}

        guess[i] = charArr[200];

	if (check_pass(guess) != 1 && count < 5)
	{
	   i = 0;
	   count++;
	}
    }
	
    printf ("Guess = %s\n", guess);
	 
    if (check_pass(guess)) {
        printf("Password Found!\n");
        hack_system(guess);
    };

    printf("Could not get the password!  Last guess was %s\n", guess);
    return 1;
};
