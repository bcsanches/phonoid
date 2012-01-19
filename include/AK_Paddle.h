#ifndef AK_PADDLE_H
#define AK_PADDLE_H

#include <OgreVector3.h>

#include <PH_Entity.h>
#include <PH_String.h>

namespace Arkanoid
{
	class Paddle_c: public Phobos::Entity_c
	{
		public:
			static Phobos::EntityPtr_t Create(const Phobos::String_c &name);


		protected:
			Paddle_c(const Phobos::String_c &name);
			~Paddle_c();

			virtual void OnLoadFinished();

		private:
			Ogre::Vector3 v3LeftLimit;
			Ogre::Vector3 v3RightLimit;
	};
}

#endif