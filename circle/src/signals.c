/* ************************************************************************
*  file: signals.c , trapping of signals from Unix.       Part of DIKUMUD *
*  Usage : Signal Trapping.                                               *
*  Copyright (C) 1990, 1991 - see 'license.doc' for complete information. *
************************************************************************* */

#include <signal.h>
#include <stdio.h>
#include <sys/time.h>

#include "structs.h"
#include "utils.h"

extern struct descriptor_data *descriptor_list;
extern int	mother_desc;

void	checkpointing();
void	logsig();
void	hupsig();
/* void	badcrash(); */
void	unrestricted_game();
void	reread_wizlists();

int	graceful_tried = 0;

void	signal_setup(void)
{
   struct itimerval itime;
   struct timeval interval;

   signal(SIGUSR1, reread_wizlists);
   signal(SIGUSR2, unrestricted_game);

   /* just to be on the safe side: */

   signal(SIGHUP, hupsig);
   signal(SIGPIPE, SIG_IGN);
   signal(SIGINT, hupsig);
   signal(SIGALRM, logsig);
   signal(SIGTERM, hupsig);
   /*   signal(SIGSEGV, badcrash);
   signal(SIGBUS, badcrash);
*/
   /* set up the deadlock-protection */

   interval.tv_sec = 900;    /* 15 minutes */
   interval.tv_usec = 0;
   itime.it_interval = interval;
   itime.it_value = interval;
   setitimer(ITIMER_VIRTUAL, &itime, 0);
   signal(SIGVTALRM, checkpointing);
}



void	checkpointing(void)
{
   extern int	tics;

   if (!tics) {
      log("CHECKPOINT shutdown: tics not updated");
      abort();
   } else
      tics = 0;
}


void	reread_wizlists()
{
   void	reboot_wizlists(void);

   signal(SIGUSR1, reread_wizlists);
   syslog("Rereading wizlists.", CMP, LEVEL_IMMORT, FALSE);
   reboot_wizlists();
}


void	unrestricted_game()
{
   extern int	restricted;
   extern struct ban_list_element *ban_list;
   extern int	num_invalid;

   signal(SIGUSR2, unrestricted_game);
   syslog("Received SIGUSR2 - unrestricteding game (emergent)",
	  BRF, LEVEL_IMMORT, TRUE);
   ban_list = 0;
   restricted = 0;
   num_invalid = 0;
}





/* kick out players etc */
void	hupsig(void)
{
   log("Received SIGHUP, SIGINT, or SIGTERM.  Shutting down...");
   exit(0);   /* something more elegant should perhaps be substituted */
}


void	badcrash(void)
{
   void	close_socket(struct descriptor_data *d);
   struct descriptor_data *desc;

   log("SIGSEGV or SIGBUS received.  Trying to shut down gracefully.");
   if (!graceful_tried) {
      close(mother_desc);
      log("Trying to close all sockets.");
      graceful_tried = 1;
      for (desc = descriptor_list; desc; desc = desc->next)
	 close(desc->descriptor);
   }
   abort();
}


void	logsig(void)
{
   log("Signal received.  Ignoring.");
}


