#include "AK_PaddleCommandProducer.h"

#include <boost/make_shared.hpp>

#include <PH_Console.h>
#include <PH_ContextUtils.h>
#include <PH_Core.h>
#include <PH_CoreTimer.h>
#include <PH_Exception.h>

namespace Arkanoid
{
	PaddleCommandProducer_c::PaddleCommandProducer_c(Phobos::Context_c *context):
		clStrafeButton("-strafeLeft", "+strafeLeft", "=strafeLeft", "-strafeRight", "+strafeRight", "=strafeRight", context),		
		clMouseThumb(PH_MOUSE_THUMB_CMD_NAME, context),			
		fpMouseSensitivity(0.3f),
		cmdNullMouseThumb(PH_MOUSE_THUMB_CMD_NAME),
		fMouseActive(false)
	{
		cmdNullMouseThumb.SetProc(PH_CONTEXT_CMD_BIND(&PaddleCommandProducer_c::CmdNullMouseThumb, this));
		clMouseThumb.Disable();
	}

	Phobos::IPlayerCmdPtr_t PaddleCommandProducer_c::CreateCmd()
	{
		using namespace Phobos;
		
		return boost::make_shared<PaddleCmd_c>(clStrafeButton.GetValue());
	}

	void PaddleCommandProducer_c::Enable()
	{
		using namespace Phobos;

		ConsolePtr_t console = Console_c::GetInstance();

		clStrafeButton.Enable(*console);
		
		ContextVar_c *var = console->TryGetContextVar(PH_PLAYER_CMD_MOUSE_CLIPPED_VAR);
		if(var != NULL)
		{
			if(var->GetBoolean())
				this->EnableMouse();

			var->AddListener(*this);
		}
	}

	void PaddleCommandProducer_c::Disable()
	{
		clStrafeButton.Disable();
		
		using namespace Phobos;

		ContextVar_c *var = Console_c::GetInstance()->TryGetContextVar(PH_PLAYER_CMD_MOUSE_CLIPPED_VAR);
		if (var != NULL)
			var->RemoveListener(*this);

		this->DisableMouse();
		cmdNullMouseThumb.Unlink();
	}

	void PaddleCommandProducer_c::OnVariableValueChanged(const Phobos::ContextVar_c &var)
	{
		if(var.GetBoolean())
			this->EnableMouse();		
		else
			this->DisableMouse();
	}

	void PaddleCommandProducer_c::DisableMouse()
	{
		if(!fMouseActive)
			return;

		clMouseThumb.Disable();
		Phobos::Console_c::GetInstance()->AddContextCmd(cmdNullMouseThumb);

		fMouseActive = false;
	}

	void PaddleCommandProducer_c::EnableMouse()
	{
		if(fMouseActive)
			return;

		cmdNullMouseThumb.Unlink();
		clMouseThumb.Enable(*Phobos::Console_c::GetInstance());

		fMouseActive = true;
	}

	void PaddleCommandProducer_c::SetMouseSensitivity(Phobos::Float_t v)
	{
		if(v <= 0)
			PH_RAISE(Phobos::INVALID_PARAMETER_EXCEPTION, "PaddleCommandProducer_c::SetMouseSensitivity", "Mouse sensitivity must be greater than zero");

		fpMouseSensitivity = v;
	}

	void PaddleCommandProducer_c::CmdNullMouseThumb(const Phobos::StringVector_t &args, Phobos::Context_c &)
	{
		//empty
	}
}
