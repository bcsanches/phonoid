#ifndef AK_PADDLE_H
#define AK_PADDLE_H

#include <OgreVector3.h>

#include <PH_Accelerometer.h>
#include <PH_Entity.h>
#include <PH_String.h>
#include <PH_TransformPropertyFwd.h>

#include "AK_PaddleCommandProducer.h"

namespace Arkanoid
{
	class Paddle_c: public Phobos::Entity_c
	{
		public:
			static Phobos::EntityPtr_t Create(const Phobos::String_c &name);

			inline void SetCurrentCmd(PaddleCmdPtr_t cmd);


		protected:
			Paddle_c(const Phobos::String_c &name);
			~Paddle_c();

			virtual void OnLoadFinished();
			virtual void OnFixedUpdate();

		private:
			Ogre::Vector3 v3LeftLimit;
			Ogre::Vector3 v3RightLimit;
			Ogre::Vector3 v3Direction;

			Phobos::Float_t fpTotalDistance;
			Phobos::Float_t fpLeftDistance;

			PaddleCmdPtr_t ipLastCmd;

			Phobos::Accelerometer_c clStrafeAccelerometer;

			Phobos::TransformProperty_c *pprpTransform;
	};

	inline void Paddle_c::SetCurrentCmd(PaddleCmdPtr_t cmd)
	{
		ipLastCmd = cmd;
	}
}

#endif