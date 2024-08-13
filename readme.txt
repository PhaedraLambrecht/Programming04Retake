Engine and the design choices:

This engine is made for the course Programming4 in DAE.
The engine itself is for a 2D arcade game - this one is mainly foccused on Pengo.



I decided to use include guards instead of #pragma once to ensure that my main issues wouldn't stem from includes, as I've encountered multiple problems with them in the past. 
While including guards aren't a magical fix for every issue, they do provide significant help.
Having a guard that ensures my header content is included only once, takes away a big headache.


The scene is fully responsible for managing the lifetime of objects, to ensure that no object is prematurely deleted. Preventing potential crashes or undefined behaviour. To achieve this, I use a deletion flag, which marks objects for removal. 
The actual deletion takes place after the regular update but before the collision checks.



Link to the github:
https://github.com/PhaedraLambrecht/Programming04Retake



Keybinds main menu:

- Down the alphabet: Arrow down
- Up the alphabet: Arrow up
- Confirm letter: Arrow right

- Start singleplayer: F2
- Start multiplayer: F3

- Mute volume: F5
- Decrease volume: F6
- Increase volume: F7


Keybinds game:

- Skip level: F1

- Mute volume: F5
- Decrease volume: F6
- Increase volume: F7

- Movement: D-pad/WASD
- Shooting bullets: X/x
- Hitting walls: B/z