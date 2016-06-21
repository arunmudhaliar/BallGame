# BallGame

BallGame
------------------------------------------------------------
- Developed using VS 2013 & 2010 express edition.
- Used OpenGL as graphics accelerator.
- Win32 API for windowing system.
- Used png & zlib for texture loading
- Used a small subset of openGL fixed pipeline engine for loading assets, rendering & scene management.
- Used Euler based integration for physics. (so wont be so smooth)


Files related to the game.
-------------------------------
- ball.h			: (Rigid body)	(Didn't implemeted angular velocity).
- borderWall.h		: (collision detection).
- pathGenerator.h	: (generates and renders point sprites for the path).
- targetTrail.h		: (small trail effect using texture matrix transformation).

- executable.zip	: (Released version of the game. Extract and run the BallGame.exe).