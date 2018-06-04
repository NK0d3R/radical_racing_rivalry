# Radical Racing: Rivalry v1.0.0
Not a ripoff, but a retro *tribute* to the Atari™ classic, Dragster™.

### Synopsis
A retro-themed drag racing game for the Arduboy.

Explosive gameplay!

Immersive graphics!

A stable, cinematic 30 FPS!

28 KILO POWER !

### Requirements
* At least 1x Arduboy
* Arduino 1.8.5 (older versions could work, too)

### Installing the game
Copy the `radical_racing_rivalry` folder to your Sketchbook, open
`radical_racing_rivalry/radical_racing_rivalry.ino` in Arduino and install
it to your Arduboy.

### Gameplay & Controls
#### Game modes
The game has two game modes: Time Attack and Duel.

In Time Attack, your objective is to finish a 2km (1.24 miles) sprint as fast
as you can. Your best time will be saved.

In Duel, you will face a computer opponent. Beat him to the finish line over
the same 2km sprint. If you win the race, your best time lead will be saved.
#### Controls
There are two difficulty levels in the game: Easy & Hard.
##### Easy Controls
In this mode the transmission is semi-automatic (and a bit low-tech, too).

The right action button controls the acceleration.

The left action button is the clutch. While you keep it pressed, you can use
the D-PAD Up/Down to shift to a new gear. The gear selector will be displayed
while you're holding the button. After you shift to a new gear, release
the clutch button and press the right action button to accelerate.
##### Hard Controls
In this mode the transmission is fully manual.

The right action button controls the acceleration.

The left action button is the clutch. While you keep it pressed, you can use
the D-PAD Up/Down/Left/Right to shift to a new gear. The gear selection will be
displayed while you're holding the button. After you shift to a new gear,
release the clutch button and press the right action button to accelerate.

In both control modes, you need to keep holding the clutch button until the
gear shift animation finishes (it's only 4-5 frames), otherwise you'll
break your gearbox (and you'll lose the race).
#### Managing your RPM
If the engine goes over a certain RPM, a warning indicator will be displayed.
The indicator will gradually fill up, if it gets to 100%, you engine will
explode and you'll lose the race. You need to shift to a higher gear or release
the acceleration button in order to allow the engine to cool off (1930s
technology right here!). In higher gears, the engine will overheat much faster.

The current RPM is indicated by a bar on your HUD.

### TODO list
* Support imperial units (metric scum!) - currently, all units are metric.
* Indicator to show race progress (simple minimap).

### Things that were intentionally left out
* Sound: too much kbs of code and data that could be used (and were used)
for nicer things.
