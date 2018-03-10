# AgarioJoyController(Invented by Nathan Ramanathan) 
![alt text][logo]
![alt text][arduino]
# Materials used
1. A computer (to play the PC game)
2. A wii nunchuck
3. An Arduino Pro micro
4. Some jumper wires
# Why did I make this?
Because I love Agar.io and slither.io

Unfortunately, these PC games were not built for today's gaming controllers.  Many people would just say "Nathan, why not make a joystick move the mouse"  I said, "Well, it's not that easy!".  **Well, I mean...sure you could simply move the mouse with a joystick.  But I don't think you are realizing how hard it is to move a mouse with a joystick in a PC GAME!** :rage:  
For example, the arduino pro micro has no idea where the mouse is located every millisecond.  Because of this, the arduino will move the mouse too far or too close from the player.  **It's basically like steering a car on an ice lake!**

I had to use a different method to play these PC games. 
That's when I found out about [AutoHotKey](https://autohotkey.com/).
This software has the ability to capture the mouse coordinates with extreme precision.
Because of this, I decided to make a circular region **similar to that of a joystick** to where the mouse could travel.
So it's kind of like a moat around a *Castle*.
![alt text][logo1]
# My First Reaction
After I decided to run my project for a **real test**, I had no idea how easy it was to play this game with my software!
You guys should try it too!  I might even work with Matheus Valadares, Agar.io's CEO, to make this a real thing on kickstarter with even more feature.  **One thing is for sure, I am definitely not going to use a wii nunchuck.  I probably create my own type of hardware with the arduino pro micro embedded**

# How to use.
Used to play with Agar.io or other PC games that require the user's mouse to move a player around with the help of a joystick.
*Read complete directions at **instructables.com**

https://www.instructables.com/id/Trap-a-Mouse-Cursor-for-PC-Gaming/

But for those with experience with autohotkey and arduino, these directions below should be enough.
1. Run agario.ahk file through the [sciTe4Autohotkey](https://fincs.ahk4.net/scite4ahk/dl/s4ahk-install.exe) programmer.
2. hit the play button.
3. upload the arduino code to your arduino pro micro(5V version).
4. Wire the wii nunchuck to the arduino.
5. play (*may be necessary to change the center coordinates of the wii nunchuck in the arduino code*)
6.  GO to [agario's](https://www.agar.io) website for testing.  
7. go fullscreen and position your mouse exactly on the center of the player(or cell).
8.  Type the ULTRA COMBO "CTRL+SHIFT+ALT+S"
9.  And that's it. Go and play!  THe wii nunchuck's should map the computer's screen and move according to your movement of the joystick!
10. Oh yeah, to exit type "ESC"
11. To exit out of AUTOHOTKEY program, type "CTRL+ESC"

# Extra features!
1. Game Modes: While holding the nunchuck properly, make a stabbing movement up and then downward to change game modes.  Current two games modes are AGAR.io(default) mode and Slither.io mode. 
2. There is also a mouse mode. First hold the c button on the nunchuck and then perform the *Game Mode from (part a)*.     
3. Scroll Mode 1(In Mouse Mode only): tilt the wii nunchuck sideways to engage scroll mode.  In this mode, you move the joystick to scroll across pages.
4. Scroll Mode 2(In Mouse Mode only):tilt the other way and you can scroll even FASTER!
5. While in Mouse mode, shake nunchuck sideways to disable joystick.
6.While in either Game Mode, shake nunchuck sideways to exit out of a game(Similar to typing "ESC")
---
# Videos

1.[Working Prototype video here.](https://youtu.be/2P4Ze2tlGww)


2.[Update: A Minecraft Mode](https://youtu.be/VvpU20PSfPU)



[logo]: https://raw.githubusercontent.com/nathanRamaNoodles/AgarioJoyController/nathanRamaNoodles-AUTOHOTKEY/AgarioNunchuckWorking.PNG "Agario Wii Nunchuck"
[logo1]: https://i.stack.imgur.com/Q4nm0.png "Comptuer Mouse's path"
[arduino]: https://github.com/nathanRamaNoodles/AgarioJoyController/blob/nathanRamaNoodles-AUTOHOTKEY/20171009_175710.jpg "Arduino Pro Micro"
