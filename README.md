# 2020-COMP3016-Coursework2

# Notice

This repository is a cloned version of the OpenGL project I completed in Jan 2021. Due to the repository I used not being owned by me, I have duplicated the files across to this repository which is visible to the public. All files are the exact same state as in the private repository.

If there are any concerns, please contact me.

# Interactable Forest Scene

This is an scene rendered entirely in OpenGL where you are able to control a forest scene in a few ways.

It features three different types of models - Two types of low poly trees and some small fauna.

Everytime you load the scene the models will be loaded in a slightly different location!


## Features

Everytime you load the scene, the models will be in different locations, using random number generation for the co-ordinates.

You can control the forest scene in your own way:

• Control the amount of trees you want rendered in incremented in sets of 10 between 10 and 50.

• Control the amount of fauna you want rendered in incremented sets of 2 between 2 and 10.

• Choose from 5 atmospheric forest tracks to listen to in the background.

• Have your audio or lighting change as you step through the scene.

• Finally, you can control the ambient lighting yourself using the slider at the bottom of the GUI.

## Screenshots/GIFs

![Image 1](https://i.gyazo.com/9652bf0cb86dcd0bb6243af51f1d65ba.png)

Control the amount of objects rendered
![GIF Test](https://i.gyazo.com/dc431c8c060ccb2682e11f8b20a488d3.gif)

Move around the scene
![GIF 2](https://i.gyazo.com/749e9fc4c4fc9002604d22cd875055e6.gif)




## Known issues

• Due to a problem with shaders, there is only one shader used for every item. I have tried to mitigate this using similar objects.

• The trees look like they have no textures - they are low-poly objects so the shader overtakes the colour. This makes one of the other trees look completely green.

• Due to the random nature of the coordinates, some objects may appear to be on top of each other.

## Differences to my original pitch

I have tried to stick as closely as possible to my original pitch, however due to some constraints, I have had to make some changes to my original pitch. These are listed below.

• I have used two extra libraries compared to my original pitch. These are Assimp for model importing and Irrklang for audio. 

• There is no "distinct" forest scenes. Originally I planned to have a dense forest, and a regular forest, however I have instead opted to render models in a smaller grid fashion. Depending on the randomness, it may be possible to get very similar scenes regardless. Subsequently, lighting is also not defined in each section. I have instead added the randomness element.

• The user is not able to choose a grid to render in, it is a fixed size.

• Sounds are not generated, instead it is a track played in the background. It is possible to have a similar effect to PCG by allowing the track to change everytime you step through the project.

## Acknowledgements

• All of these acknowledgements are also in the "Credits&Acknowledgements.txt" file.

**Code credits:**

• mesh.h, model.h, Shader.h - Code mostly taken from LearnOpenGL (https://learnopengl.com/). I have slightly modified this code so that I am able to translate and scale each individual model. I have commented this in the code where I have edited it, as well as the full credit and links to where I found the code.

**Model credits:**

• Low poly trees used: https://free3d.com/3d-model/low-poly-tree-73217.html

• Rock obj file used: https://www.cgtrader.com/free-3d-models/plant/other/rock-01-98f8a3df-4cba-4bb7-8452-f1c36bc35fa7

• Different texture used to due to scaling issues with the included texture.

• Rock texture used: https://www.deviantart.com/roskvape/art/Rock-Texture-307459462


**Audio credits:**


• Track 1: Klankbeeld forest summer. URL: https://freesound.org/people/klankbeeld/sounds/530415/

• Track 2: Jayhu forest. URL: https://freesound.org/people/JayHu/sounds/506103/

• Track 3: GirlWithSoundRecorder forest. URL: https://freesound.org/people/GirlWithSoundRecorder/sounds/520605/

• Track 4: Erdie forest04. URL: https://freesound.org/people/Erdie/sounds/34410/

• Track 5: Odilonmarcenaro. URL: https://freesound.org/people/odilonmarcenaro/sounds/235939/

• Links to the specific track's creator pages is in the code under both the RandomMusicChange & ForcedMusicChange methods.

**External packages used:**

• Audio - Irrklang: https://www.ambiera.com/irrklang/

• ImGui - https://github.com/ocornut/imgui

• Assimp - https://www.assimp.org/


## How does the user interact with your executable?

It is extremely easy to use my program. To move around the scene, use the WASD keys. If movement is not working, click on the scene and then press WASD.

To control the scene, use the simple GUI on the screen. Here is where you are able to modify the audio by either choosing a track or randomising it, 

## How do you open and control the software you wrote?

To open my program, run "Coursework2.exe" in the "exectuable" folder. The forest scene will then render. The GUI will allow control of how many models are rendered, audio and lighting.

If there are any issues with the executable, it will be shown in the command prompt, or a windows error message will appear if a .dll file is missing (this shouldn't happen!).

To move around the scene, W, A, S & D moves you in the relevant direction of Left, Down, Up and Right respectively.

## How does the program code work?

Before the scene is rendered, the code loads up all of the required ImGUI information including version info and window, and create the window.

Then, both the default shader and the rock shader are loaded as Shader objects, using the relevant vertex and fragment files. 

Afterwards, all of the models are loaded - 50 trees (split as 25 of one type and 25 of the other) and 10 pieces of fauna. As the models are relatively small, it is not too much of a memory cost to load all of the models at once, instead of dynamically loading them each time. 

I also execute a function which forces a track to be chosen (track 256 triggers the default track).

Finally, within the loop, all of the drawing takes place. Depending on how many trees and fauna the user has selected, the models will be drawn using the tree shader. Due to an issue with multiple shaders not working correctly, the rock shader does not currently work, therefore I am restricted to one shader. This is mainly why the planned rocks in my scene are not added.

## How do the classes and functions fit together and who does what?

The classes mesh.h, model.h and shader.h all fit together to form model loading, meanwhile the majority of the rest of the script is in one single .cpp file. 
For moving the camera, a function called moveCameraInput is bound to the event function glfwSetKeyCallback, which is triggered every time the user presses a key. Within that function, if the key pressed is W, A, S or D, it will translate the camera array accordingly.

The GUI is also bound to certain functions which are according to which button is pressed. For example, if the user wants to change track, ForcedTrackChange(int) is triggered, with the int being the track number. For changing the lighting, ControlAmbientLighting is called every frame and the lighting is changed, while a global variable, ambientAnswer is updated from the slider to dynamically change the lighting. Note that if the checkbox is ticked for updating lighting, it is also called when W,A,S,D is called.

Within the function !glfwWindowShouldClose, firstly the function display() is called which draws the camera. Then all of the model's .draw() function is called, which draws the model on the screen. Afterwards, the functions relating to imGui, such as new frame, begin and end are all called to render elements of the GUI to the user.

Finally, at the end of this loop, the ImGui data is rendered, and then the loop ends.
Outside this loop, the window is destroyed and the program is terminated - as the loop is only broken when the user closes the program.

## What makes my program special? (Where did you get the idea from? What did you start with? How did you make yours unique? Did you start with a given project?)

I first looked at the examples that were presented to us that we could use, wanting to ideally combine two. The project started with a default cube made of indices. I then moved towards adding a model, and started with a basic model to see if I could get it to load. 


Eventually I decided to modify the project of generating terrain in a forest scene by including Audio, and at the time, three different levels of lighting. I decided that adding an element of randomness would help to enhance the look and feel of the project - originally this was to be the user was able to choose the size of the forest. I knew that a forest would not be right without audio, so it seemed like the perfect fit to include audio.


To make it more unique, I decided to see if I could make every time loading the program have a completely different outcome - and I managed to do this by randomising the coordinates between two variables, as this would help boost the overall look and feel of the program. I also added the option to modify the lighting level everytime you walked through the forest, to a random level between 10 and 25. This meant that each step felt like going somewhere different in the forest. Having two ways to modify the music also helped - a basic control and changing it through every step. Combined with the lighting it can feel like every step is a movement in a forest scene. 

I started with the default cube, and imported the code to render models from LearnOpenGL. 



## Video link (Unlisted YouTube)

https://www.youtube.com/watch?v=todTlKzvsPM





