Computer Graphics Course Mini-Project.
===================================

##Requirements##
  * Box2D
  * Freeglut
  * Libconfig for C++
  * SOIL
  * FTGL

####Requirements installation with apt-get####
```
sudo apt-get install -y libconfig++-dev freeglut3-dev libbox2d-dev libsoil-dev libftgl-dev
```

###Build and Run###
```
make run
```

###Controls###
  a - Add dynamic circle to the world at the cursor's location.

  s - Add dynamic square at the cursor's location.

  d - Add dynamic triangle at the cursor's location.

  p - Screenshot, screenshots are saved in screenshots folder.

  A - Add static(immovable) circle at the cursor's location.

  S - Add static square at the cursor's location.

  D - Add static triangle at the cursor's location.

  b - To Begin simulation

  Space - Add flat static rotating platform at the cursor's location.

  Left-Click + Drag - Draw a static line or pickup object if click is on an object

  \+ - Increase size of the body that's currently being held with the mouse

  \- - Decrease size of the body that's currently being held with the mouse

  r - Rotate body being held by the mouse

  t - Toggle the type of the body being held by the mouse between static and dynamic

  i - To View Instructions

  R - Clear all bodies in the world.

  Q - Quit

  Right-click - Menu to Reset World, Reload Configurations or Exit.



### Configuration
  Configurations like the world's gravity and friction, restitution, etc. of each of the shapes can be found in the config folder.

  The user can change the values in the user.cfg file.

  The default.cfg file values are used as fallback and not to be meddled with.

  There is no need to re-compile the code after changing configurations.

  The configurations can be changed at run-time by reloading the configurations from the right-click menu.  

###Screenshots###
<img src="http://i.imgur.com/fAuCq2r.png" width="400" height="300">
<img src="http://i.imgur.com/WHoPaON.png" width="400" height="300">
<img src="http://i.imgur.com/fug4Rel.png" width="400" height="300">
<img src="http://i.imgur.com/7WRVoPb.png" width="400" height="300">
<img src="http://i.imgur.com/PgoUv8j.png" width="400" height="300">
<img src="http://i.imgur.com/ot88ea7.png" width="400" height="300">
<img src="http://i.imgur.com/u3ceVCR.png" width="400" height="300">
<img src="http://i.imgur.com/NwyMAnj.png" width="400" height="300">
<img src="http://i.imgur.com/SMSEDUT.png" width="400" height="300">
<img src="http://i.imgur.com/OJcztjG.png" width="400" height="300">
