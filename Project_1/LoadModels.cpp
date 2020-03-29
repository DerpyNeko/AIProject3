#include "globalOpenGLStuff.h"
#include "globalStuff.h"

#include "ecs/Behaviours.h"

Entity* g_player;
Entity* g_ring;

std::vector<Entity*> g_bullets;

// Loading models was moved into this function
void LoadModelTypes(cVAOMeshManager* pTheVAOMeshManager, GLuint shaderProgramID)
{
	sModelDrawInfo sphereInfo;
	sphereInfo.meshFileName = "Sphere_n_uv.ply";
	pTheVAOMeshManager->LoadModelIntoVAO(sphereInfo, shaderProgramID);

	sModelDrawInfo sphereInvertedNormalsInfo;
	sphereInvertedNormalsInfo.meshFileName = "Sphere_n_uv_INVERTED_NORMALS.ply";
	pTheVAOMeshManager->LoadModelIntoVAO(sphereInvertedNormalsInfo, shaderProgramID);

	sModelDrawInfo Cube;
	Cube.meshFileName = "Cube.ply";
	pTheVAOMeshManager->LoadModelIntoVAO(Cube, shaderProgramID);

	// At this point, mesh in in GPU
	std::cout << "Mesh was loaded OK" << std::endl;

	// Load the cube map
	::g_pTextureManager->SetBasePath("assets/textures/cubemaps");
	std::string errorString;
	if (::g_pTextureManager->CreateCubeTextureFromBMPFiles("CityCubeMap",
		"TropicalSunnyDayLeft2048.bmp", "TropicalSunnyDayRight2048.bmp",		// Alternate these
		"TropicalSunnyDayDown2048.bmp", "TropicalSunnyDayUp2048.bmp", 			// Rotate these 90 degrees
		"TropicalSunnyDayFront2048.bmp", "TropicalSunnyDayBack2048.bmp", true, errorString))
	{
		std::cout << "Loaded the city cube map OK" << std::endl;
	}
	else
	{
		std::cout << "Error: city cube map DIDN't load. On no!" << std::endl;
	}

	return;
}

// Loads the models we are drawing into the vector
void LoadModelsIntoScene()
{
	// ENTITY #0 - Skybox
	{
		Entity* entity = EntityManager::CreateEntity();
		entity->name = "SkyBoxObject";

		Properties* properties = entity->AddComponent<Properties>();
		properties->setDiffuseColour(glm::vec3(1.0f, 1.0f, 0.0f));
		properties->meshName = "Sphere_n_uv_INVERTED_NORMALS.ply";
		properties->bIsVisible = false;

		Transform* transform = entity->AddComponent<Transform>();
		transform->position = glm::vec3(0.0f, 0.0f, 0.0f);
		transform->setUniformScale(5000.0f);
		transform->orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		transform->sphereRadius *= transform->scale.x;
	}

	// ENTITY #1 - The Player
	{
		g_player = EntityManager::CreateEntity();
		g_player->name = "Player";

		Properties* properties1 = g_player->AddComponent<Properties>();
		properties1->setDiffuseColour(glm::vec3(0.0f, 0.0f, 0.0f));
		properties1->bDontLight = true;
		properties1->meshName = "Sphere_n_uv.ply";
		properties1->type = eType::PLAYER;

		Transform* playerTransform = g_player->AddComponent<Transform>();
		playerTransform->position = glm::vec3(0.0f, 0.0f, 0.0f);
		playerTransform->setUniformScale(15.0f);
		playerTransform->orientation = glm::quat(0.0f, 0.0f, 1.0f, 0.0f);
		playerTransform->sphereRadius = playerTransform->scale.x;

		Velocity* v = g_player->AddComponent<Velocity>();
		v->velocity = glm::vec3(0.001, 0.001, 0.0);
	}

	// ENTITY #3-15 - Entities
	for (int i = 0; i < 12; i++)
	{
		Entity* entity = EntityManager::CreateEntity();
		entity->name = "Cube";
		Properties* properties = entity->AddComponent<Properties>();
		properties->setDiffuseColour(glm::vec3(0.0f, 1.0f, 0.0f));
		properties->meshName = "Cube.ply";
		properties->bDontLight = true;
		properties->type = eType::NONE;

		Transform* transform = entity->AddComponent<Transform>();
		transform->position = glm::vec3(0.0f, 0.0f, 0.0f);
		transform->setUniformScale(1.0f);
	}


	// ENTITY #22 - Debug Sphere
	{
		Entity* entity = EntityManager::CreateEntity();
		entity->name = "DebugSphere";
		Properties* properties = entity->AddComponent<Properties>();
		properties->setDiffuseColour(glm::vec3(0.0f, 1.0f, 0.0f));
		properties->meshName = "Sphere_n_uv.ply";
		properties->bIsVisible = false;
		properties->bIsWireFrame = true;
		Transform* transform = entity->AddComponent<Transform>();
		transform->position = glm::vec3(0.0f, 0.0f, 0.0f);
		transform->setUniformScale(0.1f);
	}

	return;
}