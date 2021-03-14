# untitled_3d

A project I started working around Feb. 2020 that I hoped would help me learn C++ in a fun way. I don't expect to actually build a game based on this engine, its just something
fun to work on and learn more about computer graphics tricks / geometric algorithms. Beats using hackerrank or sites like that to practice data structures
and algorithms, at least in my opinion.

## List of resources I used:

### Architecture / Design:
- https://github.com/iZastic/OpenGL_3D_Game_Tutorial - Originally started from this, deviated heavily after implementing ECS.
- https://www.youtube.com/user/ThinMatrix - This guy has a great tutorial series on everything about OpenGL and building games. It's all in Java though.
- https://refactoring.guru/ - General software design patterns.
- http://gameprogrammingpatterns.com/ - Game specific programming patterns, links to lots of other resources.
- https://austinmorlan.com/posts/entity_component_system/ - REALLY helpful article that helped me completely understand how to implement an Entity-Component System.

### Rendering:
- https://learnopengl.com/ - Really good tutorial series on the OpenGL API as well as a primer to general computer graphics techniques.
- http://www.opengl-tutorial.org/ - Good tutorials on OpenGL. Used some shader code to implement billboards.

### Physics:
- Game Physics by Ian Millington - Refresher on Newtonian Mechanics. Provides a good baseline for a physics system. Still not implemented because I need collisions to work first.

### Collision Detection:
- https://www.pbrt.org/ - Learned the basic theory of bounding volume hierarchies here.
- https://box2d.org/files/ErinCatto_DynamicBVH_GDC2019.pdf - Theory for dynamic bounding volume hierarchies. You need this if you want to use a BVH for the broadphase collision
stage.
- https://github.com/bulletphysics/bullet3 - Looking at how they implemented a dynamic BVH really helped.
- Real Time Collision Detection by Chester Ericson - Used some tricks from here to make rotating AABB's faster, learned about GJK + Dobkin-Kirkpatrick hierarchies here.
- Collision Detection in Interactive 3D Environments by Gino Van Bergen - Helped me understand how to traverse a Dobkin-Kirkpatrick hierarchy.

Stole models + textures from https://github.com/iZastic/OpenGL_3D_Game_Tutorial, who stole them from ThinMatrix. Planning to remove them in the future and use my own amateur 3D models.

## TODO
✔️ Entity-Component System

✔️ Broadphase Collision Detection

❌ Narrowphase Collision Detection

❌ Collision resolution

✔️ Basic rendering

❌ Lighting, shadows, LOD, fancy shaders etc...

✔️ Infinite terrain generation

❌ Interesting infinite terrain generation + entity generation

❌ AI

❌ Chunk system 

❌ Save system

❌ Options menu (Have to change code to change resolution)

✔️ Gravity

❌ ... more to come as the project evolves.

