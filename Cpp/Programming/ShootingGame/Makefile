CXX=g++
CXXFlags= -std=c++20 -Wall -pedantic
CXXDebugFlags=-Wall -pedantic -g -fsanitize=address -Wextra -O2


compile: main.o person.o player.o computer.o menu.o gameloop.o gameplayMap.o coordinates.o configurate.o configurateStats.o loot.o unbreakableObject.o breakableObject.o emptySpace.o movingObject.o  weapon.o defaultGun.o overpoweredGun.o blastgun.o
	$(CXX) $(CXXFlags) main.o person.o player.o computer.o menu.o gameloop.o gameplayMap.o configurate.o configurateStats.o loot.o coordinates.o unbreakableObject.o breakableObject.o emptySpace.o movingObject.o weapon.o defaultGun.o overpoweredGun.o blastgun.o -o shootingGame

run: compile 
	./shootingGame
doc:
	doxygen Doxyfile

all: compile doc

clean:
	rm -rf *.o shootingGame doc
#--------------------------------------------------------------------------------------------------------------------------------#
# $< takes first file in line after colon (:)
main.o: src/main.cpp src/menu.h
	$(CXX) $(CXXFlags) -c $<
# $< takes first file in line after colon (:)
menu.o: src/menu.cpp src/menu.h src/gameloop.h
	$(CXX) $(CXXFlags) -c $< 
# $< takes first file in line after colon (:)
gameloop.o: src/gameloop.cpp  src/gameloop.h src/gameplayMap.h src/person.h src/computer.h src/player.h src/weapon.h src/defaultGun.h src/defaultGun.h src/overpoweredGun.h src/blastgun.h src/configurateStats.h
	$(CXX) $(CXXFlags) -c $< 
# $< takes first file in line after colon (:)
gameplayMap.o: src/gameplayMap.cpp src/gameplayMap.h src/coordinates.h src/object.h src/unbreakableObject.h src/movingObject.h src/emptySpace.h src/breakableObject.h src/computer.h src/person.h src/configurate.h src/player.h src/weapon.h src/defaultGun.h src/overpoweredGun.h src/blastgun.h src/configurateStats.h src/loot.h
	$(CXX) $(CXXFlags) -c $< 
#Player
# $< takes first file in line after colon (:)
player.o: src/player.cpp src/player.h src/coordinates.h src/gameplayMap.h src/weapon.h src/defaultGun.h src/overpoweredGun.h src/blastgun.h
	$(CXX) $(CXXFlags) -c $<
# $< takes first file in line after colon (:)
computer.o: src/computer.cpp src/computer.h src/coordinates.h src/weapon.h src/defaultGun.h src/overpoweredGun.h src/blastgun.h
	$(CXX) $(CXXFlags) -c $<
# $< takes first file in line after colon (:)
person.o: src/person.cpp src/person.h src/coordinates.h src/weapon.h src/defaultGun.h src/overpoweredGun.h src/blastgun.h
	$(CXX) $(CXXFlags) -c $< 
#Weapons
# $< takes first file in line after colon (:)
weapon.o: src/weapon.cpp src/weapon.h
	$(CXX) $(CXXFlags) -c $<
# $< takes first file in line after colon (:)
defaultGun.o: src/defaultGun.cpp src/defaultGun.h src/weapon.h
	$(CXX) $(CXXFlags) -c $<
# $< takes first file in line after colon (:)
overpoweredGun.o: src/overpoweredGun.cpp src/overpoweredGun.h
	$(CXX) $(CXXFlags) -c $<
# $< takes first file in line after colon (:)
blastgun.o: src/blastgun.cpp src/blastgun.h
	$(CXX) $(CXXFlags) -c $< 
#Configurate data
# $< takes first file in line after colon (:)
configurate.o: src/configurate.cpp src/configurate.h src/coordinates.h src/object.h src/unbreakableObject.h src/breakableObject.h src/emptySpace.h src/movingObject.h src/computer.h src/person.h
	$(CXX) $(CXXFlags) -c $<
# $< takes first file in line after colon (:)
configurateStats.o: src/configurateStats.cpp src/configurateStats.h src/weapon.h src/defaultGun.h src/overpoweredGun.h src/blastgun.h
	$(CXX) $(CXXFlags) -c $< 
#Gameplay map objects
# $< takes first file in line after colon (:)
loot.o: src/loot.cpp src/loot.h
	$(CXX) $(CXXFlags) -c $<
# $< takes first file in line after colon (:)
emptySpace.o: src/emptySpace.cpp src/emptySpace.h
	$(CXX) $(CXXFlags) -c $<
# $< takes first file in line after colon (:)
movingObject.o: src/movingObject.cpp src/movingObject.h
	$(CXX) $(CXXFlags) -c $<
# $< takes first file in line after colon (:)
unbreakableObject.o: src/unbreakableObject.cpp src/unbreakableObject.h src/object.h
	$(CXX) $(CXXFlags) -c $<
# $< takes first file in line after colon (:)
breakableObject.o: src/breakableObject.cpp src/breakableObject.h src/object.h
	$(CXX) $(CXXFlags) -c $<
# $< takes first file in line after colon (:)
object: src/object.h src/weapon.h src/defaultGun.h 
	$(CXX) $(CXXFlags) -c $< 
#coordinates
# $< takes first file in line after colon (:)
coordinates.o: src/coordinates.cpp src/coordinates.h
	$(CXX) $(CXXFlags) -c $< 
#--------------------------------------------------------------------------------------------------------------------------------#