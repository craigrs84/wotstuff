/* ************************************************************************
*  file: config.c							  *
*  Usage: configuration of various aspects of the game			  *
*  Copyright (C) 1990, 1991 - see 'license.doc' for complete information. *
*                                                                         *
*  This code is part of CircleMUD.  Copyright (C) 1992, 1993 The Trustees *
*  of The Johns Hopkins University.                                       *
************************************************************************* */

#include "structs.h"

#define TRUE	1
#define YES	1
#define FALSE	0
#define NO	0

/*
Below are several constants which you can change to alter certain aspects
of the way CircleMUD acts.  Since this is a .c file, all you have to do
to change one of the constants (assuming you keep your object files around)
is change the constant in this file and type 'make'.  Make will recompile
this file and relink; you don't have to wait for the whole thing to recompile
as you do if you change a header file.

I realize that it would be slightly more efficient to have lots of
#defines strewn about, so that, for example, the autowiz code isn't
compiled at all if you don't want to use autowiz.  However, the actual
code for the various options is quite small, as is the computational time
in checking the option you've selected at run-time, so I've decided the
convenience of having all your options in this one file outweighs the
efficency of doing it the other way.
*/

/****************************************************************************/
/****************************************************************************/

/* GAME PLAY OPTIONS */

/* maximum number of items players are allowed to rent */
int max_obj_save = 30;

/* is playerkilling allowed? */
byte pk_allowed = NO;

/* is playerthieving allowed? */
byte pt_allowed = NO;

/* virtual number of room that immorts should enter at by default */
sh_int immort_start_room = 1204;

/* virtual number of room that mortals should enter at */
sh_int mortal_start_room = 3001;

/* virtual number of room that frozen players should enter at */
sh_int frozen_start_room = 1202;

/* minimum level a player must be to shout/holler/gossip/auction */
int level_can_shout = 2;

/* number of movement points it costs to holler */
int holler_move_cost = 20;

/* constants for the mail system are in mail.h. */

/****************************************************************************/
/****************************************************************************/


/* AUTOWIZ OPTIONS */

/* Should the game automatically create a new wizlist/immlist every time
   someone immorts, or is promoted to a higher (or lower) god level? */
byte use_autowiz = YES;

/* If yes, what is the lowest level which should be on the wizlist?  (All
   immort levels below the level you specify will go on the immlist instead. */
int min_wizlist_lev = LEVEL_GOD;


/****************************************************************************/
/****************************************************************************/


/* GAME OPERATION OPTIONS */

/* default port the game should run on if no port given on command-line */
int DFLT_PORT = 4000;

/* maximum number of players allowed before game starts to turn people away */
int MAX_PLAYERS = 300;

/* Some nameservers (such as the one here at JHU) are slow and cause the
   game to lag terribly every time someone logs in.  The lag is caused by
   the gethostbyaddr() function -- the function which resolves a numeric
   IP address (such as 128.220.13.30) into an alphabetic name (such as
   circle.cs.jhu.edu).

   The nameserver at JHU can get so bad at times that the incredible lag
   caused by gethostbyaddr() isn't worth the luxury of having names
   instead of numbers for players' sitenames.

   If your nameserver is fast, set the variable below to NO.  If your
   nameserver is slow, of it you would simply prefer to have numbers
   instead of names for some other reason, set the variable to YES.

   You can experiment with the setting of nameserver_is_slow on-line using
   the SLOWNS command from within the MUD.
*/

byte nameserver_is_slow = YES;
   

char *MENU = 
"\n\r"
"Welcome to CircleMUD!\n\r"
"0) Exit from CircleMUD.\n\r"
"1) Enter the game.\n\r"
"2) Enter description.\n\r"
"3) Read the background story.\n\r"
"4) Change password.\n\r"
"5) Delete this character.\n\r"
"\n\r"
"   Make your choice: ";



char *GREETINGS =

"\n\r\n\r"
"                             CircleMUD  2.0\n\r"
"\n\r"
"                      Based on DikuMUD (GAMMA 0.0)\n\r"
"                               Created by\n\r"
"                  Hans Henrik Staerfeldt, Katja Nyboe,\n\r"
"           Tom Madsen, Michael Seifert, and Sebastian Hammer\n\r\n\r";

char *WELC_MESSG =
"\n\r"
"Welcome to the land of CircleMUD!  May your visit here be... Interesting."
"\n\r\n\r";

