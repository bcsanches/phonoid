#ifndef AK_CLIENT_H
#define AK_CLIENT_H

#include <PH_BaseClient.h>
#include <PH_GameCamera.h>
#include <PH_GamePlugin.h>
#include <PH_Singleton.h>
#include <PH_SpectatorCamera.h>
#include <PH_WorldManager.h>

namespace Arkanoid
{
	PH_DECLARE_NODE_PTR(Client);

	class Client_c: public Phobos::BaseClient_c, private Phobos::WorldManagerListener_c
	{
		PH_DECLARE_SINGLETON_METHODS(Client);

		public:
			PH_GAME_PLUGIN_CREATE_MODULE_PROC_DECL;

		protected:
			Client_c();

			void OnPrepareToBoot();
			void OnFixedUpdate();
			void OnUpdate();

		private:
			//
			//WorldManagerListener_c
			//

			void OnMapUnloaded();
			void OnMapLoaded();

		private:
			//
			//Attributes
			//
			Phobos::GameCamera_c		clCamera;
			Phobos::SpectatorCamera_c	clSpectatorCamera;

			bool						fMapLoaded;
	};
}

#endif
