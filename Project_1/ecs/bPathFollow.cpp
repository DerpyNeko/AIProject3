#include "bPathFollow.h"
#include "../globalStuff.h"

bool bHasCollected = false;

PathFollowBehaviour::PathFollowBehaviour(Entity* entity, Path* path)
	: mEntity(entity), mPath(path), mCurrentGoal(0), mPathRadius(1.5f)
{
}

PathFollowBehaviour::~PathFollowBehaviour(void)
{
}

void PathFollowBehaviour::Update(float dt)
{
	// Check if the entity is at the goal position
	Transform* agentTransform = mEntity->GetComponent<Transform>();
	Velocity* agentVelocity = mEntity->GetComponent<Velocity>();
	PathNode pathNode = mPath->pathNodes[mCurrentGoal];

	float dist = glm::distance(agentTransform->position, pathNode.position);

	if (dist < mPathRadius)
	{
		if (mCurrentGoal == mPath->pathNodes.size() - 1)
		{
			agentVelocity->velocity = glm::vec3(0.001f, 0.001f, 0.0f);
			bHasCollected = true;
			return;
		}

		mCurrentGoal++;
	}

	glm::vec3 desiredVelocity = glm::normalize(mPath->pathNodes[mCurrentGoal].position - agentTransform->position);

	float magnitude = glm::length(pathNode.position - agentTransform->position);

	desiredVelocity *= MAXVELOCITY;

	glm::vec3 steer;
	steer.x = desiredVelocity.x - agentVelocity->velocity.x;
	steer.y = desiredVelocity.y - agentVelocity->velocity.y;

	glm::quat orientation = glm::quat(glm::lookAt(agentTransform->position, agentTransform->position + steer, UP));

	orientation = glm::normalize(orientation);

	agentVelocity->velocity.x += steer.x * dt;
	agentVelocity->velocity.y += steer.y * dt;

	if (magnitude > MAXVELOCITY)
	{
		glm::vec3 normalized = agentVelocity->velocity;
		double mag = glm::length(normalized);
		normalized /= mag;

		agentVelocity->velocity = normalized * MAXVELOCITY;
	}
}

std::string PathFollowBehaviour::GetName(void)
{
	return "PathFollowBehaviour";
}
