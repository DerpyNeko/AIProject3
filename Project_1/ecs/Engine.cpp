#include "Engine.h"
#include "EntityManager.h"

#include "../globalOpenGLStuff.h"
#include "../globalStuff.h"

#include "../cShaderManager.h"
#include "../cDebugRenderer.h"
#include "../cLightHelper.h"

#include <glm/gtc/type_ptr.hpp>

#include "../BMPImage.h"
#include "../ResourceManager.h"

#include "FSMSystem.h"
#include "MapInfo.h"

#include "IdleState.h"
#include "SearchState.h"
#include "GatherState.h"
#include "ReturnState.h"

#include "Graph.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

cDebugRenderer* g_pDebugRendererACTUAL = NULL;
iDebugRenderer* g_pDebugRenderer = NULL;

cShaderManager* pShaderManager = NULL;
cVAOMeshManager* g_pVAOMeshManager = NULL;
cBasicTextureManager* g_pTextureManager = NULL;

cCamera* g_pCamera = NULL;

std::vector<sLight*> g_Lights;
cLightHelper* pLightHelper;

GLFWwindow* window;

GLuint program;
GLint matView_location;
GLint matProj_location;
GLint eyeLocation_location;

bool g_bDrawDebugLightSpheres = false;
void DrawDebugLightSpheres(cLightHelper* pLightHelper, sLight* light, Entity* pDebugSphere, glm::mat4 matBall, GLuint program, glm::vec4 oldDiffuse, glm::vec3 oldScale);
char GetColourCharacter(unsigned char r, unsigned char g, unsigned char b);
void swap(std::vector<std::vector<char>>& vec, unsigned long height, unsigned long width);
void CreateGraph(std::vector<std::vector<char>> bmpVec);
void DrawMaze(std::vector<std::vector<char>> bmpVec);

void Update(void);
void Draw(void);

BehaviourManager gBehaviourManager;
ResourceManager	 gResourceManager;

int gNumResources = 0;
int homeNode = 0;
int startNode = 0;
std::vector<int> resources;
std::vector<Node*> dijkstraPathNodes;
Graph* graph;

std::vector<System*> gSystems;
double startTime;

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

/**
 * Initialize
 * Acts as constructor for the Engine
 * @return - engine status code
 */
int Engine::Initialize(void)
{
	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	window = glfwCreateWindow(900, 900, "Project 3", NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(window, key_callback);

	// Mouse callbacks
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorEnterCallback(window, cursor_enter_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);

	// Create the shader manager...
	pShaderManager = new cShaderManager();
	pShaderManager->setBasePath("assets/shaders/");

	cShaderManager::cShader vertexShader;
	cShaderManager::cShader fragmentShader;

	vertexShader.fileName = "vertex01.glsl";
	vertexShader.shaderType = cShaderManager::cShader::VERTEX_SHADER;

	fragmentShader.fileName = "fragment01.glsl";
	fragmentShader.shaderType = cShaderManager::cShader::FRAGMENT_SHADER;

	if (pShaderManager->createProgramFromFile("BasicUberShader", vertexShader, fragmentShader))
	{
		std::cout << "Compiled shaders OK." << std::endl;
	}
	else
	{
		std::cout << "OH NO! Compile error" << std::endl;
		std::cout << pShaderManager->getLastError() << std::endl;
	}

	// Load the uniform location values (some of them, anyway)
	cShaderManager::cShaderProgram* pSP = ::pShaderManager->pGetShaderProgramFromFriendlyName("BasicUberShader");
	pSP->LoadUniformLocation("texture00");
	pSP->LoadUniformLocation("texture01");
	pSP->LoadUniformLocation("texture02");
	pSP->LoadUniformLocation("texture03");
	pSP->LoadUniformLocation("texture04");
	pSP->LoadUniformLocation("texture05");
	pSP->LoadUniformLocation("texture06");
	pSP->LoadUniformLocation("texture07");
	pSP->LoadUniformLocation("texBlendWeights[0]");
	pSP->LoadUniformLocation("texBlendWeights[1]");

	program = pShaderManager->getIDFromFriendlyName("BasicUberShader");

	::g_pVAOMeshManager = new cVAOMeshManager();
	::g_pTextureManager = new cBasicTextureManager();

	// Loading the uniform variables here (rather than the inner draw loop)
	//GLint objectColour_UniLoc = glGetUniformLocation(program, "objectColour");

	//GLint matModel_location = glGetUniformLocation(program, "matModel");
	matView_location = glGetUniformLocation(program, "matView");
	matProj_location = glGetUniformLocation(program, "matProj");
	eyeLocation_location = glGetUniformLocation(program, "eyeLocation");

	// Note that this point is to the +interface+ but we're creating the actual object
	::g_pDebugRendererACTUAL = new cDebugRenderer();
	::g_pDebugRenderer = (iDebugRenderer*)::g_pDebugRendererACTUAL;

	if (!::g_pDebugRendererACTUAL->initialize())
	{
		std::cout << "Warning: couldn't init the debug renderer." << std::endl;
		std::cout << "\t" << ::g_pDebugRendererACTUAL->getLastError() << std::endl;
	}
	else
	{
		std::cout << "Debug renderer is OK" << std::endl;
	}

	::g_pCamera = new cCamera();

	CreateLights(program);
	pLightHelper = new cLightHelper();

	LoadModelTypes(::g_pVAOMeshManager, program);
	LoadModelsIntoScene();

	srand((unsigned int)(time(NULL)));

	return 0;
}

/**
* Destroy
* Acts as destructor for the Engine
* @return - engine status code
*/
int Engine::Destroy(void)
{
	unsigned int numSystems = (unsigned int)(gSystems.size());
	for (unsigned int i = 0; i < numSystems; i++)
	{
		delete gSystems[i];
	}
	gSystems.clear();

	delete pShaderManager;
	delete ::g_pVAOMeshManager;
	delete ::g_pTextureManager;

	delete ::g_pDebugRenderer;
	delete ::g_pCamera;

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);

	return 0;
}

/**
 * AddSystem
 * Adds a system to the engine
 */
int Engine::AddSystem(System* system)
{
	gSystems.push_back(system);

	return 0;
}

/**
* Run
* The game loop
* @return - engine status code
*/
int Engine::Run(void)
{
	// Get the current time to start with
	startTime = glfwGetTime();

	BMPImage* bmp = new BMPImage("assets/resourceMap.bmp");

	char* data = bmp->GetData();
	unsigned long imageWidth = bmp->GetImageWidth();
	unsigned long imageHeight = bmp->GetImageHeight();

	std::vector<std::vector<char>> bmpVec(imageHeight, std::vector<char>(imageWidth));

	int index = 0;
	unsigned short r, g, b;
	for (unsigned long x = 0; x < imageWidth; x++) {
		for (unsigned long y = 0; y < imageHeight; y++) {
			bmpVec[x][y] = GetColourCharacter(data[index++], data[index++], data[index++]);
		}
	}

	swap(bmpVec, imageHeight, imageWidth);

	for (std::vector<char> v : bmpVec)
	{
		for (char c : v)
		{
			printf("%c", c);
		}

		printf("\n");
	}

	CreateGraph(bmpVec);

	FSMSystem fsmSystem;

	FSMState* stateIdle = new IdleState();
	FSMState* stateSearch = new SearchState();
	FSMState* stateGather = new GatherState();
	FSMState* stateReturn = new ReturnState();

	stateIdle->AddTransition(1, stateSearch);
	stateSearch->AddTransition(1, stateGather);
	stateGather->AddTransition(1, stateReturn);
	stateReturn->AddTransition(1, stateSearch);

	fsmSystem.AddState(stateIdle);
	fsmSystem.AddState(stateSearch);
	fsmSystem.AddState(stateGather);
	fsmSystem.AddState(stateReturn);

	fsmSystem.Start();

	// Draw the "scene" (run the program)
	while (!glfwWindowShouldClose(window))
	{
		// Switch to the shader we want
		::pShaderManager->useShaderProgram("BasicUberShader");

		float ratio;
		int width, height;

		glm::mat4x4 matProjection = glm::mat4(1.0f);
		glm::mat4x4	matView = glm::mat4(1.0f);

		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;
		glViewport(0, 0, width, height);

		glEnable(GL_DEPTH);			// Enables the KEEPING of the depth information
		glEnable(GL_DEPTH_TEST);	// When drawing, checked the existing depth
		glEnable(GL_CULL_FACE);		// Discared "back facing" triangles

		// Colour and depth buffers are TWO DIFF THINGS.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// FOV, Aspect ratio, Near clipping plane, Far clipping plane
		matProjection = glm::perspective(0.6f, ratio, 0.1f, 10000.0f);

		matView = glm::lookAt(::g_pCamera->eye, ::g_pCamera->getAtInWorldSpace(), ::g_pCamera->getUpVector());

		glUniform3f(eyeLocation_location, ::g_pCamera->eye.x, ::g_pCamera->eye.y, ::g_pCamera->eye.z);

		glUniformMatrix4fv(matView_location, 1, GL_FALSE, glm::value_ptr(matView));
		glUniformMatrix4fv(matProj_location, 1, GL_FALSE, glm::value_ptr(matProjection));

		{
			// Draw the skybox first 
			Entity* pSkyBox = EntityManager::FindEntity("SkyBoxObject");
			Properties* skyBoxProperties = pSkyBox->GetComponent<Properties>();
			Transform* skyBoxTransform = pSkyBox->GetComponent<Transform>();

			skyBoxTransform->position = g_pCamera->eye;
			skyBoxProperties->bIsVisible = true;
			skyBoxProperties->bIsWireFrame = false;

			// Bind the cube map texture to the cube map in the shader
			GLuint cityTextureUNIT_ID = 30;			// Texture unit go from 0 to 79
			glActiveTexture(cityTextureUNIT_ID + GL_TEXTURE0);	// GL_TEXTURE0 = 33984

			int cubeMapTextureID = ::g_pTextureManager->getTextureIDFromName("CityCubeMap");

			// Cube map is now bound to texture unit 30
			//glBindTexture( GL_TEXTURE_2D, cubeMapTextureID );
			glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTextureID);

			//uniform samplerCube textureSkyBox;
			GLint skyBoxCubeMap_UniLoc = glGetUniformLocation(program, "textureSkyBox");
			glUniform1i(skyBoxCubeMap_UniLoc, cityTextureUNIT_ID);

			//uniform bool useSkyBoxTexture;
			GLint useSkyBoxTexture_UniLoc = glGetUniformLocation(program, "useSkyBoxTexture");
			glUniform1f(useSkyBoxTexture_UniLoc, (float)GL_TRUE);

			glm::mat4 matIdentity = glm::mat4(1.0f);
			DrawObject(pSkyBox, matIdentity, program);

			skyBoxProperties->bIsVisible = false;
			glUniform1f(useSkyBoxTexture_UniLoc, (float)GL_FALSE);
		}

		Entity* pFloor = EntityManager::FindEntity("Floor");
		Transform* floorTransform = pFloor->GetComponent<Transform>();
		floorTransform->position = glm::vec3(255.0f, 45.0f, 0.0f);
		floorTransform->scale = glm::vec3(imageHeight * 15, imageWidth * 15, 1.0f);

		DrawMaze(bmpVec);

		fsmSystem.Update();

		Update();

		//::g_pDebugRendererACTUAL->RenderDebugObjects(matView, matProjection, deltaTime);

		glfwSwapBuffers(window);

		glfwPollEvents();

		ProcessAsyncKeys(window);

		ProcessAsyncMouse(window);

	}//while (!glfwWindowShouldClose(window))

	return 0;
}

/**
* Update
* Code for the update phase
*/
void Update(void)
{
	{
		float dt = 0.3f;
		gBehaviourManager.Update(dt);

		for (System* s : gSystems)
		{
			s->Process(EntityManager::GetEntityList(), dt);
		}
	}

	Draw();
}

void Draw(void)
{
	for (Entity* e : EntityManager::GetEntityList())
	{
		glm::mat4x4 matModel = glm::mat4(1.0f);

		DrawObject(e, matModel, program);
	}
}

void DrawDebugLightSpheres(cLightHelper* pLightHelper, sLight* light, Entity* pDebugSphere, glm::mat4 matBall, GLuint program, glm::vec4 oldDiffuse, glm::vec3 oldScale)
{
	const float ACCURACY_OF_DISTANCE = 0.0001f;
	const float INFINITE_DISTANCE = 10000.0f;

	Properties* sphereProperties = pDebugSphere->GetComponent<Properties>();
	Transform* sphereTransform = pDebugSphere->GetComponent<Transform>();

	float distance90Percent =
		pLightHelper->calcApproxDistFromAtten(0.90f, ACCURACY_OF_DISTANCE,
			INFINITE_DISTANCE,
			light->atten.x,
			light->atten.y,
			light->atten.z);

	sphereTransform->setUniformScale(distance90Percent);			// 90% brightness
	sphereProperties->setDiffuseColour(glm::vec3(1.0f, 1.0f, 0.0f));
	DrawObject(pDebugSphere, matBall, program);

	float distance50Percent =
		pLightHelper->calcApproxDistFromAtten(0.50f, ACCURACY_OF_DISTANCE,
			INFINITE_DISTANCE,
			light->atten.x,
			light->atten.y,
			light->atten.z);
	sphereTransform->setUniformScale(distance50Percent);	// 50% brightness
	sphereProperties->setDiffuseColour(glm::vec3(0.0f, 1.0f, 0.0f));
	DrawObject(pDebugSphere, matBall, program);

	float distance25Percent =
		pLightHelper->calcApproxDistFromAtten(0.25f, ACCURACY_OF_DISTANCE,
			INFINITE_DISTANCE,
			light->atten.x,
			light->atten.y,
			light->atten.z);
	sphereTransform->setUniformScale(distance25Percent);	// 25% brightness
	sphereProperties->setDiffuseColour(glm::vec3(1.0f, 0.0f, 0.0f));
	DrawObject(pDebugSphere, matBall, program);

	float distance1Percent =
		pLightHelper->calcApproxDistFromAtten(0.01f, ACCURACY_OF_DISTANCE,
			INFINITE_DISTANCE,
			light->atten.x,
			light->atten.y,
			light->atten.z);
	sphereTransform->setUniformScale(distance1Percent);	// 1% brightness
	sphereProperties->setDiffuseColour(glm::vec3(0.0f, 0.0f, 1.0f));
	DrawObject(pDebugSphere, matBall, program);

	sphereProperties->materialDiffuse = oldDiffuse;
	sphereTransform->scale = oldScale;
	sphereProperties->bIsVisible = false;
}

char GetColourCharacter(unsigned char r, unsigned char g, unsigned char b)
{
	//std::cout << (int)r << " " << (int)g << " " << (int)b << std::endl;
	if (r == 255 && g == 0 && b == 0)				return 'r';
	if (r == 0 && g == 255 && b == 0)				return 'g';
	if (r == 0 && g == 0 && b == 255)				return 'b';
	if (r == 255 && g == 255 && b == 0)			return 'y';
	if (r == 255 && g == 255 && b == 255)		return 'w';
	if (r == 0 && g == 0 && b == 0)					return '_';
	return 'x';
}

void swap(std::vector<std::vector<char>>& vec, unsigned long height, unsigned long width)
{
	std::vector<std::vector<char>> temp(height, std::vector<char>(width));
	int tx = height - 1;

	for (unsigned long x = 0; x < width; x++) {
		for (unsigned long y = 0; y < height; y++) {
			temp[x][y] = vec[tx][y];
		}
		tx--;
	}

	vec = temp;
}

void CreateGraph(std::vector<std::vector<char>> bmpVec)
{
	Transform* playerTransform = g_player->GetComponent<Transform>();

	graph = new Graph();
	int x = 240;
	int y = 270;
	int i = 0;
	for (unsigned int a = 0; a < bmpVec.size(); a++)
	{
		for (unsigned int b = 0; b < bmpVec[a].size(); b++)
		{
			graph->CreateNode(bmpVec[a][b], i, Vertex(x, y, 0), false);

			if (bmpVec[a][b] == 'r')
			{
				resources.push_back(i);
				gNumResources = resources.size();

			}
			else if (bmpVec[a][b] == 'b')
			{
				homeNode = i;
			}
			else if (bmpVec[a][b] == 'g')
			{
				startNode = i;
				playerTransform->position = glm::vec3(x, y, -5.0f);
			}
			i++;
			x -= 30;
		}
		y -= 30;
		x = 240;
	}
	x = 240;
	y = 270;

	// Current node will add edges to the upper right, right, lower right and below nodes
	for (int i = 0; i < graph->nodes.size(); i++)
	{
		if (graph->nodes[i]->id != '_')
		{
			if (graph->nodes[i + 1]->id != '_')
			{
				graph->AddEdge(graph->nodes[i], graph->nodes[i - 15], 14);
				graph->AddEdge(graph->nodes[i], graph->nodes[i + 1], 10);

				graph->AddEdge(graph->nodes[i - 15], graph->nodes[i], 14);
				graph->AddEdge(graph->nodes[i + 1], graph->nodes[i], 10);
			}
			if (graph->nodes[i + 16]->id != '_')
			{
				graph->AddEdge(graph->nodes[i], graph->nodes[i + 16], 10);
				graph->AddEdge(graph->nodes[i], graph->nodes[i + 17], 14);

				graph->AddEdge(graph->nodes[i + 16], graph->nodes[i], 10);
				graph->AddEdge(graph->nodes[i + 17], graph->nodes[i], 14);
			}
			if (graph->nodes[i + 1]->id == '_')
			{
				if (graph->nodes[i + 16]->id != '_')
				{
					graph->AddEdge(graph->nodes[i], graph->nodes[i + 16], 10);
					graph->AddEdge(graph->nodes[i + 16], graph->nodes[i], 10);
				}
			}
		}
	}
}

void DrawMaze(std::vector<std::vector<char>> bmpVec)
{
	int x = 240;
	int y = 270;

	for (unsigned int a = 0; a < bmpVec.size(); a++)
	{
		for (unsigned int b = 0; b < bmpVec[a].size(); b++)
		{

			glm::mat4 matCube(1.0f);
			Entity* pCube = EntityManager::FindEntity("Cube");
			Transform* cubeTransform = pCube->GetComponent<Transform>();
			cubeTransform->position = glm::vec3(x, y, 0);
			cubeTransform->scale = glm::vec3(13.0f, 13.0f, 7.0f);
			Properties* cubeProperty = pCube->GetComponent<Properties>();

			if (bmpVec[a][b] == 'r')
			{
				cubeProperty->setDiffuseColour(glm::vec3(1.0f, 0.0f, 0.0f));
			}
			else if (bmpVec[a][b] == 'g')
			{
				cubeProperty->setDiffuseColour(glm::vec3(0.0f, 1.0f, 0.0f));
			}
			else if (bmpVec[a][b] == 'b')
			{
				cubeProperty->setDiffuseColour(glm::vec3(0.0f, 0.0f, 1.0f));
			}
			else if (bmpVec[a][b] == 'y')
			{
				cubeProperty->setDiffuseColour(glm::vec3(1.0f, 1.0f, 0.0f));
			}
			if (bmpVec[a][b] == 'w')
			{
				cubeProperty->setDiffuseColour(glm::vec3(1.0f, 1.0f, 1.0f));
			}
			else if (bmpVec[a][b] == '_')
			{
				cubeProperty->setDiffuseColour(glm::vec3(0.0f, 0.0f, 0.0f));
			}

			DrawObject(pCube, matCube, program);

			x -= 30;
		}
		y -= 30;
		x = 240;
	}
	x = 240;
	y = 270;
}
