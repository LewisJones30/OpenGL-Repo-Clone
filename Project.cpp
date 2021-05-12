//////////////////////////////////////////////////////////////////////////////
//
//  Triangles.cpp
//
//////////////////////////////////////////////////////////////////////////////

#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GLFW/glfw3.h"
#include "LoadShaders.h"
#include <glm/glm.hpp> //includes GLM
#include "glm/ext/vector_float3.hpp"
#include "glm/fwd.hpp"
#include <glm/ext/matrix_transform.hpp> // GLM: translate, rotate
#include <glm/ext/matrix_clip_space.hpp> // GLM: perspective and ortho 
#include <imgui.h>
#include <ctime>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_win32.h"
#include <glm/gtc/type_ptr.hpp> // GLM: access to the value_ptr
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <map>
#include <filesystem>
#include <iostream>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Project.h"
#include "Model.h"
#include "Shader.h"
#include <irrKlang.h>
// to use this example you will need to download the header files for GLM put them into a folder which you will reference in
// properties -> VC++ Directories -> libraries

enum VAO_IDs { Triangles, Indices, Colours, Tex, NumVAOs = 36 };
enum Buffer_IDs { ArrayBuffer, NumBuffers = 4 };
enum Attrib_IDs { vPosition = 0, cPosition = 1, tPosition = 2 };

GLuint  VAOs[NumVAOs];
GLuint  Buffers[NumBuffers];
GLuint texture1;
GLuint program;

glm::vec3 camerastorage;
const GLuint  NumVertices = 36; 
int NumberOfTreesToSpawn = 10;
int numberofFaunatoSpawn = 2;
irrklang::ISoundEngine* AudioPlayback = irrklang::createIrrKlangDevice();
irrklang::ISoundSource* Track1 = AudioPlayback->addSoundSourceFromFile("audio/530415__klankbeeld__forest-summer-roond-020-200619-0186.wav");
bool trackChange = false;
bool lightingChange = false;
float ambientAnswer = 20;
using namespace std;

//----------------------------------------------------------------------------
//
// init
//
#define BUFFER_OFFSET(a) ((void*)(a))
void
init(void)
{



	ShaderInfo  shaders[] =
	{
		{ GL_VERTEX_SHADER, "media/triangles.vert" },
		{ GL_FRAGMENT_SHADER, "media/triangles.frag" },
		{ GL_NONE, NULL }
	};
	program = LoadShaders(shaders);
	glUseProgram(program);
	glGenVertexArrays(NumVAOs, VAOs);


	//glm::mat4 view = glm::mat4(1.0f);
	//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));
	camerastorage = glm::vec3(0.0f, 0.0f, -35.0f);


	//Initialise ambient lighting

	ControlAmbientLighting(15);

}


void makeObject(int VAOnumber)
{
	glBindVertexArray(VAOs[VAOnumber]);
	GLfloat vertices[][3] = {
		{0.5f,  0.5f, -0.5f},  //0 top right
		{0.5f, -0.5f, -0.5f},  //1 bottom right
		{-0.5f, -0.5f, -0.5f}, //2 bottom left
		{-0.5f,  0.5f, -0.5f},  //3 top left

		{0.5f,  0.5f, 0.5f},  //4 top right
		{0.5f, -0.5f, 0.5f},  //5 bottom right
		{-0.5f, -0.5f, 0.5f}, //6 bottom left
		{-0.5f,  0.5f, 0.5f}  //7 top left 
	};
	GLuint indices[][3] = {  // note that we start from 0!
		{0, 3, 1},  // first Triangle front
		{3, 2, 1},   // second Triangle

		{4, 7, 0 },
		{7, 3, 0 },

		{1, 2, 5 },
		{2, 6, 5 },

		{5, 4, 0 },
		{0, 1, 5 },

		{2, 3, 7 },
		{7, 6, 2 },

		{4, 5, 7 },  // first Triangle back
		{7, 5, 6 }   // second Triangle
	};

	GLfloat  colours[][4] = {
		{ 1.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f },
		{ 0.0f, 0.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f },
	};
	GLfloat  texture_coords[] = {
		 1.0f, 1.0f,
		 1.0f, 0.0f,
		 0.0f, 0.0f,
		 0.0f, 1.0f,

		 0.0f, 1.0f,
		 0.0f, 0.0f,
		 1.0f, 0.0f,
		 1.0f, 1.0f,



	};



	glGenBuffers(NumBuffers, Buffers);

	glBindBuffer(GL_ARRAY_BUFFER, Buffers[Triangles]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers[Indices]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);



	glVertexAttribPointer(vPosition, 3, GL_FLOAT,
		GL_FALSE, 0, BUFFER_OFFSET(0));

	//Colour Binding
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[Colours]);
	glBufferStorage(GL_ARRAY_BUFFER, sizeof(colours), colours, 0);


	glVertexAttribPointer(cPosition, 4, GL_FLOAT,
		GL_FALSE, 0, BUFFER_OFFSET(0));

	//Texture Binding
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[Tex]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texture_coords), texture_coords, GL_STATIC_DRAW);
	glVertexAttribPointer(tPosition, 2, GL_FLOAT,
		GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(vPosition);
	glEnableVertexAttribArray(cPosition);
	glEnableVertexAttribArray(tPosition);


}

void modelBind(float x, float y, float z)
{
	
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(x, y, z));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, camerastorage);
	glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3, 0.1f, 20.0f);
	glm::mat4 mvp = projection * view * model;
	int mvpLoc = glGetUniformLocation(program, "mvp");
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));
}


void moveCameraInput(GLFWwindow* window, int key, int scancode, int action, int mods) //Listening to the key callback event
{

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	glm::vec3 cameraPos = camerastorage;
	glm::vec3 moveRight = glm::vec3(5.0f, 0.0f, 0.0f);
	glm::vec3 moveUp = glm::vec3(0.0f, 0.0f, 5.0f);
	
	int i = sqrt(36);
	//Each of these if statements will only run once per key press, meaning the user cannot run too far.
	if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		if (camerastorage.z >= 5)
		{

		}
		else
		{
			cameraPos += moveUp;
			camerastorage = cameraPos;

			if (trackChange == true)
			{
				RandomMusicChange();
			}
			if (lightingChange == true)
			{
				srand((unsigned)time(NULL));
				ambientAnswer = rand() % 15 + 10;
			}
		}

	}
	if (key == GLFW_KEY_A && action == GLFW_PRESS)
	{

		if (camerastorage.x >= 0)
		{

		}
		else
		{
			cameraPos += moveRight;
			camerastorage = cameraPos;

			if (trackChange == true)
			{
				RandomMusicChange();
			}
			if (lightingChange == true)
			{
				srand((unsigned)time(NULL));
				ambientAnswer = rand() % 15 + 10;
			}
		}




	}
	if (key == GLFW_KEY_S && action == GLFW_PRESS)
	{
		if (camerastorage.z <= -35)
		{

		}
		else
		{
			cameraPos -= moveUp;
			camerastorage = cameraPos;
			if (trackChange == true)
			{
				RandomMusicChange();
			}
			if (lightingChange == true)
			{
				srand((unsigned)time(NULL));
				ambientAnswer = rand() % 15 + 10;
			}
		}



	}
	if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		if (camerastorage.x <= -30)
		{

		}
		else
		{
			cameraPos -= moveRight;
			camerastorage = cameraPos;
			if (trackChange == true)
			{
				RandomMusicChange();
			}
			if (lightingChange == true)
			{
				srand((unsigned)time(NULL));
				ambientAnswer = rand() % 15 + 10;
			}
		}


		
	}


}

void RandomMusicChange()
{
	srand((unsigned)time(NULL));
	int TrackNumber = rand() % 5; //Between 0 and 4.

	switch (TrackNumber)
	{
	case 0:
	{
		AudioPlayback->stopAllSounds();
		AudioPlayback->play2D("audio/530415__klankbeeld__forest-summer-roond-020-200619-0186.mp3", true); 
		//Audio credit:
		//Taken from freesound.org.
		//Audio creator: klankbeeld.
		//Site: https://freesound.org/people/klankbeeld/
		//All credits also in included .txt file.
		break;
	}
	case 1:
	{
		AudioPlayback->stopAllSounds();
		AudioPlayback->play2D("audio/506103__jayhu__forest.mp3", true);
		//Audio credit:
		//Taken from freesound.org.
		//Audio creator: JayHu.
		//Site: https://freesound.org/people/JayHu/
		//All credits also in included .txt file.
		break;
	}
	case 2:
	{
		AudioPlayback->stopAllSounds();
		AudioPlayback->play2D("audio/520605__girlwithsoundrecorder__forest.mp3", true);
		//Audio credit:
		//Taken from freesound.org.
		//Audio creator: GirlWithSoundRecorder.
		//Site: https://freesound.org/people/GirlWithSoundRecorder/
		//All credits also in included .txt file.
		break;
	}
	case 3:
	{
		AudioPlayback->stopAllSounds();
		AudioPlayback->play2D("audio/34410__erdie__forest04.mp3", true);
		//Audio credit:
		//Taken from freesound.org.
		//Audio creator: Erdie.
		//Site: https://freesound.org/people/Erdie
		//All credits also in included .txt file.
		break;
	}
	case 4:
	{
		AudioPlayback->stopAllSounds();
		AudioPlayback->play2D("audio/235939__odilonmarcenaro__forest-ambiance.mp3", true);
		//Audio credit:
		//Taken from freesound.org.
		//Audio creator: Odilonmarcenaro
		//Site: https://freesound.org/people/odilonmarcenaro
		//All credits also in included .txt file.
		break;
	}
	default:
	{
		AudioPlayback->drop();
		AudioPlayback->play2D("audio/530415__klankbeeld__forest-summer-roond-020-200619-0186.wav", true);
		//Audio credit:
		//Taken from freesound.org.
		//Audio creator: klankbeeld.
		//Site: https://freesound.org/people/klankbeeld/
		//All credits also in included .txt file.
		break;
	}
	}

}

void ForcedTrackChange(int trackNumber)
{
	int TrackNumber = trackNumber;

		switch (TrackNumber)
		{
		case 0:
		{

			AudioPlayback->stopAllSounds();
			AudioPlayback->play2D("audio/530415__klankbeeld__forest-summer-roond-020-200619-0186.mp3", true);
			//Audio credit:
			//Taken from freesound.org.
			//Audio creator: klankbeeld.
			//Site: https://freesound.org/people/klankbeeld/
			//All credits also in included .txt file.
			break;
		}
		case 1:
		{
			AudioPlayback->stopAllSounds();
			AudioPlayback->play2D("audio/506103__jayhu__forest.mp3", true);
			//Audio credit:
			//Taken from freesound.org.
			//Audio creator: JayHu.
			//Site: https://freesound.org/people/JayHu/
			//All credits also in included .txt file.
			break;

		}
		case 2:
		{
			AudioPlayback->stopAllSounds();
			AudioPlayback->play2D("audio/520605__girlwithsoundrecorder__forest.mp3", true);
			//Audio credit:
			//Taken from freesound.org.
			//Audio creator: klankbeeld.
			//Site: https://freesound.org/people/klankbeeld/
			//All credits also in included .txt file.
			break;
		}
		case 3:
		{
			AudioPlayback->stopAllSounds();
			AudioPlayback->play2D("audio/34410__erdie__forest04.mp3", true);
			//Audio credit:
			//Taken from freesound.org.
			//Audio creator: klankbeeld.
			//Site: https://freesound.org/people/klankbeeld/
			//All credits also in included .txt file.
			break;
		}
		case 4:
		{
			AudioPlayback->stopAllSounds();
			AudioPlayback->play2D("audio/235939__odilonmarcenaro__forest-ambiance.mp3", true);
			//Audio credit:
			//Taken from freesound.org.
			//Audio creator: odilonmarcenaro.
			//Site: https://freesound.org/people/odilonmarcenaro
			//All credits also in included .txt file.
			break;
		}
		default:
		{
			AudioPlayback->play2D("audio/530415__klankbeeld__forest-summer-roond-020-200619-0186.mp3", true);
			//Audio credit:
			//Taken from freesound.org.
			//Audio creator: klankbeeld.
			//Site: https://freesound.org/people/klankbeeld/
			//All credits also in included .txt file.
			break;
		}
		}

	}

void ControlAmbientLighting(float lightIntensity)
{
	glm::vec4 ambient = glm::vec4(lightIntensity, lightIntensity, lightIntensity, 1.0f);
	int aloc = glGetUniformLocation(program, "ambient");
	glUniform4fv(aloc, 1, glm::value_ptr(ambient));


}



void loadTexture(GLuint &texture, std::string texturepath)
{
	// load and create a texture 
// -------------------------

// texture 1
// ---------
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	GLint width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data = stbi_load(texturepath.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}

//----------------------------------------------------------------------------
//
// display
//

void
display(void)
{
	static const float black[] = { 0.0f, 0.0f, 0.0f, 0.0f };

	glClearBufferfv(GL_COLOR, 0, black);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// bind textures on corresponding texture units
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);



	// Adding all matrices up to create combined matrix
	int j = sqrt(36);
	//modelBind(0.0f, 0.0f, 0.0f);
	//glBindVertexArray(VAOs[0]);
	//glBindTexture(GL_TEXTURE_2D, texture1);
	//glDrawElements(GL_TRIANGLES, NumVertices, GL_UNSIGNED_INT, 0);
	/*for (int i = 0; i < SizeOfSpawnChosen; i++)
	{
		float k = floor(i / 5);
		modelBind(((i % j) * 5), 0.0f, -((i / j) * 5));
		glBindVertexArray(VAOs[NumVAOs]);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glDrawElements(GL_TRIANGLES, NumVertices, GL_UNSIGNED_INT, 0);
	}*/
	
	// Adding all matrices up to create combined matrix
	//glBindVertexArray(VAOs[1]);
	//glBindTexture(GL_TEXTURE_2D, texture1);
	//modelBind(1.0f, 0.0f, 0.0f);
	//glDrawElements(GL_TRIANGLES, NumVertices, GL_UNSIGNED_INT, 0);

	
}
//----------------------------------------------------------------------------
//
// main
//




int
main(int argc, char** argv)
{
	glfwInit();


	GLFWwindow* window = glfwCreateWindow(800, 600, "Forest Scene", NULL, NULL);
	//* window1 = glfwCreateWindow(800, 600, "ImGui", NULL, NULL);
	glfwMakeContextCurrent(window);
	glewInit();
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	init();
	//ImGUI setup
	bool show_second_window = false;
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(NULL);
	io.Fonts->AddFontDefault();
	glfwSetKeyCallback(window, moveCameraInput);
	srand((unsigned)time(NULL));
	float treeScale = 0.5f;
	float rockScale = 0.4f;
	float yCoordTree = 0.5f;
	float yCoordRock = -1.0f;
	Shader ourShader("media/triangles.vert", "media/triangles.frag");
	Shader rockShader("media/rock.vert", "media/rock.frag");
	Model treemodel1("media/textures/Tree_2.obj", rand() % 30, yCoordRock, rand() % 30, 0.35f);
	Model treemodel2("media/textures/lowpolytree.obj", rand() % -30, yCoordTree, rand() % 30, treeScale);
    Model treemodel3("media/textures/Tree_2.obj", rand() % 30, yCoordRock, rand() % 30, 0.35f);
	Model treemodel4("media/textures/lowpolytree.obj", rand() % -30, yCoordTree, rand() % 30, treeScale);
	Model treemodel5("media/textures/Tree_2.obj", rand() % 30, yCoordRock, rand() % 30, 0.35f);
	Model treemodel6("media/textures/lowpolytree.obj", rand() % -30, yCoordTree, rand() % 30, treeScale);
	Model treemodel7("media/textures/Tree_2.obj", rand() % 30, yCoordRock, rand() % 30, 0.35f);
	Model treemodel8("media/textures/lowpolytree.obj", rand() % -30, yCoordTree, rand() % 30, treeScale);
	Model treemodel9("media/textures/Tree_2.obj", rand() % 30, yCoordRock, rand() % 30, 0.35f);
	Model treemodel10("media/textures/lowpolytree.obj", rand() % -30, yCoordTree, rand() % 30, treeScale);
	Model treemodel11("media/textures/Tree_2.obj", rand() % 30, yCoordRock, rand() % 30, 0.35f);
	Model treemodel12("media/textures/lowpolytree.obj", rand() % -30, yCoordTree, rand() % 30, treeScale);
	Model treemodel13("media/textures/Tree_2.obj", rand() % 30, yCoordRock, rand() % 30, 0.35f);
	Model treemodel14("media/textures/lowpolytree.obj", rand() % -30, yCoordTree, rand() % 30, treeScale);
	Model treemodel15("media/textures/Tree_2.obj", rand() % 30, yCoordRock, rand() % 30, 0.35f);
	Model treemodel16("media/textures/lowpolytree.obj", rand() % -30, yCoordTree, rand() % 30, treeScale);
	Model treemodel17("media/textures/Tree_2.obj", rand() % 30, yCoordRock, rand() % 30, 0.35f);
	Model treemodel18("media/textures/lowpolytree.obj", rand() % -30, yCoordTree, rand() % 30, treeScale);
	Model treemodel19("media/textures/Tree_2.obj", rand() % 30, yCoordRock, rand() % 30, 0.35f);
	Model treemodel20("media/textures/lowpolytree.obj", rand() % -30, yCoordTree, rand() % 30, treeScale);
	Model treemodel21("media/textures/Tree_2.obj", rand() % 30, yCoordRock, rand() % 30, 0.35f);
	Model treemodel22("media/textures/lowpolytree.obj", rand() % -30, yCoordTree, rand() % 30, treeScale);
	Model treemodel23("media/textures/Tree_2.obj", rand() % 30, yCoordRock, rand() % 30, 0.35f);
	Model treemodel24("media/textures/lowpolytree.obj", rand() % -30, yCoordTree, rand() % 30, treeScale);
	Model treemodel25("media/textures/Tree_2.obj", rand() % 30, yCoordRock, rand() % 30, 0.35f);
	Model treemodel26("media/textures/lowpolytree.obj", rand() % -30, yCoordTree, rand() % 30, treeScale);
	Model treemodel27("media/textures/Tree_2.obj", rand() % 30, yCoordRock, rand() % 30, 0.35f);
	Model treemodel28("media/textures/lowpolytree.obj", rand() % -30, yCoordTree, rand() % 30, treeScale);
	Model treemodel29("media/textures/Tree_2.obj", rand() % 30, yCoordRock, rand() % 30, 0.35f);
	Model treemodel30("media/textures/lowpolytree.obj", rand() % -30, yCoordTree, rand() % 30, treeScale);
	Model treemodel31("media/textures/Tree_2.obj", rand() % 30, yCoordRock, rand() % 30, 0.35f);
	Model treemodel32("media/textures/lowpolytree.obj", rand() % -30, yCoordTree, rand() % 30, treeScale);
	Model treemodel33("media/textures/Tree_2.obj", rand() % 30, yCoordRock, rand() % 30, 0.35f);
	Model treemodel34("media/textures/lowpolytree.obj", rand() % -30, yCoordTree, rand() % 30, treeScale);
	Model treemodel35("media/textures/Tree_2.obj", rand() % 30, yCoordRock, rand() % 30, 0.35f);
	Model treemodel36("media/textures/lowpolytree.obj", rand() % -30, yCoordTree, rand() % 30, treeScale);
	Model treemodel37("media/textures/Tree_2.obj", rand() % 30, yCoordRock, rand() % 30, 0.35f);
	Model treemodel38("media/textures/lowpolytree.obj", rand() % -30, yCoordTree, rand() % 30, treeScale);
	Model treemodel39("media/textures/Tree_2.obj", rand() % 30, yCoordRock, rand() % 30, 0.35f);
	Model treemodel40("media/textures/lowpolytree.obj", rand() % -30, yCoordTree, rand() % 30, treeScale);
	Model treemodel41("media/textures/Tree_2.obj", rand() % 30, yCoordRock, rand() % 30, 0.35f);
	Model treemodel42("media/textures/lowpolytree.obj", rand() % -30, yCoordTree, rand() % 30, treeScale);
	Model treemodel43("media/textures/Tree_2.obj", rand() % 30, yCoordRock, rand() % 30, 0.35f);
	Model treemodel44("media/textures/lowpolytree.obj", rand() % -30, yCoordTree, rand() % 30, treeScale);
	Model treemodel45("media/textures/Tree_2.obj", rand() % 30, yCoordRock, rand() % 30, 0.35f);
	Model treemodel46("media/textures/lowpolytree.obj", rand() % -30, yCoordTree, rand() % 30, treeScale);
	Model treemodel47("media/textures/Tree_2.obj", rand() % 30, yCoordRock, rand() % 30, 0.35f);
	Model treemodel48("media/textures/lowpolytree.obj", rand() % -30, yCoordTree, rand() % 30, treeScale);
	Model treemodel49("media/textures/Tree_2.obj", rand() % 30, yCoordRock, rand() % 30, 0.35f);
	Model treemodel50("media/textures/lowpolytree.obj", rand() % -30, yCoordTree, rand() % 30, treeScale);
	Model model5("media/textures/GrassFlat.obj", 0, -1, 0, 2.0f);
	Model fauna1("media/textures/Fauna.obj", rand() % -30, -1, rand() % -30, 0.4f);
	Model fauna2("media/textures/Fauna.obj", rand() % -30, -1, rand() % -30, 0.4f);
	Model fauna3("media/textures/Fauna.obj", rand() % -30, -1, rand() % -30, 0.4f);
	Model fauna4("media/textures/Fauna.obj", rand() % -30, -1, rand() % -30, 0.4f);
	Model fauna5("media/textures/Fauna.obj", rand() % -30, -1, rand() % -30, 0.4f);
	Model fauna6("media/textures/Fauna.obj", rand() % -30, -1, rand() % -30, 0.4f);
	Model fauna7("media/textures/Fauna.obj", rand() % -30, -1, rand() % -30, 0.4f);
	Model fauna8("media/textures/Fauna.obj", rand() % -30, -1, rand() % -30, 0.4f);
	Model fauna9("media/textures/Fauna.obj", rand() % -30, -1, rand() % -30, 0.4f);
	Model fauna10("media/textures/Fauna.obj", rand() % -30, -1, rand() % -30, 0.4f);


	//Model model3("C:\\Users\\Lewis\\Documents\\10450_Rectangular_Grass_Patch_v1_iterations-2.obj", 0, 0, 0, 1);

	ForcedTrackChange(256);

	while (!glfwWindowShouldClose(window))
	{
		// uncomment to draw only wireframe 
		// glPolygonMode(GL_FRONT_AND_BACit K, GL_LINE);
		glEnable(GL_DEPTH_TEST);
		glfwMakeContextCurrent(window);
		modelBind(0.0f, 0.0f, 0.0f);
		display();
		//rockShader.use();
		model5.Draw(rockShader);
		ourShader.use();
		treemodel1.Draw(ourShader);
		treemodel2.Draw(ourShader);
		treemodel3.Draw(ourShader);
		treemodel4.Draw(ourShader);
		treemodel5.Draw(ourShader);
		treemodel6.Draw(ourShader);
		treemodel7.Draw(ourShader);
		treemodel8.Draw(ourShader);
		treemodel9.Draw(ourShader);
		treemodel10.Draw(ourShader);
		if (NumberOfTreesToSpawn >= 20)
		{
			treemodel11.Draw(ourShader);
			treemodel12.Draw(ourShader);
			treemodel13.Draw(ourShader);
			treemodel14.Draw(ourShader);
			treemodel15.Draw(ourShader);
			treemodel16.Draw(ourShader);
			treemodel17.Draw(ourShader);
			treemodel18.Draw(ourShader);
			treemodel19.Draw(ourShader);
			treemodel20.Draw(ourShader);
		}
		if (NumberOfTreesToSpawn >= 30)
		{
			treemodel21.Draw(ourShader);
			treemodel22.Draw(ourShader);
			treemodel23.Draw(ourShader);
			treemodel24.Draw(ourShader);
			treemodel25.Draw(ourShader);
			treemodel26.Draw(ourShader);
			treemodel27.Draw(ourShader);
			treemodel28.Draw(ourShader);
			treemodel29.Draw(ourShader);
			treemodel30.Draw(ourShader);
		}
		if (NumberOfTreesToSpawn >= 40)
		{
			treemodel31.Draw(ourShader);
			treemodel32.Draw(ourShader);
			treemodel33.Draw(ourShader);
			treemodel34.Draw(ourShader);
			treemodel35.Draw(ourShader);
			treemodel36.Draw(ourShader);
			treemodel37.Draw(ourShader);
			treemodel38.Draw(ourShader);
			treemodel39.Draw(ourShader);
			treemodel40.Draw(ourShader);
		}
		if (NumberOfTreesToSpawn == 50)
		{
			treemodel41.Draw(ourShader);
			treemodel42.Draw(ourShader);
			treemodel43.Draw(ourShader);
			treemodel44.Draw(ourShader);
			treemodel45.Draw(ourShader);
			treemodel46.Draw(ourShader);
			treemodel47.Draw(ourShader);
			treemodel48.Draw(ourShader);
			treemodel49.Draw(ourShader);
			treemodel50.Draw(ourShader);
		}

		if (numberofFaunatoSpawn >= 4)
		{
			//rockShader.use();
			fauna3.Draw(ourShader);
			fauna4.Draw(ourShader);
		}
		if (numberofFaunatoSpawn >= 6)
		{
			fauna5.Draw(ourShader);
			fauna6.Draw(ourShader);
		}
		if (numberofFaunatoSpawn >= 8)
		{
			fauna7.Draw(ourShader);
			fauna8.Draw(ourShader);
		}
		if (numberofFaunatoSpawn >= 10)
		{
			fauna9.Draw(ourShader);
			fauna10.Draw(ourShader);
		}
		ourShader.use();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Forest render ");
		ImGui::Text("Please note that objects will most likely clip. This is due to how the randomness for locations work.");
		//ImGui::Text(" Your current coordinates: (%f, %f)", camerastorage.x, camerastorage.z);
		ImGui::Text("Choose how many trees you want to have rendered in this scene. Current: %d", NumberOfTreesToSpawn);
		ImGui::SameLine();
		if (ImGui::Button("10") == true)
		{
			//Code to render 5x5.
			NumberOfTreesToSpawn = 10;
		}
		ImGui::SameLine();
		if (ImGui::Button("20") == true)
		{
			NumberOfTreesToSpawn = 20;
		}
		ImGui::SameLine();
		if (ImGui::Button("30") == true)
		{
			NumberOfTreesToSpawn = 30;
		}
		ImGui::SameLine();
		if (ImGui::Button("40") == true)
		{
			NumberOfTreesToSpawn = 40;
		}
		ImGui::SameLine();
		if (ImGui::Button("50") == true)
		{
			NumberOfTreesToSpawn = 50;
		}
		ImGui::Text("Choose how many small plants you want to have rendered in this scene. Current: %d", numberofFaunatoSpawn);
		ImGui::SameLine();
		if (ImGui::Button("2") == true)
		{
			numberofFaunatoSpawn = 2;
		}
		ImGui::SameLine();
		if (ImGui::Button("4") == true)
		{
			numberofFaunatoSpawn = 4;
		}
		ImGui::SameLine();
		if (ImGui::Button("6") == true)
		{
			numberofFaunatoSpawn = 6;
		}
		ImGui::SameLine();
		if (ImGui::Button("8") == true)
		{
			numberofFaunatoSpawn = 8;
		}
		ImGui::Text("Track options:");
		ImGui::SameLine();
		if (ImGui::Button("Random Track") == true)
		{
			RandomMusicChange();
		}
		ImGui::SameLine();
		if (ImGui::Button("Track 1") == true)
		{
			ForcedTrackChange(0);
		}
		ImGui::SameLine();
		if (ImGui::Button("Track 2") == true)
		{
			ForcedTrackChange(1);
		}
		ImGui::SameLine();
		if (ImGui::Button("Track 3") == true)
		{
			ForcedTrackChange(2);
		}
		ImGui::SameLine();
		if (ImGui::Button("Track 4") == true)
		{
			ForcedTrackChange(3);
		}
		ImGui::SameLine();
		if (ImGui::Button("Track 5") == true)
		{
			ForcedTrackChange(4);
		}
		ImGui::Checkbox("Track changes when you navigate through scene", &trackChange);
		ImGui::Checkbox("Lighting changes when you navigate through scene", &lightingChange);
		ImGui::SliderFloat("Ambient Lighting", &ambientAnswer, 10, 25);
		ControlAmbientLighting(ambientAnswer);
		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
		glfwPollEvents();


	}

	glfwDestroyWindow(window);

	glfwTerminate();
}
