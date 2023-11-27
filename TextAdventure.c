/**************************************************
 * Lab:         Lecture 11
 * Name: Seth Glasscock
 * Description: Text adventure Game.
 * ***********************************************/

//Notes: Need to make graphics such as examine to see certain items, like the Grand Window

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
//#include "save.txt"
// Useful defines:
// constants used for the 4 directions
#define N 0
#define E 1
#define S 2
#define W 3

// room structure
struct room {
  char *name;    // name of the room
  struct room *exit[4]; // possible exits N,E,S,W
  char *description;
};

// item structure
struct item {
  char *name;    // name of the room
  struct room *location;  // where is this item located, null for player inventory
};



// Map...
//                         EXIT
//                          |
//  ? -- ? -- ?    BR:0 -- FD:1 -- LB:2
//  |         |     |       |      
//  ? -- ?    ? -- WH:3 -- GH:4 -- EH:5
//       |          |       |       |
//       ?         St:6    GW:7 -- CY:8
//
struct room map[] = {
  {  // 0
    .name = "Bed Room",
    .exit = {NULL, &map[1], &map[3], NULL},
    .description = "It has a giant bed that looks pretty warm and comfy. you also see a Sign on the mantle."
  },
  {  // 1
    .name = "Front Door",
    .exit = {NULL, &map[2], &map[4], &map[0]},
    .description = "This looks like the Door to Escape but its locked."
  },
  {  // 2
    .name = "Library",
    .exit = {NULL, NULL, NULL, &map[1]},
    .description = "This place is huge, and there are so many different books in here."
  },
  {  // 3
    .name = "West Hallway",
    .exit = {&map[0], &map[4], &map[6], &map[9]},
    .description = "You find yourself on the west side of the building in a long corridor with a giant red rug"
  },  
  {  // 4
    .name = "Grand Hall",
    .exit = {&map[1], &map[5], &map[7], &map[3]},
    .description = "This room is really big and has a golden chandelier."
  },
  {  // 5
    .name = "East Hall",
    .exit = {NULL, NULL, &map[8], &map[4]},
    .description = "You find yourself on the east side of the building in a long corridor with a giant purple rug"
  },
  {  // 6
    .name = "Storage Room",
    .exit = {&map[3], NULL, NULL, NULL},
    .description = "This room is big and has a lot of storage crates in the darkness"
  },
  {  // 7
    .name = "Grand Window",
    .exit = {&map[4], &map[8], NULL, NULL},
    .description = "You look upon the Grand Window and it looks like you can examine it closer."
  },
  {  // 8
    .name = "CourtYard",
    .exit = {&map[5], NULL, NULL, &map[7]},
    .description = "You look across the barren backyard where you find mounds of dirt and a Gravestone"
  },
  {  // 9
    .name = "???",
    .exit = {&map[10], &map[3], &map[3], &map[3]},
    .description = "You faintly hear eerie music."
  },
  {  // 10
    .name = "???",
    .exit = {&map[3], &map[3], &map[3], &map[11]},
    .description = "You faintly hear eerie music, it sounds like a music box."
  },
  {  // 11
    .name = "???",
    .exit = {&map[3], &map[3], &map[3], &map[12]},
    .description = "The music is starting to come closer."
  },
  {  // 12
    .name = "???",
    .exit = {&map[3], &map[3], &map[13], &map[3]},
    .description = "Each room is looking like each other."
  },
  {  // 13
    .name = "???",
    .exit = {&map[3], &map[14], &map[3], &map[3]},
    .description = "The music is getting extreamaly loud now and its leaving ringing in your ears."
  },
  {  // 14
    .name = "???",
    .exit = {&map[3], &map[3], &map[15], &map[3]},
    .description = "You gasp for breath as you have already been here for too long."
  },
  {  // 15
    .name = "Treasure Room",
    .exit = {&map[3], &map[3], &map[3], &map[3]},
    .description = "You found the music box, and it starts playing a familar tune."
  },
   {  // 15
    .name = "Place Holder",
    .exit = {NULL, NULL, NULL, NULL},
    .description = "Just A Room That Is A Place Holder."
  },
};

// Iitial objects and their locations
struct item obj[] = {
  { "Step Ladder", &map[2] },//obj 0
  { "Master Key", &map[16] /*NULL*/},//obj 1
  { "Shovel", &map[16] /*NULL*/},//obj 2
  { "Sword", &map[15] /*NULL*/ },//obj 3
  { "Candle", &map[4] },//obj 4
  { "Short Stick", NULL },//obj 5
  { "Sign", &map[0]},//obj 6
  { "Gravestone", &map[8]},//obj 7
  { "Crate", &map[6]},//obj 8
  { "Dirt", &map[8]},//obj 9
  { "Carvings", &map[9]},//obj 10
  { "Window", &map[7]},//obj 11
  { "Door", &map[1]},//obj 12
  { "Map", &map[15]},//obj 13
  { NULL, NULL } // null item to flag the end of the list
};

// function declarations
void init_game(void);
void game_help(void);    // prints basic instructions for use
void print_direction(struct room*);
void print_inventory(struct room*);
void print_pinventory(struct room*);
void print_description(struct room*);
void print_help();
void print_movement(struct room*);
void print_grab(struct room*);
void print_drop(struct room*);
void print_read(struct room*);
void print_use(struct room*);
void print_kill(struct room*);
void print_map(struct room*);
void print_break(struct room*);
void print_dig(struct room*);
void print_display(struct room*);
void print_examine(struct room*);
void print_saveloc();
void print_return();
// command proc
bool get_command(void);// returns false for exit game
char command_buffer[100]; // store input from user
char *verb;  // current command verb
char *noun;  // second part of user input


// Globals...
struct room *player_location;

int main(int argc, char *str[]) {
  init_game();
  
  if(argc >= 2){
    if((strcmp(str[1], "load") == 0)){
      print_return();
      printf("Loading Game...\n\nWelcome back Player\n\n");
    }
     else{
       printf("Current error has occurred. If you have a save file try typing -./main load-. If you do not have a save file, just use -./main-.");
       exit(0);
     } 
  }
  

  printf("Welcome to Links Escape\n   You find yourself stuck in a building filled with puzzles and items, can you escape?\n");
  // TBD: Add intro text and initial instructions.
  game_help();

  while(get_command()){
   if (strcmp(verb, "use") == 0){
      //Only one item uses this command and its the Master Key
      
  
    if(obj[1].location == NULL && player_location == &map[1]){
      printf(" you turned the Key in the door and pushed it open. \n");
        break;
    }
    if(obj[1].location != NULL && player_location == &map[1]){
      printf("   you you cannot use what you do not have. \n");
        
    }
    
   }
    
    if(strcmp(verb, "load")==0){
     print_return();
     printf("Loading Game...\n\nWelcome back Player\n");
      //Allows the Player to come back where they or another left off last time.
    }
    
  if(strcmp(verb, "quit") == 0){
    exit(0);
  }
  //This just ends the game.

   if(strcmp(verb, "kill") == 0){
      if(strcmp(noun, "yourself") == 0){
        printf("-you lie there in your warm blood while your vision gets hazy \n -Game over-");
        exit(0);
      }else if(strcmp(noun, "yourself") != 0){
        printf("nothing happened");
      }
     } 
    //Just one that i put in for fun and is an easter egg. Please don't take points off for this one.
    
  if(strcmp(verb, "break") == 0){
    print_break(player_location);
  }
    //This function allows player to break the Crate in the storage room.

  if(strcmp(verb, "dig") == 0){
    print_dig(player_location);
  }
   //This function allows the player to dig the dirt after getting the Shovel.
      
    // process command
    if(strcmp(verb, "help") == 0){
      print_help();
    }
    //This function just lets the player see all of His or Her actions that they can do.
    
    if(strcmp(verb, "look") == 0){
      printf("\n you looked in the %s \n", (*player_location).name);
      print_description(player_location);
      print_direction(player_location);
      
    }
    //Just a basic look function that lets player know where they are.
    
    if(strcmp(verb, "des") == 0){
      printf("\nYou looked at all the features of the %s \n", (*player_location).name);
      ;
      print_inventory(player_location);
    }//Tells the player if there are any items or anything in that certain room.
    
    if(strcmp(verb, "inv") == 0){
      print_pinventory(NULL);
    }//Shows the player the items in their Inventory

    if(strcmp(verb, "grab") == 0){
      print_grab(player_location);
    }//Lets the Player Pick up items around the Game

    if(strcmp(verb, "drop") == 0){
      print_drop(player_location);
    }//Lets the player drop items out of their inventory.

      
    if(strcmp(verb, "go") == 0){
      print_movement(player_location);
    }//Basic moving function.

    if(strcmp(verb, "read") == 0){
      print_read(player_location);
    }//Allows the player to read certain texts in the game.

    if(strcmp(verb, "map") == 0){
      if(obj[13].location == NULL){
    print_map(player_location);
        printf("\n you are in *\n");
        
   }//Shows the player the Map of the game and the rooms that they are and aren't in.
     else if(obj[13].location != NULL){
        
      printf("\n you tried to read it but you do not have one\n");
     }
    }

    if(strcmp(verb, "dis") == 0){
      print_display(player_location);
    }//Shows player a compass and the ways they can go next without having to use the look command, also after picking up the Candle this cammand runs automatically.

    if(strcmp(verb, "examine") == 0){
      print_examine(player_location);
    }//Allos player to see the actual item that they want to see in pixal art form.

    if(strcmp(verb, "save") == 0){
      print_saveloc(player_location);
      printf("Saving progress");
      exit(0);
    }//save function so the player can quit and come back later, mixed with the load function as well.

    if(strcmp(verb, "examine") != 0 && strcmp(verb, "use") != 0 &&strcmp(verb, "map") != 0 &&strcmp(verb, "dis") != 0 &&strcmp(verb, "read") != 0 &&strcmp(verb, "go") != 0 &&strcmp(verb, "drop") != 0 &&strcmp(verb, "grab") != 0 &&strcmp(verb, "inv") != 0 &&strcmp(verb, "des") != 0 && strcmp(verb, "look") != 0 && strcmp(verb, "help") != 0 && strcmp(verb, "dig") != 0 && strcmp(verb, "break") != 0 && strcmp(verb, "kill") != 0 && strcmp(verb, "quit") != 0 && strcmp(verb, "load") != 0){
      printf("  Sorry, that is not a command or the spelling is wrong\n");
      
    }
  }
 //return 1;
  printf("\nCongratulations! You have escaped.\n");
  printf("\nBig thanks for the beta testers: Kyler Kellogg, Libby Amos");
  return 0;
}



void init_game(void)
{
  // start player in first room
  srand((unsigned)time(NULL));
  player_location = &map[4]; 
  for(int n = 0; obj[n].name != NULL; n++){
    if(strcmp(obj[n].name, "Map") == 0){
      obj[n].location = &map[rand()%9];
    }    
  }
  return;
}


void game_help(void)
{
  printf("\n Important information. \n");
  printf(" -Items names have special spelling and are capital sensitive. \n");
  printf(" -Commands are also case sensitive so pay attention to how you spell it. \n");
  printf(" -There is also a -help- function to give you explanations how to use commands.\n ");
  printf("-Good luck and try to have fun. \n\n");
  
  // TBD: Add basic instruction text.

  return;
}


bool get_command(void)
{
  if(obj[4].location == NULL){
    print_display(player_location);
  }
  printf("\nWhat would you like to do: ");
  fgets(command_buffer, 100, stdin);
  verb = strtok(command_buffer, " \n");
  //printf("*%s*", verb);
  noun = strtok(NULL, "\n");
  if (noun == NULL && (strcmp(verb, "go") == 0 || strcmp(verb, "grab") == 0 || strcmp(verb, "drop") == 0 || strcmp(verb, "read") == 0 || strcmp(verb, "use") == 0 || strcmp(verb, "kill") == 0 || strcmp(verb, "break") == 0 || strcmp(verb, "examine") == 0)) {
    printf("    error(add context)\n");
    get_command();  //recursive error checking
  }
    








   {// loop while command is invalid
    // warn of invalid command on all but first attempt
    // printf...

    // input to command_buffer
    // normalize input (set all to lower case)

    //get first word of buffer
    // verb = address of first word start
    // null terminate after first word
    // noun = address of start of second word or NULL if none

    // compare with valid commands

    // is command valid? break out of loop if so...
    
  }

  // return false on exit command
  return true; // return true for all command except exit
}

//Just standard moving function which lets the player move.
void print_direction(struct room *room){
  char *dir[4] = { "North","East","South","West"};
  for(int n = 0; n < 4; ++n){
    if( room->exit[n] != NULL ){
      printf(" -you can go %s \n",dir[n]);
    }
  }
}

void print_movement(struct room *room){
  switch(noun[0]){
    case 'n':
    case 'N':
      if(room->exit[N] != NULL){
        player_location = room->exit[N];
        printf("  you went north.\n");
        break;
      }
      else if(room->exit[N] == NULL){
        printf("  you cannot go north.\n");
        break;
      }
    case 'e':
    case 'E':
      if(room->exit[E] != NULL){
        player_location = room->exit[E];
        printf("  you went east.\n");
        break;
      }
      else if(room->exit[E] == NULL){
        printf("  you cannot go east.\n");
        break;
      }
    case 's':
    case 'S':
      if(room->exit[S] != NULL){
        player_location = room->exit[S];
        printf("  you went south.\n");
        break;
      }
      else if(room->exit[S] == NULL){
        printf("  you cannot go south.\n");
        break;
      }
    case 'w':
    case 'W':
      if(room->exit[W] != NULL){
        player_location = room->exit[W];
        printf("  you went west.\n");
        break;
      }
      else if(room->exit[W] == NULL){
        printf("  you cannot go west.\n");
        break;
      }
    //case ' ':
    default:
      printf("  Please do a correct direction.\n");
      break;
     }
  }


 

//Tells the player a little about the room.
void print_description(struct room *room){
      printf("  %s \n", room->description);
   
}

//Tells the player about certain items that are in the room that they are in.
void print_inventory(struct room *room){
 if(room == &map[6] && obj[8].location == &map[6]){
      printf("  you see the glint of something shiny in the Crate. \n");
        
    
 }
  if(room == &map[6] && obj[8].location == &map[16]){
      printf("  You see the shattered remains of the Crate. \n");
        
    
 }
 if(room == &map[0] && obj[6].location == &map[0]){
      printf("  You look up and see a Sign on the mantle above the fireplace. \n");
 }
   
  //else(room == &map[6])
 if(room == &map[8] && obj[1].name != NULL){
      printf("  you see a big mound of Dirt with a Gravestone. \n");
        
    
 } //This is for when player is in the secret room and the sword is there.
if(room == &map[15] && obj[3].name != NULL){
  printf(" You see a Sword in a pedestal in this room\n");
}
if(room == &map[9] || room == &map[10] || room == &map[11] || room == &map[12] ||room == &map[13] ||room == &map[14]){
  printf(" You see strange Carvings in the floor. \n");
}
if(room == &map[7] && obj[11].name != NULL){
  printf(" You gaze upon a beautiful stained glass -Window-, it also looks like you can examine it.\n");
}
if(player_location == obj[4].location && obj[4].location != NULL){
  printf(" You see a Candle that is brightly shining, it might be able to help you see.\n");
}  
if(player_location == obj[13].location && obj[13].location != NULL){
    printf(" You spot an old piece of paper and looking at it closer it appears to be a Map.\n ");
  }
  if(room == &map[1] && obj[12].name != NULL){
  printf(" You look upon and large wooden Door with a weirdly shaped keyhole.\n");
}
  
 for(int n = 0; obj[n] .name != NULL; ++n){
    if( obj[n] .location == room ){
      if(n == 3 && room == &map[15] && obj[3].name != NULL);
      else if(n == 9 && room == &map[8] && obj[1] .name != NULL);
      else if(n == 10 && (room == &map[9] || room == &map[10] || room == &map[11] || room == &map[12] ||room == &map[13] ||room == &map[14]));
      else if(n == 6 && room == &map[0]);
      else if(n == 8 && room == &map[6]);
      
      else if(n == 11 && room == &map[7]);
      else if(n == 12 && room == &map[1]);
      else if(n == 4 && player_location == obj[4].location);
      else if(n == 13 && player_location == obj[13].location);
      else if(n == 7 && room == &map[9]);
        //
      else
        printf("  there seems to be a %s in this room. \n",obj[n] .name);
    }
  }
}

//Tells player items in their inventory.
void print_pinventory(struct room *room){
 for(int n = 0; obj[n] .name != NULL; ++n){
    if( obj[n] .location == room ){
      printf(" -%s. \n",obj[n] .name);
    }
  }
}

//Read certain writing on certain items.
void print_read(struct room *room){
  if(room == &map[0] && strcmp(noun, "Sign") == 0){
    if(obj[0].location != NULL){
      printf("  you cannot read the sign from down here\n");
        return;
    }
    if(obj[0].location == NULL){
      printf("▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂\n▏ ~~~~~~~~~~~ ▕\n▏ ~~~~~~~~~~~ ▕\n▏ ~~~~~~~~~~~ ▕\n▏ ~~~~~~~~~~~ ▕\n▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔\n");
      printf("  It says 'There is a -Shovel- stored in the storage room that has been used for generations.'\n");
        return;
    }
 }
  
  
 if(room == &map[8] && strcmp(noun, "Gravestone") == 0){
   printf("       ▂▂▂▂▂▂\n       ▏    ▕\n       ▏    ▕\n  ▕▔▔▔▔~~~~~~▔▔▔▔▏\n  ▕    ~~~~~~    ▏\n  ▕▂▂▂▂~~~~~~▂▂▂▂▏\n       ▏    ▕\n       ▏    ▕\n       ▏    ▕\n       ▏    ▕\n       ▏    ▕\n       ▔▔▔▔▔▔\n");
      printf("  it says 'The Master of the house was buried with the -Master Key-.'\n");
        return; 
   }

  if((room == &map[9] || room == &map[10] || room == &map[11] || room == &map[12] ||room == &map[13] ||room == &map[14]) && strcmp(noun, "Carvings") == 0){
    printf("  It says '-Reverse ?-' the rest is too faded to read. \n");
  }
}


//Only used in one spot but allows to break the crate.
void print_break(struct room *room){
 if(room == &map[6] && obj[3].location == NULL && obj[8].location == &map[6] && strcmp(noun, "Crate") == 0){
   printf(" You shattered the Crate revealing the Shovel\n");
   obj[8].location = &map[16];
   obj[2].location = &map[6];
}
  else if(room == &map[6] && (obj[3].location != NULL || obj[8].location != &map[6]) && strcmp(noun, "Crate") == 0){
   printf(" Nothing Happened \n");
   
  }
}

//Allows player to dig Dirt.
void print_dig(struct room *room){
 if(room == &map[8] && obj[2].location == NULL && obj[9].location == &map[8] && strcmp(noun, "Dirt") == 0){
   printf(" You dug through the Dirt with your Shovel and you find the Master Key clenched in the masters hand\n");
   obj[9].location = &map[16];
   obj[1].location = &map[8];
 }
  else if(room == &map[8] && (obj[2].location != NULL || obj[9].location || &map[8]) && strcmp(noun, "Dirt") == 0){
   printf(" You tried to dig with you hands but the Dirt is too hard. \n");
  }
}

//Lets player put items into their inventory.
void print_grab(struct room *room){
  if(room == &map[15] && strcmp(noun, "Sword") == 0){
      printf("  You pulled the Sword out of the pedastal\n");
      obj[3].location = NULL;
        return;
    }
  if(room == &map[8] && strcmp(noun, "Master Key") == 0){
    if(obj[2].location != NULL){
      printf(" you look around but cannot find anything like that. \n");
        return;
      }
  }
  if(room == &map[7] && strcmp(noun, "Window") == 0){
    if(obj[11].location != NULL) {
      printf("  Why would you try to grab such an enormous object?\n");
        return;
    }  
    
  }
  if(room == &map[8] && strcmp(noun, "Gravestone") == 0){
    if(obj[7].location != NULL){
      printf(" you tried to grab Gravestone but it was too heavy. \n");
        return;
    }
  }
  if(room == &map[0] && strcmp(noun, "Sign") == 0){
    if(obj[6].location != NULL){
      printf(" you tried to grab Sign, but you are short. \n");
        return;
    }
  }
  if(room == &map[6] && strcmp(noun, "Crate") == 0){
    if(obj[8].location != NULL){
      printf(" You tried to pick up the Crate, but it is too large. \n");
        return;
    } 
  }
  if(room == &map[8] && strcmp(noun, "Dirt") == 0){
    if(obj[9].location != NULL){
      printf("  You tried to grab some Dirt but the ground is too hard to grab. \n");
        return;
    }
    } 
  if(room == &map[1] && strcmp(noun, "Door") == 0){
    if(obj[12].location != NULL){
      printf("  You tried to grab the Door but it was stuck on the hinges. \n");
        return;
    }
    } 
  if((room == &map[9] || room == &map[10] || room == &map[11] || room == &map[12] ||room == &map[13] ||room == &map[14]) && strcmp(noun, "Carvings") == 0){
    if(obj[10].location != NULL){
      printf("  You tried to grab the Carvings but they are ingraved into the stone floor. \n");
        return;
    }
  }

  
  for(int n = 0; obj[n] .name != NULL; ++n){
    if( obj[n] .location == room && strcmp(noun, obj[n] .name) == 0){
      obj[n].location = NULL;
      printf(" you grabbed %s. \n",obj[n] .name);
      return;
    }
    if( obj[n] .location == room && strcmp(noun, obj[n] .name) != 0){
      printf(" you tried to pick it up, but forgot what you tried to do. \n");
    }
  }
  
}

//Allows player to remove items out of their inventory.
void print_drop(struct room *room){
  //printf("you are in %s dropping %s", room->name, noun);
  for(int n = 0; obj[n] .name != NULL; ++n){
    //printf("object name %s", obj[n].name);
    if( obj[n] .location == NULL && strcmp(noun, obj[n] .name) == 0){
      obj[n].location = room;
      printf("\n you dropped %s. \n",obj[n] .name);
    }
  }
}

/*void print_use(struct room *room){
  if(room == &map[1] && strcmp(noun, "Master Key") == 0){
    if(obj[1].location == NULL){
      printf(" you turned the Key in the door and pushed it open. \n");
        return (1);
    }
    if(obj[1].location != NULL){
      printf(" you you cannot use what you do not have. \n");
        return;
    }
   }
}*/




//Shows player list of commands they can use.
void print_help(){
printf("\nlist of commands");
printf("\n -go      (allows player to move)");
printf("\n -des     (shows the description and examines of the room)");
printf("\n -inv     (shows items in players inventory)");
printf("\n -look    (shows player which room they are in and the directions they can go)");
printf("\n -grab    (allows player to pick up items in the room you are in)");
printf("\n -drop    (allows player to drop items from inventory)");
printf("\n -quit    (exits the game)");
printf("\n -read    (allows player to read certain items)");
printf("\n -use     (allows the player to use certain items)");
printf("\n -map     (shows you the schematics of the area)");
printf("\n -break   (allows player to break certain items)");
printf("\n -dig     (allows player to dig certain things)");
printf("\n -examine (allows player to fully look at different objects)(Everything is not to scale)");
printf("\n -help    (you really need to know what this means?)");
printf("\n -save    (Saves the game and all the items you have)");
printf("\n -load    (Loads previously saved game)\n");

  
// More to come
}

//Shows the room they are in and the direction they can go.
void print_display(struct room *room){
if(room == &map[0]){

printf("\n\n * - #          N\n |        \n #          W   0   E\n  \n                S");
}
else if(room == &map[1]){
printf("\n\n # - * - #      N\n     |    \n     #      W   0   E\n          \n                S"); 
}
else if(room == &map[2]){
printf("\n\n     # - *      N\n          \n            W   0   E\n          \n                S"); 
}
else if(room == &map[3]){
printf("\n\n #              N\n |        \n-* - #      W   0   E\n |        \n #              S"); 
}
else if(room == &map[4]){
printf("\n\n     #          N\n     |    \n # - * - #  W   0   E\n     |    \n     #          S"); 
}
else if(room == &map[5]){
printf("\n\n                N\n          \n     # - *  W   0   E\n         |\n         #      S"); 
}
else if(room == &map[6]){
printf("\n\n                N\n          \n #          W   0   E\n |        \n *              S"); 
}
else if(room == &map[7]){
printf("\n\n                N\n          \n     #      W   0   E\n     |    \n     * - #      S"); 
}
 else if(room == &map[8]){
printf("\n\n                N\n          \n         #  W   0   E\n         |\n     # - *      S"); 
} 
 else if(room == &map[9] || room == &map[10] || room == &map[11] || room == &map[12] || room == &map[13] || room == &map[14] || room == &map[15]){
   printf("\n\n     ?          ?\n     |    \n ? - ? - ?  ?   0   ?\n     |    \n     ?          ?");
 }
}

//Shows map of entire playable area other then the mystery place.
void print_map(struct room *room){
if(room == &map[0]){
  printf("████████████████▔████████████████\n█         ██         ██         █\n█         ██         ██         █\n█    *    ▒▒         ▒▒         █\n█         ██         ██         █ \n█         ██         ██         █\n█████▒██████████▒████████████████\n█████▒██████████▒████████████████\n█         ██         ██         █\n█         ██         ██         █\n?         ▒▒         ▒▒         █\n█         ██         ██         █ \n█         ██         ██         █\n█████▒██████████▒██████████▒█████\n█████▒██████████▒██████████▒█████\n█         ██         ██         █\n█         ██         ██         █\n█         ██         ▒▒         █\n█         ██         ██         █ \n█         ██         ██         █ \n█████████████████████████████████\n");//&map[0]

}
if(room == &map[1]){
  printf("████████████████▔████████████████\n█         ██         ██         █\n█         ██         ██         █\n█         ▒▒    *    ▒▒         █\n█         ██         ██         █ \n█         ██         ██         █\n█████▒██████████▒████████████████\n█████▒██████████▒████████████████\n█         ██         ██         █\n█         ██         ██         █\n?         ▒▒         ▒▒         █\n█         ██         ██         █ \n█         ██         ██         █\n█████▒██████████▒██████████▒█████\n█████▒██████████▒██████████▒█████\n█         ██         ██         █\n█         ██         ██         █\n█         ██         ▒▒         █\n█         ██         ██         █ \n█         ██         ██         █ \n█████████████████████████████████\n");//&map[1]
}
if(room == &map[2]){
  printf("████████████████▔████████████████\n█         ██         ██         █\n█         ██         ██         █\n█         ▒▒         ▒▒    *    █\n█         ██         ██         █ \n█         ██         ██         █\n█████▒██████████▒████████████████\n█████▒██████████▒████████████████\n█         ██         ██         █\n█         ██         ██         █\n?         ▒▒         ▒▒         █\n█         ██         ██         █ \n█         ██         ██         █\n█████▒██████████▒██████████▒█████\n█████▒██████████▒██████████▒█████\n█         ██         ██         █\n█         ██         ██         █\n█         ██         ▒▒         █\n█         ██         ██         █ \n█         ██         ██         █ \n█████████████████████████████████\n");//&map[2]
}
  if(room == &map[3]){
  printf("████████████████▔████████████████\n█         ██         ██         █\n█         ██         ██         █\n█         ▒▒         ▒▒         █\n█         ██         ██         █ \n█         ██         ██         █\n█████▒██████████▒████████████████\n█████▒██████████▒████████████████\n█         ██         ██         █\n█         ██         ██         █\n?    *    ▒▒         ▒▒         █\n█         ██         ██         █ \n█         ██         ██         █\n█████▒██████████▒██████████▒█████\n█████▒██████████▒██████████▒█████\n█         ██         ██         █\n█         ██         ██         █\n█         ██         ▒▒         █\n█         ██         ██         █ \n█         ██         ██         █ \n█████████████████████████████████\n");//&map[3]
}
  if(room == &map[4]){
  printf("████████████████▔████████████████\n█         ██         ██         █\n█         ██         ██         █\n█         ▒▒         ▒▒         █\n█         ██         ██         █ \n█         ██         ██         █\n█████▒██████████▒████████████████\n█████▒██████████▒████████████████\n█         ██         ██         █\n█         ██         ██         █\n?         ▒▒    *    ▒▒         █\n█         ██         ██         █ \n█         ██         ██         █\n█████▒██████████▒██████████▒█████\n█████▒██████████▒██████████▒█████\n█         ██         ██         █\n█         ██         ██         █\n█         ██         ▒▒         █\n█         ██         ██         █ \n█         ██         ██         █ \n█████████████████████████████████\n");//&map[4]
}
  if(room == &map[5]){
  printf("████████████████▔████████████████\n█         ██         ██         █\n█         ██         ██         █\n█         ▒▒         ▒▒         █\n█         ██         ██         █ \n█         ██         ██         █\n█████▒██████████▒████████████████\n█████▒██████████▒████████████████\n█         ██         ██         █\n█         ██         ██         █\n?         ▒▒         ▒▒    *    █\n█         ██         ██         █ \n█         ██         ██         █\n█████▒██████████▒██████████▒█████\n█████▒██████████▒██████████▒█████\n█         ██         ██         █\n█         ██         ██         █\n█         ██         ▒▒         █\n█         ██         ██         █ \n█         ██         ██         █ \n█████████████████████████████████\n");//&map[5]
}
  if(room == &map[6]){
  printf("████████████████▔████████████████\n█         ██         ██         █\n█         ██         ██         █\n█         ▒▒         ▒▒         █\n█         ██         ██         █ \n█         ██         ██         █\n█████▒██████████▒████████████████\n█████▒██████████▒████████████████\n█         ██         ██         █\n█         ██         ██         █\n?         ▒▒         ▒▒         █\n█         ██         ██         █ \n█         ██         ██         █\n█████▒██████████▒██████████▒█████\n█████▒██████████▒██████████▒█████\n█         ██         ██         █\n█         ██         ██         █\n█    *    ██         ▒▒         █\n█         ██         ██         █ \n█         ██         ██         █ \n█████████████████████████████████\n");//&map[6]
}
  if(room == &map[7]){
  printf("████████████████▔████████████████\n█         ██         ██         █\n█         ██         ██         █\n█         ▒▒         ▒▒         █\n█         ██         ██         █ \n█         ██         ██         █\n█████▒██████████▒████████████████\n█████▒██████████▒████████████████\n█         ██         ██         █\n█         ██         ██         █\n?         ▒▒         ▒▒         █\n█         ██         ██         █ \n█         ██         ██         █\n█████▒██████████▒██████████▒█████\n█████▒██████████▒██████████▒█████\n█         ██         ██         █\n█         ██         ██         █\n█         ██    *    ▒▒         █\n█         ██         ██         █ \n█         ██         ██         █ \n█████████████████████████████████\n");//&map[7]
}
  if(room == &map[8]){
  printf("████████████████▔████████████████\n█         ██         ██         █\n█         ██         ██         █\n█         ▒▒         ▒▒         █\n█         ██         ██         █ \n█         ██         ██         █\n█████▒██████████▒████████████████\n█████▒██████████▒████████████████\n█         ██         ██         █\n█         ██         ██         █\n?         ▒▒         ▒▒         █\n█         ██         ██         █ \n█         ██         ██         █\n█████▒██████████▒██████████▒█████\n█████▒██████████▒██████████▒█████\n█         ██         ██         █\n█         ██         ██         █\n█         ██         ▒▒    *    █\n█         ██         ██         █ \n█         ██         ██         █ \n█████████████████████████████████\n");//&map[8]
}
  if(room == &map[9] || room == &map[10] || room == &map[11] || room == &map[12]|| room == &map[13] || room == &map[14]){
  printf("???????????\n?▒▒▒▒▒▒▒▒▒?\n?▒▒▒░░░▒▒▒?\n?▒▒▒▒?▒▒▒▒?\n?▒▒▒▒▒▒▒▒▒? \n?▒▒▒▒▒▒▒▒▒? \n???????????\n");//&map[9]
}
}


//Examine Function lets player see at least a little of what an object look like and have the feel like they have it.
void print_examine(struct room *room){
 if(obj[11].location == &map[7] && strcmp(noun, "Window") == 0){
   printf("      ▄▟█▔▔█▙▄\n     ▟▛    ░ ▜▙\n    ▟▛▒  ▞▚ ░ ▜▙\n   ▟▛ ░ ▟▃▃▙ ░░▜▙\n   █ ▒ ▟▃ ▓▃▙   █\n   █  ▞▒ ▚▞ ░▚░▒█\n   █▒ ▔▔▔▔▔▔▔▔ ░█\n   █▃▃▃▃▃▃▃▃▃▃▃▃█\n   █▃▃▃▙▃▚▃▃▞▃▚▃█\n   █░▞▚▞  ▚░  ▚▞█\n  ████████████████");
 }

  if(obj[12].location == &map[1] && strcmp(noun, "Door") == 0){
    printf(" ▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃ \n ▏ ░     ▒   ▓     ░▕ \n ▏ ▓   ▟██████▙   ▒ ▕ \n ▏▒░░  █▓▓▓▓▓▓█  ▒  ▕ \n █  ▒  █▓▓▓▓▓▓█ ░   ▕ \n █  ░  ▜██████▛  ░  ▕ \n ▏ ░▓░░    ░    ▒   ▕ \n ▏ ▒  ░  ░  ░  ▓  ░ ▕ \n ▏ ░  ░   ▒   ░     ▕ \n ▏░  ░▒   ▒     ▐▀░ ▕ \n ▏   ░     ░      ░ ▕ \n ▏ ░  ▓     ░ ▗▜▚ ░ ▕ \n ▏░    ░     ░   ░  ▕ \n █   ░░ ░▓   ░   ▒  ▕ \n █  ▒     ░   ░▓░ ▓ ▕ \n ▏ ░     ▒ ░   ░    ▕ \n ▏░ ▒   ░   ▒   ░   ▕ \n ▏  ░   ░    ░   ░  ▕ \n ▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔");//Door
  }
  if(obj[3].location == &map[15] && obj[3].name != NULL && strcmp(noun, "Sword") == 0){
printf("     ▐▌\n");
printf("     ▐▌\n");
printf("   ▗▜██▛▖\n");
printf("    |  | \n");
printf("    |  |\n");
printf("    |  |\n");
printf("    |  |\n");
printf("    ████\n");
printf("   ██▘▝██\n");
printf("  ██▘▗▖▝██\n");
printf(" ██▘ ▝▘ ▝██\n");
printf("▔▔▔▔▔▔▔▔▔▔▔▔\n\n");  
  }
  if(obj[3].location != &map[15] && obj[3].name != NULL && strcmp(noun, "Sword") == 0){
printf("     /∖ \n");
printf("    /  ∖  \n");
printf("    |  | \n");
printf("    |  |\n");
printf("    |  |\n");
printf("    |  |\n");
printf("    |  |\n");
printf("    |  |\n");
printf("    |  |\n");
printf("    |  |\n");
printf("   ▝▟██▙▘\n");
printf("     ▐▌\n");
printf("     ▐▌\n");
  return;
  }
  if(obj[3].location == NULL && strcmp(noun, "Sword") == 0){
printf("     /∖ \n");
printf("    /  ∖  \n");
printf("    |  | \n");
printf("    |  |\n");
printf("    |  |\n");
printf("    |  |\n");
printf("    |  |\n");
printf("    |  |\n");
printf("    |  |\n");
printf("    |  |\n");
printf("   ▝▟██▙▘\n");
printf("     ▐▌\n");
printf("     ▐▌\n");
  }
  if((obj[0].location == NULL || obj[0].location == player_location) && strcmp(noun, "Step Ladder") == 0){
    printf("     ▕▀███▄\n");
printf(" ▕▀███▄ ▀███▄\n");
printf(" ▕  ▀███▄ ▀███▄\n");
printf("  	  ▀███▄▕▀███▄\n");
printf("       ▕▀███    ▕\n");
printf("       ▕	    ▕\n");
  }
  if((obj[1].location == NULL || obj[1].location == player_location) && strcmp(noun, "Master Key") == 0){
  printf("▛▀▀▜\n");
  printf("▜▄▄▛\n");
  printf(" ▐▌\n");
  printf(" ▄▌\n");
  printf("▄▄▌\n");
  }
  if((obj[2].location == NULL || obj[2].location == player_location) && strcmp(noun, "Shovel") == 0){
  printf(" ▛▀▀▀▜ \n");
  printf(" ▌   ▐ \n");
  printf(" ▜▄▄▄▛ \n");
  printf("   ▓   \n");
  printf("   █   \n");
  printf("   █   \n");
  printf("   █   \n");
  printf("   █   \n");
  printf("▒▒▒▓▒▒▒\n");
  printf("███▓███\n");
  printf("▀█████▀\n");
  printf("  ▀█▀\n");
  }
  if((obj[4].location == NULL || obj[4].location == player_location) && strcmp(noun, "Candle") == 0){
  printf("   ▞▖\n");
  printf("  ▞ ▚\n");
  printf("  ▚▛▞\n");
  printf("   ▒\n");
  printf("  ▓▓▓\n");
  printf("  ▓▓▓\n");
  printf("  ▓▓▓\n");
  printf("  ▓▓▓\n");
  printf("  ▓▓▓  ▁\n");
  printf("  ▓▓▓ ▐ ▌\n");
  printf("▝▄███▄▘▔\n");
  }
  if((obj[5].location == NULL || obj[5].location == player_location) && strcmp(noun, "Short Stick") == 0){
  printf("  █▗ \n");
  printf("▗ █▞\n");
  printf(" ▚█\n");
  printf("  █ ▞\n");
  printf("  █▞\n");
  printf("  █\n");
  }
  if((obj[6].location == NULL || obj[6].location == player_location) && strcmp(noun, "Sign") == 0){
    printf("▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂\n▏ ~~~~~~~~~~~ ▕\n▏ ~~~~~~~~~~~ ▕\n▏ ~~~~~~~~~~~ ▕\n▏ ~~~~~~~~~~~ ▕\n▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔\n");
  }
  if((obj[7].location == NULL || obj[7].location == player_location) && strcmp(noun, "Gravestone") == 0){
    printf("       ▂▂▂▂▂▂\n       ▏    ▕\n       ▏    ▕\n  ▕▔▔▔▔~~~~~~▔▔▔▔▏\n  ▕    ~~~~~~    ▏\n  ▕▂▂▂▂~~~~~~▂▂▂▂▏\n       ▏    ▕\n       ▏    ▕\n       ▏    ▕\n       ▏    ▕\n       ▏    ▕\n       ▔▔▔▔▔▔\n");
  }
  if(obj[8].location == &map[6] && strcmp(noun, "Crate") == 0){
  printf("           ▄▄        \n");
  printf("       ▄▄▀▀▒▒▀▀▄▄              \n");
  printf("   ▄▄▀▀▒▒      ▒▒▀█▄        \n");
  printf("  ██▄▒▒      ▒▒▄▄▀▀█      \n");
  printf("  █▓▓▀▀▄▄▒▒▄▄▀▀  ▒▒█       \n");
  printf("  █▓▓▕▔▕▓▀█▒▒  ░ ▒▒█ \n");
  printf("  █▓▓▕ ▕▓▓█▒▒  ░ ▒▒█\n");
  printf("  █▓▓▕▁▕▓▓█▒▒    ▄▄▀\n");
  printf("   ▀▀▄▄▔▓▓█▒▒▄▄▀▀ \n");
  printf("       ▀▀▄█▀▀    \n");
  }
  if(obj[8].location == &map[16] && strcmp(noun, "Crate") == 0){
    printf("             ▕\n");
  printf("  ▏▄▄     ▏▄▄█▀▀▄▄▄ ▕\n");  
  printf("  █▓▓▕▁▕▓▓█▒     ▄▄▀\n");
  printf("   ▀▀▄▄▔▓▓█▒▒▄▄▀▀ \n");
  printf("       ▀▀▄█▀▀    \n");
  }
  if(obj[9].location == &map[8] && strcmp(noun, "Dirt") == 0){
    printf(" Hmmmmmmm, this Dirt looks like Dirt.\n");
  }
  if((player_location == &map[9] || player_location == &map[10] ||player_location == &map[11] ||player_location == &map[12] ||player_location == &map[13] ||player_location == &map[14])&& strcmp(noun, "Carvings") == 0){
  printf(" ▄▄▄   ~~~\n");
  printf("▐   ▌  ~~~ \n");
  printf(" ▀█    ~~~\n");
  printf("  ▄    ~~~\n");
    
  }
  if(obj[13].name != NULL && strcmp(noun, "Map") == 0){
    printf(" You tried to examine the Map but you don't have one.\n");
  }
  if(obj[13].name == NULL && strcmp(noun, "Map") == 0){
    printf(" You went to examine the Map, its very old and frail.\n");
  }
    
}

void print_saveloc(){
FILE *fp;
fp = fopen("save.txt", "w");
fprintf(fp, "Player Location: %ld\n",  player_location-map);
  
fprintf(fp, "Inventory Items:");
  for(int i = 0; obj[i] .name != NULL; ++i){
    {
      fprintf(fp, "\n %ld",
        obj[i].location ? obj[i].location-map:-1);
    }
  }
fclose(fp);
}

void print_return(){
FILE *fp;
  fp = fopen("save.txt", "r");
  int Temp;
  fscanf(fp, "Player Location: %d\n", &Temp);
  player_location = map+Temp;

  fscanf(fp, "Inventory Items: ");
  for(int i = 0; obj[i] .name != NULL; ++i){
    {
      fscanf(fp, "\n %d", &Temp);
        if(Temp == -1){
          obj[i].location = NULL;
        }
      else{
        obj[i].location = map+Temp;
      }
    }
  }
  //int i = 0;
   // if(obj[i].name = -1,  ++i){
      //obj[i].location == NULL;
      
    //}
//13
  
}