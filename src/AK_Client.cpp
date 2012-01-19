#include "AK_Client.h"

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
		fMapLoaded(false)
	{
		clSpectatorCamera.Disable();
	}

	void Client_c::OnFixedUpdate()
	{
		if(!fMapLoaded)
			return;

		clSpectatorCamera.FixedUpdate();
	}

	void Client_c::OnUpdate()
	{
		if(!fMapLoaded)
			return;

		clSpectatorCamera.Update();
	}

	void Client_c::OnPrepareToBoot()
	{
		Phobos::WorldManager_c::GetInstance()->AddListener(*this);
	}

	void Client_c::OnMapUnloaded()
	{
		clCamera.Disable();
		clSpectatorCamera.Disable();
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
			transform.RotateY(Ogre::Degree(180));			

			clCamera.SetTransform(transform);			
			//clCamera.EnableViewport(1);

			//Fix camera rotation
			transform.RotateX(Ogre::Degree(transform.GetRotation().getPitch()) * 2);
			clSpectatorCamera.SetTransform(transform);
			
			clSpectatorCamera.Enable();
		}

		fMapLoaded = true;
	}
}