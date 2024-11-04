Project implements a shooting game for one player against multiple enemies controlled by computer.

**Gameplay**
***
**Basic rules** \
There are 2 rounds in game which cycles, first one is **moving round** and the second one is **shooting round**. Player movement and
player shooting is handled by keys W,S,A,D. Both shooting round and moving round can be skipped when invalid key is pressed.
There are multiple objects within the game, which player can use in his advantage
- *Static object* - divided into two groups
  - breakable, when destroyed by player it can give one of many upgrades.
  - unbreakable, which in most cases serves as border for a map or as a hiding spot
- *Dynamic objects* - landmines, which gives damage when collision with any unit is detected
***
- Enemy units are programmed to chase the player and if possible they will try to shoot at him.
***
**Game Conclusion** \
Game concludes in *player's victory* when there are no enemy units left, that means that player can die in order to
kill the last unit and still be victorious.\
On the other hand, *player loses* when he is killed and there are still alive enemy units left.
At the end of each game there is an updated leaderboard of all players available. Currently, leaderboard is not being saved when
the game is about to be shut down. However, that might change in future updates.
***
**Weapons**
- In order to win, player can choose between weapons with different behaviours, which he can receive as an upgrade by destroying breakable objects.
***
**Game can be configured from a file.**
- For instance loading game maps, if they do meet specified format, setting enemy spawn point, upgrades drop chances and so on.