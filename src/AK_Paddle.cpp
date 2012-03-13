#include "AK_Paddle.h"

#include <PH_Core.h>
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
		Entity_c(name),
		clStrafeAccelerometer(0.4f)
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

		pprpTransform = &this->GetCustomProperty<Phobos::TransformProperty_c>(PH_ENTITY_PROP_TRANSFORM);

		Phobos::WorldManagerPtr_t worldManager = Phobos::WorldManager_c::GetInstance();
		v3LeftLimit = boost::static_pointer_cast<Phobos::PointEntity_c>(worldManager->GetEntityByName(leftLimitEntityName))->GetTransform().GetOrigin();
		v3RightLimit = boost::static_pointer_cast<Phobos::PointEntity_c>(worldManager->GetEntityByName(rightLimitEntityName))->GetTransform().GetOrigin();
		
		using namespace Ogre;
		Vector3 center = v3RightLimit - v3LeftLimit;

		fpTotalDistance = center.length();
		fpLeftDistance = fpTotalDistance / 2.0f;
		v3Direction = center / fpTotalDistance;

		center /= 2;
		pprpTransform->SetOrigin(center + v3LeftLimit);

		this->EnableFixedUpdate();
	}

	void Paddle_c::OnFixedUpdate()
	{
		using namespace Ogre;

		const Phobos::CoreTimer_s &timer = Phobos::Core_c::GetInstance()->GetGameTimer();

		//FIXME we should always have a command
		if(!ipLastCmd)
		{
			clStrafeAccelerometer.SetButtonState(0);
		}
		else
		{
			clStrafeAccelerometer.SetButtonState(ipLastCmd->GetStrafe());
		}

		clStrafeAccelerometer.Update(timer.fpFrameTime);

		Vector3 movement = v3Direction * clStrafeAccelerometer.GetValue() * timer.fpFrameTime * 3;
		Phobos::Float_t movementLength = movement.length();
		if(movementLength > 0)
		{
			if(clStrafeAccelerometer.GetValue() < 0)
			{
				fpLeftDistance -= movementLength;				
			}
			else
			{
				fpLeftDistance += movementLength;				
			}

			if(fpLeftDistance <= 0)
			{
				fpLeftDistance = 0;
				pprpTransform->SetOrigin(v3LeftLimit);				
			}
			else if(fpLeftDistance >= fpTotalDistance)
			{
				fpLeftDistance = fpTotalDistance;
				pprpTransform->SetOrigin(v3RightLimit);				
			}
			else
			{
				pprpTransform->Translate(movement);
			}
		}				
	}
}
