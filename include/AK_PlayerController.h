#ifndef AK_PLAYER_CONTROLLER_H
#define AK_PLAYER_CONTROLLER_H

#include <PH_EntityFwd.h>
#include <PH_GameCamera.h>
#include <PH_IGameController.h>

#include "AK_PaddleCommandProducer.h"

namespace Arkanoid
{
	class PlayerController_c: public Phobos::IGameController_c
	{
		public:			
			PlayerController_c();

			inline void Enable()
			{
				clCommandProducer.Enable();
				clCamera.Enable();
			}

			inline void Disable()
			{
				clCamera.Disable();
				clCommandProducer.Disable();
			}

			void SetTarget(Phobos::Entity_c *entity);			

			void FixedUpdate();			

			void SetCameraTransform(const Phobos::Transform_c &transform);

		private:								
			PaddleCommandProducer_c clCommandProducer;

			Phobos::Entity_c *pclTargetEntity;			

			Phobos::GameCamera_c clCamera;
	};
}

#endif
