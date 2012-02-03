#ifndef AK_CLIENT_H
#define AK_CLIENT_H

#include <PH_BaseClient.h>
#include <PH_ContextCmd.h>
#include <PH_ContextVar.h>
#include <PH_GameCamera.h>
#include <PH_GamePlugin.h>
#include <PH_Singleton.h>
#include <PH_SpectatorCamera.h>
#include <PH_WorldManager.h>

#include "AK_PlayerController.h"

namespace Arkanoid
{
	PH_DECLARE_NODE_PTR(Client);

	class Client_c: public Phobos::BaseClient_c, private Phobos::WorldManagerListener_c
	{
		PH_DECLARE_SINGLETON_METHODS(Client);

		public:
			PH_GAME_PLUGIN_CREATE_MODULE_PROC_DECL;

			enum ControlMode_e
			{
				CM_PLAYER,
				CM_SPECTATOR
			};

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

			//
			//Cmds
			//

			void CmdToggleSpectatorMode(const Phobos::StringVector_t &args, Phobos::Context_c &);

			void VarSpectatorMoveSpeedChanged(const class Phobos::ContextVar_c &var, const Phobos::String_c &oldValue, const Phobos::String_c &newValue);
			void VarSpectatorTurnSpeedChanged(const class Phobos::ContextVar_c &var, const Phobos::String_c &oldValue, const Phobos::String_c &newValue);
			void VarMouseSensitivityChanged(const class Phobos::ContextVar_c &var, const Phobos::String_c &oldValue, const Phobos::String_c &newValue);

			//
			//Others
			//
			void ToggleSpectatorMode();

		private:						
			Phobos::ContextVar_c		varMouseSensitivity;

			Phobos::ContextVar_c		varSpectatorMoveSpeed;
			Phobos::ContextVar_c		varSpectatorTurnSpeed;

			Phobos::ContextCmd_c		cmdToggleSpectatorMode;
			
			//
			//Attributes
			//			
			Phobos::SpectatorCamera_c	clSpectatorCamera;

			PlayerController_c			clPlayerController;

			Phobos::IGameController_c	*pclCurrentController;
			ControlMode_e				eCurrentControlMode;

			bool						fMapLoaded;
	};
}

#endif
