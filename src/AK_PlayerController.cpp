#include "AK_PlayerController.h"

#include <PH_Console.h>
#include <PH_Entity.h>
#include <PH_EntityKeys.h>
#include <PH_TransformProperty.h>

#include "AK_Paddle.h"

namespace Arkanoid
{
	PlayerController_c::PlayerController_c():
		pclTargetEntity(NULL)
	{		
		clCamera.SetNearClipDistance(0.1f);
		clCamera.EnableViewport(1);
	}

	void PlayerController_c::SetTarget(Phobos::Entity_c *entity)
	{
		pclTargetEntity = entity;	
	}

	void PlayerController_c::FixedUpdate()
	{
		using namespace Phobos;

		PaddleCmdPtr_t cmd = boost::static_pointer_cast<PaddleCmd_c>(clCommandProducer.CreateCmd());
		static_cast<Paddle_c*>(pclTargetEntity)->SetCurrentCmd(cmd);
	}	

	void PlayerController_c::SetCameraTransform(const Phobos::Transform_c &transform)
	{
		clCamera.SetCameraTransform(transform);
	}
}

