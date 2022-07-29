/* ************************************************************************
*  file: act.obj1.c , Implementation of commands.         Part of DIKUMUD *
*  Usage : Commands mainly moving around objects.                         *
*  Copyright (C) 1990, 1991 - see 'license.doc' for complete information. *
************************************************************************* */

#include <stdio.h>
#include <string.h>

#include "structs.h"
#include "utils.h"
#include "comm.h"
#include "interpreter.h"
#include "handler.h"
#include "db.h"
#include "spells.h"

/* extern variables */

extern struct str_app_type str_app[];
extern struct room_data *world;
extern struct descriptor_data *descriptor_list;

/* extern functions */
char	*fname(char *namelist);
int	isname(char *arg, char *arg2);
struct obj_data *create_money( int amount );
ACMD(do_put);


/* procedures related to get */
void	get(struct char_data *ch, struct obj_data *obj_object, 
struct obj_data *sub_object)
{
   char	buffer[MAX_STRING_LENGTH];

   if (sub_object) {
      obj_from_obj(obj_object);
      obj_to_char(obj_object, ch);
      if (sub_object->carried_by == ch) {
	 act("You get $p from $P.", 0, ch, obj_object, sub_object, TO_CHAR);
	 act("$n gets $p from $s $P.", 1, ch, obj_object, sub_object, TO_ROOM);
      } else {
	 act("You get $p from $P.", 0, ch, obj_object, sub_object, TO_CHAR);
	 act("$n gets $p from $P.", 1, ch, obj_object, sub_object, TO_ROOM);
      }
   } else {
      obj_from_room(obj_object);
      obj_to_char(obj_object, ch);
      act("You get $p.", 0, ch, obj_object, 0, TO_CHAR);
      act("$n gets $p.", 1, ch, obj_object, 0, TO_ROOM);
   }
   if ((obj_object->obj_flags.type_flag == ITEM_MONEY) && 
       (obj_object->obj_flags.value[0] >= 1)) {
      obj_from_char(obj_object);
      sprintf(buffer, "There were %d coins.\n\r", obj_object->obj_flags.value[0]);
      send_to_char(buffer, ch);
      GET_GOLD(ch) += obj_object->obj_flags.value[0];
      extract_obj(obj_object);
   }
}


ACMD(do_get)
{
   char	arg1[MAX_STRING_LENGTH];
   char	arg2[MAX_STRING_LENGTH];
   struct obj_data *sub_object;
   struct obj_data *obj_object;
   struct obj_data *next_obj;
   bool found = FALSE;
   bool fail  = FALSE;
   int	type   = 3;
   bool alldot = FALSE;


   argument_interpreter(argument, arg1, arg2);

   /* get type */
   if (!*arg1) {
      type = 0;
   }
   if (*arg1 && !*arg2) {
      alldot = FALSE;
      buf2[0] = '\0';
      if ((str_cmp(arg1, "all") != 0) && 
          (sscanf(arg1, "all.%s", buf2) != 0)) {
	 strcpy(arg1, "all");
	 alldot = TRUE;
      }
      if (!str_cmp(arg1, "all")) {
	 type = 1;
      } else {
	 type = 2;
      }
   }

   if (*arg1 && *arg2) {
      alldot = FALSE;
      buf2[0] = '\0';
      if ((str_cmp(arg1, "all") != 0) && 
          (sscanf(arg1, "all.%s", buf2) != 0)) {
	 strcpy(arg1, "all");
	 alldot = TRUE;
      }
      if (!str_cmp(arg1, "all")) {
	 if (!str_cmp(arg2, "all")) {
	    type = 3;
	 } else {
	    type = 4;
	 }
      } else {
	 if (!str_cmp(arg2, "all")) {
	    type = 5;
	 } else {
	    type = 6;
	 }
      }
   }

   switch (type) {
      /* get */
   case 0:
      send_to_char("Get what?\n\r", ch);
      break;
      /* get all */
   case 1:
      sub_object = 0;
      found = FALSE;
      fail	 = FALSE;
      for (obj_object = world[ch->in_room].contents; 
          obj_object; 
          obj_object = next_obj) {
	 next_obj = obj_object->next_content;

	 /* IF all.obj, only get those named "obj" */
	 if (alldot && !isname(buf2, obj_object->name)) {
	    continue;
	 } /* if */

	 if (CAN_SEE_OBJ(ch, obj_object)) {
	    if ((IS_CARRYING_N(ch) + 1) <= CAN_CARRY_N(ch)) {
	       if ((IS_CARRYING_W(ch) + obj_object->obj_flags.weight) <= 
	           CAN_CARRY_W(ch)) {
		  if (CAN_WEAR(obj_object, ITEM_TAKE)) {
		     get(ch, obj_object, sub_object);
		     found = TRUE;
		  } else {
		     send_to_char("You can't take that!\n\r", ch);
		     fail = TRUE;
		  }
	       } else {
		  sprintf(buf, "%s : You can't carry that much weight.\n\r", 
		      (obj_object->short_description));
		  send_to_char(buf, ch);
		  fail = TRUE;
	       }
	    } else {
	       sprintf(buf, "%s : You can't carry that many items.\n\r", 
	           (obj_object->short_description));
	       send_to_char(buf, ch);
	       fail = TRUE;
	    }
	 }
      }
      if (found) {
	 send_to_char("OK.\n\r", ch);
      } else {
	 if (!fail)
	    send_to_char("You see nothing here.\n\r", ch);
      }
      break;
      /* get ??? */
   case 2:
      sub_object = 0;
      found = FALSE;
      fail	 = FALSE;
      obj_object = get_obj_in_list_vis(ch, arg1, 
          world[ch->in_room].contents);
      if (obj_object) {
	 if ((IS_CARRYING_N(ch) + 1 < CAN_CARRY_N(ch))) {
	    if ((IS_CARRYING_W(ch) + obj_object->obj_flags.weight) < 
	        CAN_CARRY_W(ch)) {
	       if (CAN_WEAR(obj_object, ITEM_TAKE)) {
		  get(ch, obj_object, sub_object);
		  found = TRUE;
	       } else {
		  send_to_char("You can't take that!\n\r", ch);
		  fail = TRUE;
	       }
	    } else {
	       sprintf(buf, "%s : You can't carry that much weight.\n\r", 
	           (obj_object->short_description));
	       send_to_char(buf, ch);
	       fail = TRUE;
	    }
	 } else {
	    sprintf(buf, "%s : You can't carry that many items.\n\r", 
	        (obj_object->short_description));
	    send_to_char(buf, ch);
	    fail = TRUE;
	 }
      } else {
	 sprintf(buf, "You do not see a %s here.\n\r", arg1);
	 send_to_char(buf, ch);
	 fail = TRUE;
      }
      break;
      /* get all all */
   case 3:
      send_to_char("You must be joking?!\n\r", ch);
      break;
      /* get all ??? */
   case 4:
      found = FALSE;
      fail	 = FALSE;
      sub_object = get_obj_in_list_vis(ch, arg2, 
          world[ch->in_room].contents);
      if (!sub_object) {
	 sub_object = get_obj_in_list_vis(ch, arg2, ch->carrying);
      }
      if (sub_object) {
	 if (GET_ITEM_TYPE(sub_object) == ITEM_CONTAINER) {
	    if (IS_SET(sub_object->obj_flags.value[1], CONT_CLOSED)) {
	       sprintf(buf, "The %s is closed.\n\r", fname(sub_object->name));
	       send_to_char(buf, ch);
	       return;
	    } /* if */
	    for (obj_object = sub_object->contains; 
	        obj_object; 
	        obj_object = next_obj) {
	       next_obj = obj_object->next_content;

	       /* IF all.obj, only get those named "obj" */
	       if (alldot && !isname(buf2, obj_object->name) ) {
		  continue;
	       } /* if */

	       if (CAN_SEE_OBJ(ch, obj_object)) {
		  if ((IS_CARRYING_N(ch) + 1 < CAN_CARRY_N(ch))) {
		     if ((IS_CARRYING_W(ch) + obj_object->obj_flags.weight) < 
		         CAN_CARRY_W(ch)) {
			if (CAN_WEAR(obj_object, ITEM_TAKE)) {
			   get(ch, obj_object, sub_object);
			   found = TRUE;
			} else {
			   send_to_char("You can't take that!\n\r", ch);
			   fail = TRUE;
			}
		     } else {
			sprintf(buf, "%s : You can't carry that much weight.\n\r",
			    (obj_object->short_description));
			send_to_char(buf, ch);
			fail = TRUE;
		     }
		  } else {
		     sprintf(buf, "%s : You can't carry that many items.\n\r",
		         (obj_object->short_description));
		     send_to_char(buf, ch);
		     fail = TRUE;
		  }
	       }
	    }
	    if (!found && !fail) {
	       sprintf(buf, "You do not see anything in the %s.\n\r", 
	           fname(sub_object->name));
	       send_to_char(buf, ch);
	       fail = TRUE;
	    }
	 } else {
	    sprintf(buf, "The %s is not a container.\n\r",
	        fname(sub_object->name));
	    send_to_char(buf, ch);
	    fail = TRUE;
	 }
      } else {
	 sprintf(buf, "You do not see or have the %s.\n\r", arg2);
	 send_to_char(buf, ch);
	 fail = TRUE;
      }
      break;
   case 5:
      send_to_char("You can't take a thing from more than one container.\n\r", ch);
      break;
   case 6:
      found = FALSE;
      fail	 = FALSE;
      sub_object = get_obj_in_list_vis(ch, arg2, 
          world[ch->in_room].contents);
      if (!sub_object) {
	 sub_object = get_obj_in_list_vis(ch, arg2, ch->carrying);
      }
      if (sub_object) {
	 if (GET_ITEM_TYPE(sub_object) == ITEM_CONTAINER) {
	    if (IS_SET(sub_object->obj_flags.value[1], CONT_CLOSED)) {
	       sprintf(buf, "The %s is closed.\n\r", fname(sub_object->name));
	       send_to_char(buf, ch);
	       return;
	    } /* if */
	    obj_object = get_obj_in_list_vis(ch, arg1, sub_object->contains);
	    if (obj_object) {
	       if ((IS_CARRYING_N(ch) + 1 < CAN_CARRY_N(ch))) {
		  if ((IS_CARRYING_W(ch) + obj_object->obj_flags.weight) < 
		      CAN_CARRY_W(ch)) {
		     if (CAN_WEAR(obj_object, ITEM_TAKE)) {
			get(ch, obj_object, sub_object);
			found = TRUE;
		     } else {
			send_to_char("You can't take that!\n\r", ch);
			fail = TRUE;
		     }
		  } else {
		     sprintf(buf, "%s : You can't carry that much weight.\n\r",
		         (obj_object->short_description));
		     send_to_char(buf, ch);
		     fail = TRUE;
		  }
	       } else {
		  sprintf(buf, "%s: You can't carry that many items.\n\r", 
		      (obj_object->short_description));
		  send_to_char(buf, ch);
		  fail = TRUE;
	       }
	    } else {
	       sprintf(buf, "The %s does not contain the %s.\n\r", 
	           fname(sub_object->name), arg1);
	       send_to_char(buf, ch);
	       fail = TRUE;
	    }
	 } else {
	    sprintf(buf, "The %s is not a container.\n\r", fname(sub_object->name));
	    send_to_char(buf, ch);
	    fail = TRUE;
	 }
      } else {
	 sprintf(buf, "You do not see or have the %s.\n\r", arg2);
	 send_to_char(buf, ch);
	 fail = TRUE;
      }

      break;
   }
}


#define DONATION_ROOM_1		3063
#define DONATION_ROOM_2		0
#define DONATION_ROOM_3		0

ACMD(do_drop)
{
   char	sname[100];
   int	amount;
   struct obj_data *tmp_object;
   struct obj_data *next_obj;
   int	test = FALSE;
   int	value = 0;
   int	junk = 0, donate = 0, RDR = 0;

   switch (subcmd) {
   case SCMD_JUNK:
      strcpy(sname, "junk");
      junk = 1;
      break;
   case SCMD_DONATE:
      strcpy(sname, "donate");
      donate = 1;
      switch (number(0, 2)) {
      case 0:
	 donate = 0;
	 junk = 1;
	 break;
      case 1:
      case 2: 
	 RDR = real_room(DONATION_ROOM_1); 
	 break;
	 /*			case 3: RDR = real_room(DONATION_ROOM_2); break;
			case 4: RDR = real_room(DONATION_ROOM_3); break;
*/
      }
      if (RDR == NOWHERE) {
	 send_to_char("Sorry, you can't donate anything right now.\n\r", ch);
	 return;
      }
      break;
   default:
      strcpy(sname, "drop");
      break;
   }

   argument = one_argument(argument, arg);
   if (is_number(arg)) {
      amount = atoi(arg);
      argument = one_argument(argument, arg);
      if (str_cmp("coins", arg) && str_cmp("coin", arg)) {
	 send_to_char("Sorry, you can't do that...\n\r", ch);
	 return;
      }
      if (amount <= 0) {
	 send_to_char("Sorry, you can't do that!\n\r", ch);
	 return;
      }
      if (GET_GOLD(ch) < amount) {
	 send_to_char("You haven't that many coins!\n\r", ch);
	 return;
      }

      if (!junk) {
	 tmp_object = create_money(amount);
	 if (donate) {
	    send_to_char("You throw some gold into the air where it disappears in a puff of smoke!\n\r",
	         ch);
	    act("$n throws some gold into the air where it disappears in a puff of smoke!", FALSE, ch,
	         0, 0, TO_ROOM);
	    send_to_room("Some gold suddenly appears in mid-air, then drops to the ground!\n\r", RDR);
	    obj_to_room(tmp_object, RDR);
	 } else {
	    send_to_char("You drop some gold.\n\r", ch);
	    act("$n drops some gold.", FALSE, ch, 0, 0, TO_ROOM);
	    obj_to_room(tmp_object, ch->in_room);
	 }
      } else {
	 act("$n drops some gold which disappears in a puff of smoke!",
	     FALSE, ch, 0, 0, TO_ROOM);
	 send_to_char(
	     "You drop some gold which disappears in a puff of smoke!\n\r", ch);
      }
      GET_GOLD(ch) -= amount;
      return;
   }

   if (!*arg) {
      sprintf(buf, "What do you want to %s?\n\r", sname);
      send_to_char(buf, ch);
      return;
   }

   /* Can't junk or donate all */
   if ((donate || junk) && !str_cmp(arg, "all")) {
      if (junk) 
	 send_to_char("Go to the dump if you want to junk EVERYTHING!\n\r", ch);
      else 
	 send_to_char("Go do the donation room if you want to donate EVERYTHING!\n\r", ch);
      return;
   }

   /* If it is "all" or "all." */
   if (!str_cmp(arg, "all") || !strn_cmp(arg, "all.", 4)) {
      for (tmp_object = ch->carrying; tmp_object; tmp_object = next_obj) {
	 next_obj = tmp_object->next_content;
	 if (*(arg + 3) == '.')
	    if (!isname(arg + 4, tmp_object->name))
	       continue;

	 if (IS_SET(tmp_object->obj_flags.extra_flags, ITEM_NODROP)) {
	    sprintf(buf, "You can't %s %s, it must be CURSED!\n\r",
	        sname, OBJS(tmp_object, ch));
	    send_to_char(buf, ch);
	    test = TRUE;
	    continue;
	 }

	 /* CHECK FOR DONATE FLAG - SPG 4/4/93*/
	 if (IS_SET(tmp_object->obj_flags.extra_flags, ITEM_NODONATE)
	      && donate) {
	    sprintf(buf, "You can't donate %s.\n\r", OBJS(tmp_object, ch));
	    send_to_char(buf, ch);
	    test = TRUE;
	    continue;
	 }

	 sprintf(buf, "You %s %s.%s\n\r", sname, OBJS(tmp_object, ch),
	     (junk || donate) ? "  It vanishes in a puff of smoke!" : "");
	 send_to_char(buf, ch);
	 sprintf(buf, "$n %ss $p.%s", sname, 
	     (junk || donate) ? "  It vanishes in a puff of smoke!" : "");
	 act(buf, TRUE, ch, tmp_object, 0, TO_ROOM);
	 if (junk) {
	    test = TRUE;
	    value += MAX(1, MIN(200, tmp_object->obj_flags.cost >> 4));
	    extract_obj(tmp_object);
	 } else {
	    test = TRUE;
	    obj_from_char(tmp_object);
	    if (donate) {
	       obj_to_room(tmp_object, RDR);
	       send_to_room("Something suddenly appears in a puff a smoke!\n\r", RDR);
	    } else 
	       obj_to_room(tmp_object, ch->in_room);
	 }
      }
   } else {

      /* It is not "all" or "all." */
      tmp_object = get_obj_in_list_vis(ch, arg, ch->carrying);
      if (tmp_object) {
	 test = 1;
	 if (IS_SET(tmp_object->obj_flags.extra_flags, ITEM_NODROP)) {
	    sprintf(buf, "You can't %s it, it must be CURSED!\n\r", sname);
	    send_to_char(buf, ch);
	    return;
	 }

	 /* CHECK FOR DONATE FLAG - SPG 4/4/93*/
	 if (IS_SET(tmp_object->obj_flags.extra_flags, ITEM_NODONATE)
	      && donate) {
	    sprintf(buf, "You can't %s it.\n\r", sname);
	    send_to_char(buf, ch);
	    return;
	 }
	 sprintf(buf, "You %s %s.%s\n\r", sname, OBJS(tmp_object, ch),
	     (junk || donate) ? "  It vanishes in a puff of smoke!" : "");
	 send_to_char(buf, ch);
	 sprintf(buf, "$n %ss $p.%s", sname, 
	     (junk || donate) ? "  It vanishes in a puff of smoke!" : "");
	 act(buf, TRUE, ch, tmp_object, 0, TO_ROOM);
	 if (junk) {
	    value = MAX(1, MIN(200, tmp_object->obj_flags.cost >> 4));
	    extract_obj(tmp_object);
	 } else {
	    obj_from_char(tmp_object);
	    if (donate) {
	       obj_to_room(tmp_object, RDR);
	       send_to_room("Something suddenly appears in a puff of smoke!\n\r", RDR);
	    } else 
	       obj_to_room(tmp_object, ch->in_room);
	 }
      }
   }

   if (!test)
      send_to_char("You do not seem to have anything like that.\n\r", ch);

   if (value && subcmd == SCMD_JUNK) {
      send_to_char("You have been rewarded by the gods!\n\r", ch);
      act("$n has been rewarded by the gods!", TRUE, ch, 0, 0, TO_ROOM);
      GET_GOLD(ch) += value;
   }
}


void	do_putalldot(struct char_data *ch, char *name, char *target, int cmd)
{
   struct obj_data *tmp_object;
   struct obj_data *next_object;
   bool found = FALSE;
   char buf[100];

   /* If "put all.object bag", get all carried items
         * named "object", and put each into the bag.
         */
   for (tmp_object = ch->carrying; tmp_object; 
       tmp_object = next_object) {
      next_object = tmp_object->next_content;
      if (isname(name, tmp_object->name) ) {
	 sprintf(buf, "%s %s", name, target);
	 found = TRUE;
	 do_put(ch, buf, cmd, 0);
      }
   }
   if (!found) {
      send_to_char("You don't have one.", ch);
   } /* if */
}


ACMD(do_put)
{
   char	arg1[MAX_STRING_LENGTH];
   char	arg2[MAX_STRING_LENGTH];
   struct obj_data *obj_object;
   struct obj_data *sub_object;
   struct char_data *tmp_char;
   int	bits;

   argument_interpreter(argument, arg1, arg2);
   if (*arg1) {
      if (*arg2) {
	 buf2[0] = '\0';
	 if (sscanf(arg1, "all.%s", buf2) != 0) {
	    do_putalldot(ch, buf2, arg2, cmd);
	    return;
	 }
	 obj_object = get_obj_in_list_vis(ch, arg1, ch->carrying);
	 if (obj_object) {
	    bits = generic_find(arg2, FIND_OBJ_INV | FIND_OBJ_ROOM,
	        ch, &tmp_char, &sub_object);
	    if (sub_object) {
	       if (GET_ITEM_TYPE(sub_object) == ITEM_CONTAINER) {
		  if (!IS_SET(sub_object->obj_flags.value[1], CONT_CLOSED)) {
		     if (obj_object == sub_object) {
			send_to_char("You attempt to fold it into itself, but fail.\n\r", ch);
			return;
		     }
		     if (((sub_object->obj_flags.weight) + 
		         (obj_object->obj_flags.weight)) < 
		         (sub_object->obj_flags.value[0])) {
			send_to_char("Ok.\n\r", ch);
			if (bits == FIND_OBJ_INV) {
			   obj_from_char(obj_object);
			   /* make up for above line */
			   IS_CARRYING_W(ch) += GET_OBJ_WEIGHT(obj_object);
			   /*	obj_from_char(sub_object); do not rearrange order... */
			   obj_to_obj(obj_object, sub_object);
			   /*	obj_to_char(sub_object,ch);do not rearrange order... */
			} else {
			   obj_from_char(obj_object);
			   /* Do we need obj_from_room???(sub_object,....); */
			   obj_to_obj(obj_object, sub_object);
			   /* Dow we need obj_to_room???(sub_object,ch);    */
			}

			act("$n puts $p in $P.", TRUE, ch, obj_object, sub_object, TO_ROOM);
		     } else {
			send_to_char("It won't fit.\n\r", ch);
		     }
		  } else
		     send_to_char("It seems to be closed.\n\r", ch);
	       } else {
		  sprintf(buf, "The %s is not a container.\n\r", fname(sub_object->name));
		  send_to_char(buf, ch);
	       }
	    } else {
	       sprintf(buf, "You dont have the %s.\n\r", arg2);
	       send_to_char(buf, ch);
	    }
	 } else {
	    sprintf(buf, "You dont have the %s.\n\r", arg1);
	    send_to_char(buf, ch);
	 }
      } else {
	 sprintf(buf, "Put %s in what?\n\r", arg1);
	 send_to_char(buf, ch);
      }
   } else {
      send_to_char("Put what in what?\n\r", ch);
   }
}





ACMD(do_give)
{
   char	obj_name[80], vict_name[80];
   int	amount;
   struct char_data *vict;
   struct obj_data *obj;

   argument = one_argument(argument, obj_name);
   if (is_number(obj_name)) {
      amount = atoi(obj_name);
      argument = one_argument(argument, arg);
      if (str_cmp("coins", arg) && str_cmp("coin", arg)) {
	 send_to_char("Sorry, you can't do that (yet)...\n\r", ch);
	 return;
      }
      if (amount < 0) {
	 send_to_char("Sorry, you can't do that!\n\r", ch);
	 return;
      }
      if ((GET_GOLD(ch) < amount) && (IS_NPC(ch) || (GET_LEVEL(ch) < LEVEL_GOD))) {
	 send_to_char("You haven't got that many coins!\n\r", ch);
	 return;
      }
      argument = one_argument(argument, vict_name);
      if (!*vict_name) {
	 send_to_char("To who?\n\r", ch);
	 return;
      }
      if (!(vict = get_char_room_vis(ch, vict_name))) {
	 send_to_char("To who?\n\r", ch);
	 return;
      }
      send_to_char("Ok.\n\r", ch);
      sprintf(buf, "%s gives you %d gold coins.\n\r", PERS(ch, vict), amount);
      send_to_char(buf, vict);
      act("$n gives some gold to $N.", 1, ch, 0, vict, TO_NOTVICT);
      if (IS_NPC(ch) || (GET_LEVEL(ch) < LEVEL_GOD))
	 GET_GOLD(ch) -= amount;
      GET_GOLD(vict) += amount;
      return;
   }

   argument = one_argument(argument, vict_name);


   if (!*obj_name || !*vict_name) {
      send_to_char("Give what to who?\n\r", ch);
      return;
   }
   if (!(obj = get_obj_in_list_vis(ch, obj_name, ch->carrying))) {
      send_to_char("You do not seem to have anything like that.\n\r",
          ch);
      return;
   }
   if (IS_SET(obj->obj_flags.extra_flags, ITEM_NODROP)) {
      send_to_char("You can't let go of it! Yeech!!\n\r", ch);
      return;
   }
   if (!(vict = get_char_room_vis(ch, vict_name))) {
      send_to_char("No one by that name around here.\n\r", ch);
      return;
   }

   if ((1 + IS_CARRYING_N(vict)) > CAN_CARRY_N(vict)) {
      act("$N seems to have $S hands full.", 0, ch, 0, vict, TO_CHAR);
      return;
   }
   if (obj->obj_flags.weight + IS_CARRYING_W(vict) > CAN_CARRY_W(vict)) {
      act("$E can't carry that much weight.", 0, ch, 0, vict, TO_CHAR);
      return;
   }
   obj_from_char(obj);
   obj_to_char(obj, vict);
   act("$n gives $p to $N.", 1, ch, obj, vict, TO_NOTVICT);
   act("$n gives you $p.", 0, ch, obj, vict, TO_VICT);
   send_to_char("Ok.\n\r", ch);
}



ACMD(do_split)
{
   int	amount, num, share;
   struct char_data *k;
   struct follow_type *f;

   if (IS_NPC(ch))
      return;

   one_argument(argument, buf);

   if (is_number(buf)) {
      amount = atoi(buf);
      if (amount <= 0) {
	 send_to_char("Sorry, you can't do that.\n\r", ch);
	 return;
      }

      if (amount > GET_GOLD(ch)) {
	 send_to_char("You don't seem to have that much gold to split.\n\r", ch);
	 return;
      }

      if (ch->master)
	 k = ch->master;
      else
	 k = ch;

      if (IS_AFFECTED(k, AFF_GROUP) && (k->in_room == ch->in_room))
	 num = 1;
      else
	 num = 0;

      for (f = k->followers; f; f = f->next)
	 if (IS_AFFECTED(f->follower, AFF_GROUP) && 
	     (!IS_NPC(f->follower)) && 
	     (f->follower->in_room == ch->in_room))
	    num++;

      if (num && IS_AFFECTED(ch, AFF_GROUP))
	 share = amount / num;
      else {
	 send_to_char("With whom do you wish to share your gold?\n\r", ch);
	 return;
      }

      GET_GOLD(ch) -= share * (num - 1);

      if (IS_AFFECTED(k, AFF_GROUP) && (k->in_room == ch->in_room)
           && !(IS_NPC(k)) &&  k != ch) {
	 GET_GOLD(k) += share;
	 sprintf(buf, "%s splits %d coins; you receive %d.\n\r", GET_NAME(ch),
	     amount, share);
	 send_to_char(buf, k);
      }

      for (f = k->followers; f; f = f->next) {
	 if (IS_AFFECTED(f->follower, AFF_GROUP) && 
	     (!IS_NPC(f->follower)) && 
	     (f->follower->in_room == ch->in_room) && 
	     f->follower != ch) {
	    GET_GOLD(f->follower) += share;
	    sprintf(buf, "%s splits %d coins; you receive %d.\n\r", GET_NAME(ch),
	        amount, share);
	    send_to_char(buf, f->follower);
	 }
      }
      sprintf(buf, "You split %d coins among %d members; %d coins each.\n\r",
          amount, num, share);
      send_to_char(buf, ch);
   } else {
      send_to_char("How many coins do you wish to split with your group??\n\r", ch);
      return;
   }
}


