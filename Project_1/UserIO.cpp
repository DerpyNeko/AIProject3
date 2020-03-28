#include "globalOpenGLStuff.h"
#include "globalStuff.h"

#include "ecs/Behaviours.h"

bool isWPressed = false;
bool isSPressed = false;

FlockBehaviour* flockBehave;

int bulletCount = -1;

std::vector<glm::vec3> lastFormationCalled;

void increaseWeight(float& weight1, float& weight2, float& weight3)
{
	weight1 += 0.1f;
	if (weight1 >= 1.0f)
		weight1 = 1.0f;

	float num = 1.0f - weight1;

	weight2 = weight3 = num / 2;
}

void decreaseWeight(float& weight1, float& weight2, float& weight3)
{
	weight1 -= 0.1f;
	if (weight1 <= 0.0f)
		weight1 = 0.0f;

	float num = 1.0f - weight1;

	weight2 = weight3 = num / 2;
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Transform* playerTransform = g_player->GetComponent<Transform>();
	Velocity* playerVelocity = g_player->GetComponent<Velocity>();

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	{
		std::cout << playerTransform->position.x << " " << playerTransform->position.y << std::endl;
	}

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		gBehaviourManager.SetBehaviour(g_player, new FormationBehaviour(g_player, circleFormation));
		lastFormationCalled = circleFormation;
	}

	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		gBehaviourManager.SetBehaviour(g_player, new FormationBehaviour(g_player, vFormation));
		lastFormationCalled = vFormation;
	}

	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
	{
		gBehaviourManager.SetBehaviour(g_player, new FormationBehaviour(g_player, squareFormation));
		lastFormationCalled = squareFormation;
	}

	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
	{
		gBehaviourManager.SetBehaviour(g_player, new FormationBehaviour(g_player, lineFormation));
		lastFormationCalled = lineFormation;
	}

	if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
	{
		gBehaviourManager.SetBehaviour(g_player, new FormationBehaviour(g_player, rowsFormation));
		lastFormationCalled = rowsFormation;
	}

	if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
	{
		gBehaviourManager.RemoveBehaviour(g_player, "FormationBehaviour");
		flockBehave = new FlockBehaviour(g_player, 0.4f, 0.4f, 0.2f);
		gBehaviourManager.SetBehaviour(g_player, flockBehave);
	}

	if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
	{
		gBehaviourManager.RemoveBehaviour(g_player, "FlockBehaviour");
		gBehaviourManager.SetBehaviour(g_player, new FormationBehaviour(g_player, lastFormationCalled));
	}

	if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
	{
		Path* path = new Path();
		for (Entity* e : EntityManager::GetEntityList())
		{
			Properties* p = e->GetComponent<Properties>();
			Transform* t = e->GetComponent<Transform>();
			if (p->type == eType::OTHER)
			{
				path->pathNodes.push_back(PathNode(t->position));
			}
		}

		gBehaviourManager.SetBehaviour(g_player, new PathFollowBehaviour(g_player, path));
	}

	if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
	{
		isReversed = !isReversed;
		std::cout << "Key 9: isReversed: " << isReversed << std::endl;
	}

	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
	{
		//	isStopped = !isStopped;
		gBehaviourManager.RemoveBehaviour(g_player, "FormationBehaviour");
		gBehaviourManager.RemoveBehaviour(g_player, "PathFollowBehaviour");
		gBehaviourManager.RemoveBehaviour(g_player, "FlockBehaviour");
		playerVelocity->velocity = glm::vec3(0, 0, 0);
		for (Entity* e : EntityManager::GetEntityList())
		{
			Properties* p = e->GetComponent<Properties>();
			if (p->type == eType::OTHER)
			{
				p->setDiffuseColour(glm::vec3(0.0f, 0.0f, 1.0f));
			}
		}
	}

	// Flock Weight Controls
	// Increase
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
	{
		if (flockBehave != NULL)
		{
			float cWeight = flockBehave->GetCohesionWeight();
			float sWeight = flockBehave->GetSeparationWeight();
			float aWeight = flockBehave->GetAlignmentWeight();

			increaseWeight(cWeight, sWeight, aWeight);

			flockBehave->SetCohesionWeight(cWeight);
			flockBehave->SetSeparationWeight(sWeight);
			flockBehave->SetAlignmentWeight(aWeight);

			std::cout << "CohesionWeight: " << cWeight << " SeparationWeight: " << sWeight << " AlignmentWeight: " << aWeight << std::endl;
		}

	}

	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
	{
		if (flockBehave != NULL)
		{
			float cWeight = flockBehave->GetCohesionWeight();
			float sWeight = flockBehave->GetSeparationWeight();
			float aWeight = flockBehave->GetAlignmentWeight();

			increaseWeight(sWeight, cWeight, aWeight);

			flockBehave->SetCohesionWeight(cWeight);
			flockBehave->SetSeparationWeight(sWeight);
			flockBehave->SetAlignmentWeight(aWeight);

			std::cout << "CohesionWeight: " << cWeight << " SeparationWeight: " << sWeight << " AlignmentWeight: " << aWeight << std::endl;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
	{
		if (flockBehave != NULL)
		{
			float cWeight = flockBehave->GetCohesionWeight();
			float sWeight = flockBehave->GetSeparationWeight();
			float aWeight = flockBehave->GetAlignmentWeight();

			increaseWeight(aWeight, sWeight, cWeight);

			flockBehave->SetCohesionWeight(cWeight);
			flockBehave->SetSeparationWeight(sWeight);
			flockBehave->SetAlignmentWeight(aWeight);

			std::cout << "CohesionWeight: " << cWeight << " SeparationWeight: " << sWeight << " AlignmentWeight: " << aWeight << std::endl;
		}
	}

	// Decrease
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
	{
		if (flockBehave != NULL)
		{
			float cWeight = flockBehave->GetCohesionWeight();
			float sWeight = flockBehave->GetSeparationWeight();
			float aWeight = flockBehave->GetAlignmentWeight();

			decreaseWeight(cWeight, sWeight, aWeight);

			flockBehave->SetCohesionWeight(cWeight);
			flockBehave->SetSeparationWeight(sWeight);
			flockBehave->SetAlignmentWeight(aWeight);

			std::cout << "CohesionWeight: " << cWeight << " SeparationWeight: " << sWeight << " AlignmentWeight: " << aWeight << std::endl;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
	{
		if (flockBehave != NULL)
		{
			float cWeight = flockBehave->GetCohesionWeight();
			float sWeight = flockBehave->GetSeparationWeight();
			float aWeight = flockBehave->GetAlignmentWeight();

			decreaseWeight(sWeight, cWeight, aWeight);

			flockBehave->SetCohesionWeight(cWeight);
			flockBehave->SetSeparationWeight(sWeight);
			flockBehave->SetAlignmentWeight(aWeight);

			std::cout << "CohesionWeight: " << cWeight << " SeparationWeight: " << sWeight << " AlignmentWeight: " << aWeight << std::endl;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
	{
		if (flockBehave != NULL)
		{
			float cWeight = flockBehave->GetCohesionWeight();
			float sWeight = flockBehave->GetSeparationWeight();
			float aWeight = flockBehave->GetAlignmentWeight();

			decreaseWeight(aWeight, sWeight, cWeight);

			flockBehave->SetCohesionWeight(cWeight);
			flockBehave->SetSeparationWeight(sWeight);
			flockBehave->SetAlignmentWeight(aWeight);

			std::cout << "CohesionWeight: " << cWeight << " SeparationWeight: " << sWeight << " AlignmentWeight: " << aWeight << std::endl;
		}
	}

	return;
}

void ProcessAsyncKeys(GLFWwindow* window)
{
	Transform* playerTransform = g_player->GetComponent<Transform>();
	Velocity* playerVelocity = g_player->GetComponent<Velocity>();
	Velocity* ringVelocity = g_ring->GetComponent<Velocity>();

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)	// "fowards"
	{
		playerVelocity->velocity.y = (glm::vec3(0, 2.0f, 0) * glm::toMat3(playerTransform->orientation)).y;
		playerVelocity->velocity.x = (glm::vec3(0, 2.0f, 0) * glm::toMat3(playerTransform->orientation)).x;
		isWPressed = true;
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE && isWPressed == true)
	{
		playerVelocity->velocity = glm::vec3(0.001, 0.001, 0.0);
		isWPressed = false;
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)	// "backwards"
	{
		playerVelocity->velocity.y = (glm::vec3(0, -2.0f, 0) * glm::toMat3(playerTransform->orientation)).y;
		playerVelocity->velocity.x = (glm::vec3(0, -2.0f, 0) * glm::toMat3(playerTransform->orientation)).x;
		isSPressed = true;
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE && isSPressed == true)
	{
		playerVelocity->velocity = glm::vec3(0.001, 0.001, 0.0);
		isSPressed = false;
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)	// rotate "left"
	{
		playerTransform->adjMeshOrientationEulerAngles(glm::vec3(0, 0, 3), true);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)	// rotate "right"
	{
		playerTransform->adjMeshOrientationEulerAngles(glm::vec3(0, 0, -3), true);
	}

	return;
}

bool g_MouseIsInsideWindow = false;

void cursor_enter_callback(GLFWwindow* window, int entered)
{
	if (entered)
	{
		::g_MouseIsInsideWindow = true;
	}
	else
	{
		::g_MouseIsInsideWindow = false;
	}
	return;
}

// Mouse (cursor) callback
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	return;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	return;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	// A regular mouse wheel returns the y value
	::g_pCamera->setMouseWheelDelta(yoffset);

	return;
}

void ProcessAsyncMouse(GLFWwindow* window)
{
	double x, y;
	glfwGetCursorPos(window, &x, &y);

	::g_pCamera->setMouseXY(x, y);

	const float MOUSE_SENSITIVITY = 0.1f;

	if ((glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) && ::g_MouseIsInsideWindow)
	{
		::g_pCamera->Yaw_LeftRight(-::g_pCamera->getDeltaMouseX() * MOUSE_SENSITIVITY);
		::g_pCamera->Pitch_UpDown(::g_pCamera->getDeltaMouseY() * MOUSE_SENSITIVITY);
	}

	// Adjust the movement speed based on the wheel position
	if (::g_MouseIsInsideWindow)
	{
		::g_pCamera->movementSpeed = ::g_pCamera->getMouseWheel() * MOUSE_SENSITIVITY;
	}

	return;
}
