# Age of Empires: The Lost Lands of Khanelyf
## Description
The game follows the story of a hero in the lands of Khanelyf, he will have to go through challenges to conquer the land. A rpg that reminds us of Warcraft III but giving more value to the strategy and troop movement of Age of Empires and also keeping their art style. The game supports tutorials and the singleplayer mode. Every acomplishment you make will elevate your status and gain you respect from your people allowing you to make your civilitzation stronger.

## Development
You can follow our progress at [@AFK_Games](https://twitter.com/AFK_Games), in our [Facebook page](https://www.facebook.com/AwayFromKeyboardGames/) or our [Trello](https://trello.com/b/SDxGXSx2/project-2) where we manage our workflow. 
Our Github [here](https://github.com/AwayFromKeyboard/Project-2_The-Lost-Lands-of-Khanelyf)

## Our team
* [Daniel Garcia](https://github.com/viriato22)  (Team Lead)

* [Andreu Sacasas](https://github.com/Andreu997) (Manager)

* [Simón Stoyanov](https://github.com/SimonStoyanov) (Code)

* [Sergi Parra](https://github.com/t3m1X) (QA)

* [Eric Abad](https://github.com/cireks) (Design)

## Web Page

Click [Here](https://awayfromkeyboard.github.io/Project-2_The-Lost-Lands-of-Khanelyf/) to enter our website

## Instructions

Controls:

* Use the arrow keys or the mouse to move the camera.

* Use the left mouse button to select your units.

* Keep left click pressed and drag to select multiple units.

* Left click on the minimap to change view.

* Left click on barracks to create new units:

  * Barbarian costs 10 gold.

  * Swordsman costs 30 gold.

* Left click on broken buildings to create new buildings:

  * Barrack costs 90 gold.

  * Basic building costs 30 gold.

  * Blacksmith costs 50 gold.

* Left click on blacksmith building to change the first ability (for 50 gold).

* Use the right mouse button to:

  * Attack enemy units.
 
  * Attack enemy buildings.
 
  * Move the selected troops (can also be used in the minimap).

  * Pick objects.

* On main menu:

  * Left click on "New Game" to start a new game.

  * Left click on "Load Game" to load the last saved game.

  * Put the mouse over "Credits" to see the members of the team.

  * Left click on "Watch Trailer" to see the trailer.

  * Left click on "Exit Game" to exit the game.

  * Left click on the check box of "Fullscreen" to change between fullscreen and windowed mode.

* Use the ESCAPE key to skip the trailer while seeing it or to open the pause menu while playing.

* On pause menu:

  * Left click on Save Game to save the current game.

  * Left click on Load Game to load the last saved game.

  * Left click on Options to open the options menu.

	* Left click on Audio to disable or enable it.

	* Left click on Controls to open controls menu.

		* Left click on one button on this menu and click some button on the keyboard to replace the actual key to the new one.

  * Left click on Quit to exit game.
  
  * Left click on Main Menu to return to the Main Menu.

  * Left click on Resume Game or ESC to exit pause menu.

* F1 to debug mode.

* While on debug:

  * Non walkable tiles + spawn points from all entities are shown.

  * Construct buildings does not require gold.

  * F2 to see UI elements.

  * F3 to see colisions.

  * Z to adquire level up points.

  * B to constuct barrack(then you can create 2 types of units with left click).

  * A to create a barbarian.

  * S to create a swordsman.

  * K to kill an entity.

  * W to create a boss.

  * U, I, O to create a basic building.

  * J to create an object.

  * N to create a broken building.

* Abilities
	* Battlecry: Boosts the damage of the allies inside the radius of action by 5 points for 5 seconds.
	* Undying Will: The Hero is immune to damage for 4 seconds.
	* Whirlwind: The hero deals 40 damage to adjecent enemies.
	* Charge: The hero charges to one selected enemy inside the radius of action and deals 40 + current damage to one enemy.

* Battlecry and Undying Will abilities can be activated by 2 ways:
	* X + left click (default key)
	* Left click to the button of the ability

* Whirlwind ability can be used by:
	* C + left click to use Whirlwind ability (heavy damage to adjecent enemies)
	* Left click to the button of the ability.
	
* Charge ability is activated with:
	* V + left click on enemy at range to use Charge (heavy damage to one enemy)
	* Left click to the button of the ability and then click to an enemy inside the radius of action.

## Goal:

Follow and complete the quests without getting your hero killed and avoid that the enemy waves destroy all the buildings of your empire. 

## Quests:

1st: Kill 5 enemies(there's two hidden on your right and three more in the town going north), you will receive some gold.
2nd: Create a barrack and create troops in it.
3rd: Go east and kill the 4 enemies that protect the towers.
4rd: Go north and kill the 5 enemies that protect the village.
5th: Conquer the village at the north (destroy the two enemy buildings).
6th: Escort the npc from the towers to the village.
7th: Move the provisions from the enemy base to the village at the north (between the two houses).
8th: Kill the boss near the enemy fortress.

## Win Condition

To win the game the player must complete all the quests

# Change Log
## V 0.1
* Troop Movement
* Camera Culling
## V 0.2
* More Units Can Be Creted at Once
* Improved Game Stability
## V 0.2.5
* Barbarians can be created
* Full Animatoin Cycle for Units Implemented (idle, move, attack, death and decompose)
* Units Are Loaded by Reading the Map on Tiled
* Drops and Costs Implemented
## V 0.3
* Enemy Textures Have Changed
* Music Implemented
* FX Implemented
* Map Changed
## V 0.4
* Human Resources Added
* Level Up System implemented
* Quad Tree Implemented
* Decoration on the Map created.
* Barrack Implemented. The player can create a barrack by pressing B.
* Buildings as Entities (just the barrack)
* Iterative pathfinding
## V 0.5
* Quest Manager
* Player Ability (Battle shout)
* Button to kill units (press k)
* Bug Fixing
* Game object has been removed (crash solved)
* Load time improved
## V 0.5.5
* Added life bar.
* Life increases when idle to all units
* Brofiler
* Dialogs
* Fixed bug with the Hero's AI
* Fixed bug with creating units from Barracks
## V 0.5.9
* Added 2 new abilities
> Whirlwind (C button + LClick): 
Deals 40 dmg to nearby enemeies.
Range: 2 tiles
Cooldown: 8 s
> Charge (V button on an enemy + LClick): 
Charges into the selected enemy.
Deals player damage + 40.
Range: 5 tiles in straight lines.
Cooldown: 12 s
* Changed the cooldown of battlecry to 30 s
* An object can be picked and dropped
> When the object is picked, the player slows down and can not attack
* Dialogues are shown for the proper quests
* Solved swordsman errors
## V 0.6
* Expanded the quests to 6.
* Dialogues now match with the quests.
* Changes to the tiled map
* All the "BasicBuilding" entities are loaded from tiled.
* Optimized abilities
* Minor bug fixing
### Added 2 new types of quests:
> - Conquest 
The player will need to destroy the enemy building to change it to ally.
> - Provisions
The player will need to move an item from the main village to the one at the north. (the newly subdued)
### Buildings have a life bar
> - Barracks 
Life = 200
> - BasicBuilding
Life = 150
### Entity deaths crash
> The problem was a simple name collision
### Whirlwind bug
> The ability hit the enemies multiple times. 
To solve it, a loop was modified inside the main Whirlwind function "Whirlwind()".
### Visual ability bug
> The range of the abilities does not show up when they are on cooldown.
## V 0.7
### Minimap implemented
> ### Description:
>Red dots: enemies
Cyan dots: enemy buildings
Blue dot: Hero
Green dots: allies
Yellow dots: ally buildings
### Pause Menu
> ### Includes:
> Load a save file
Save the game into an xml (file)
Resume the game
Quit the game
### Load and Save 
> ### Saves:
> All entities
The current quest and the last dialogue shown
Camera position
Gold
Human Resources
Stats of the hero
### Ordering Sprites
### Solved the two major crashes
> The memory was being allocated by time and within minutes the program crashes
> The game crashed when a unit was destroyed

### Minor changes
* The max life is expanded when the hero upgrades its life points.
* Updated the collision for the ally basic buildings
* Other specific crashes and bugs were solved during the development of other features
## V 0.7.5
* Major Crashes solved
* Towers and citadel as entities
* town buildings vulnerable to enemies and posibility to rebuild them
## V Alpha
### Visual update to the map
### Pause Menu Updated
> ### Includes:
> Options
Audio enable/disable
Key configurations

### Quests Updated
Escort mission and boss mission created

### New ability
Undying will: turns the hero invencible for 3 seconds

### New buildings

### Enemy waves implemented

### Boss added

### New win condition
If the player reaches the last mission and completes it the game will end

### Minor changes
* Other specific crashes and bugs were solved during the development of other features
## V 0.9.1
### Crash solved when choosing the abilities
### Crash solved of the pathfinding
### New lose condition
> If the player has no allied buildings, he will lose the game
### Gold Drops have been modified
> **Swordsmans** gives 20 gold
**Barbarians** gives 10 gold
### Map tiles updated
> The **Bridge** has been replaced by new art
### Created another way to activate abilities
> **Battlecry**, **Undying Will** and **Whirwind** can be activated by clicking their respective buttons.
**Charge** can be activated if the button is pressed and then an enemy inside the rasdius of action is clicked with the Left Click.
### Save and Load are now disabled due to a crash
## V 0.9.5
### Trailer
> When entering the game, the player will be forced to watch the trailer, which can be skippable pressing the **escape key**.
### Main Menu
> Now the game has a main menu, where the player can **load a save file**, play a **new game**, **watch the trailer** again, look at the **credits** and **exit** the game.
### Particles
> New **Particles** were created for the **Battlecry** and **Undying Will** abilities.
### Help
> ### Texts are now visible when the mouse is over some important object of the game
>Texts for the **Player Stats**.
Texts for the **Building Creation**.
Texts for the **Entity Creation**.
Texts for the **Abilities**.
Text for the **Drop Button**.
![image](https://cloud.githubusercontent.com/assets/17119542/26792878/bcd13064-4a13-11e7-878a-b1ccfbbe1b5a.png)
### Balanced Units
> ### Units
>**Enemy Barbarian** now drops 10 gold.
**Enemy Swordsman** now drops 20 gold.
**Ally Barbarian** now costs 10 gold.
**Ally Swordsman** now costs 30 gold.
### New Buildings
>**Blacksmith** can be created after your blacksmith is broken by paying 50 gold.
To get a **Blacksmith** the player will need to destroy the enemy one.
Its functionality is to change the ability of the hero between the **Battlecry** and **Undying Will** for 50 gold.

>![image](https://cloud.githubusercontent.com/assets/17119542/26793240/36f47b52-4a15-11e7-9550-ddc0ebb3e9db.png)
### Major Tile Rework
### New NPC Texture for the Escort Quest
### New Cursors
> ### Now, when the mouse is over some important objects the cursor will change
>When over an **Enemy**, the cursor will be a **Sword**.
When over a **Bulding**, the cursor will be a **Hammer**.
When over the **World**, the cursor will be a **Arrow**.
When the game is **Paused**, the cursor will be a **Hand**
### Bug Fixes
> **Save and Load** were corrected. Now everything useful is saved.
Changed **IA** from the enemy waves.
Fixed an issue with **enemies moving randomly across the map**
When a unit is in the same position than another one, now clicking that spot will not longer **select both of them**, instead **only one will be selected**.
## Gold
### Fixed Bugs
> **Cursor Animation** was reproduced even if there were not any allies selected.
When the player **lost** or **won** the game and wanted to play again by going to the main menu and selecting "new game", the game was in **pause** because the win/lose boolean was not changed.
### Visual Enhancements
> ### Particles
> **Battlecry** and **Undying Will** have their respective particles
The buttons used to **Level Up** the stats of the hero were moved to be more visualty "good"
Now a text will show at the bottom right corner which will tell the player that he has points to spend
