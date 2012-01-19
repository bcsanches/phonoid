#include "AK_Paddle.h"

#include <PH_Dictionary.h>
#include <PH_EntityFactory.h>
#include <PH_EntityKeys.h>
#include <PH_PointEntity.h>
#include <PH_TransformProperty.h>
#include <PH_WorldManager.h>

namespace Arkanoid
{
	PH_FULL_ENTITY_CREATOR("Paddle", Paddle_c);

	Paddle_c::Paddle_c(const Phobos::String_c &name):
		Entity_c(name)
	{
		//empty
	}

	Paddle_c::~Paddle_c()
	{
		//empty
	}

	void Paddle_c::OnLoadFinished()
	{
		const Phobos::Dictionary_c &dictionary = this->GetDictionary();

		const Phobos::String_c &leftLimitEntityName = dictionary.GetValue("leftLimit");
		const Phobos::String_c &rightLimitEntityName = dictionary.GetValue("rightLimit");

		Phobos::WorldManagerPtr_t worldManager = Phobos::WorldManager_c::GetInstance();
		v3LeftLimit = boost::static_pointer_cast<Phobos::PointEntity_c>(worldManager->GetEntityByName(leftLimitEntityName))->GetTransform().GetOrigin();
		v3RightLimit = boost::static_pointer_cast<Phobos::PointEntity_c>(worldManager->GetEntityByName(rightLimitEntityName))->GetTransform().GetOrigin();
	}
}
