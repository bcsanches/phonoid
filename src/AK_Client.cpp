#include "AK_Client.h"

#include <PH_Console.h>
#include <PH_ContextUtils.h>
#include <PH_EntityKeys.h>
#include <PH_Kernel.h>
#include <PH_TransformProperty.h>
#include <PH_WorldManager.h>

PH_GAME_PLUGIN_ENTRY_POINT

namespace Arkanoid
{
	PH_DEFINE_DEFAULT_SINGLETON(Client);

	PH_GAME_PLUGIN_REGISTER_MODULE(Client);

	PH_GAME_PLUGIN_CREATE_MODULE_PROC_IMPL(Client);

	Client_c::Client_c():
		BaseClient_c("AK_Client"),				
		varMouseSensitivity("dvMouseSensitivity", "0.1"),
		varSpectatorMoveSpeed("dvSpectatorMoveSpeed", "2.0"),
		varSpectatorTurnSpeed("dvSpectatorTurnSpeed", "45.0"),
		cmdToggleSpectatorMode("toggleSpectatorMode"),
		pclCurrentController(NULL),
		fMapLoaded(false)
	{						
		cmdToggleSpectatorMode.SetProc(PH_CONTEXT_CMD_BIND(&Client_c::CmdToggleSpectatorMode, this));

		varSpectatorMoveSpeed.SetCallback(PH_CONTEXT_VAR_BIND(&Client_c::VarSpectatorMoveSpeedChanged, this));
		varSpectatorTurnSpeed.SetCallback(PH_CONTEXT_VAR_BIND(&Client_c::VarSpectatorTurnSpeedChanged, this));
		varMouseSensitivity.SetCallback(PH_CONTEXT_VAR_BIND(&Client_c::VarMouseSensitivityChanged, this));		

		clSpectatorCamera.Disable();
	}

	void Client_c::OnFixedUpdate()
	{
		if(!fMapLoaded)
			return;

		if(pclCurrentController)
			pclCurrentController->FixedUpdate();
	}

	void Client_c::OnUpdate()
	{
		if(!fMapLoaded)
			return;

		if(pclCurrentController)
			pclCurrentController->Update();
	}

	void Client_c::OnPrepareToBoot()
	{
		BaseClient_c::OnPrepareToBoot();

		this->RegisterToggleMouseCursorClipCmd();
		
		using namespace Phobos;

		WorldManager_c::GetInstance()->AddListener(*this);

		ConsolePtr_t console = Console_c::GetInstance();
				
		console->AddContextCmd(cmdToggleSpectatorMode);

		console->AddContextVar(varMouseSensitivity);
		console->AddContextVar(varSpectatorMoveSpeed);
		console->AddContextVar(varSpectatorTurnSpeed);	
	}

	void Client_c::OnMapUnloaded()
	{
		if(pclCurrentController)
			pclCurrentController->Disable();

		fMapLoaded = false;
	}

	void Client_c::OnMapLoaded()
	{
		using namespace Phobos;

		WorldManagerPtr_t worldManager = WorldManager_c::GetInstance();

		EntityPtr_t camera = boost::static_pointer_cast<Entity_c>(worldManager->TryGetEntityByType("GameCamera"));
		if(!camera)
		{
			Kernel_c::GetInstance().LogMessage("[CmdLoadMap] World does not contais GameCamera entity");
		}
		else
		{
			Transform_c transform = camera->GetCustomProperty<Phobos::TransformProperty_c>(PH_ENTITY_PROP_TRANSFORM).GetTransform();
			clPlayerController.SetCameraTransform(transform);

			transform.RotateY(Ogre::Degree(180));			

			//Fix camera rotation
			transform.RotateX(Ogre::Degree(transform.GetRotation().getPitch()) * 2);			
			clSpectatorCamera.SetTransform(transform);

			EntityPtr_t paddle = boost::static_pointer_cast<Entity_c>(worldManager->TryGetEntityByType("Paddle"));
			if(paddle)
			{
				clPlayerController.SetTarget(paddle.get());

				eCurrentControlMode = CM_PLAYER;
				pclCurrentController = &clPlayerController;
				clPlayerController.Enable();
			}
			else
			{			
				clSpectatorCamera.Enable();
				pclCurrentController = &clSpectatorCamera;
				eCurrentControlMode = CM_SPECTATOR;
			}

		}

		fMapLoaded = true;
	}

	void Client_c::ToggleSpectatorMode()
	{
		if(eCurrentControlMode == CM_PLAYER)
		{			
			clPlayerController.Disable();

			clSpectatorCamera.Enable();

			pclCurrentController = &clSpectatorCamera;

			eCurrentControlMode = CM_SPECTATOR;
		}
		else
		{			
			clSpectatorCamera.Disable();

			clPlayerController.Enable();

			pclCurrentController = &clPlayerController;

			eCurrentControlMode = CM_PLAYER;
		}
	}

	void Client_c::CmdToggleSpectatorMode(const Phobos::StringVector_t &args, Phobos::Context_c &)
	{
		this->ToggleSpectatorMode();
	}

	void Client_c::VarSpectatorMoveSpeedChanged(const Phobos::ContextVar_c &var, const Phobos::String_c &oldValue, const Phobos::String_c &newValue)
	{
		clSpectatorCamera.SetMoveSpeed(Phobos::StringToFloat(newValue));
	}

	void Client_c::VarSpectatorTurnSpeedChanged(const Phobos::ContextVar_c &var, const Phobos::String_c &oldValue, const Phobos::String_c &newValue)
	{
		clSpectatorCamera.SetTurnSpeed(Phobos::StringToFloat(newValue));
	}

	void Client_c::VarMouseSensitivityChanged(const Phobos::ContextVar_c &var, const Phobos::String_c &oldValue, const Phobos::String_c &newValue)
	{
		clSpectatorCamera.SetMouseSensitivity(Phobos::StringToFloat(newValue));
	}
}