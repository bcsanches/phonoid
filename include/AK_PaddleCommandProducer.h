#ifndef AK_PADDLE_COMMAND_PRODUCER_H
#define AK_PADDLE_COMMAND_PRODUCER_H

#include <PH_AxisButton.h>
#include <PH_Thumb.h>
#include <PH_Context.h>
#include <PH_ContextVar.h>
#include <PH_IPlayerCommandProducer.h>
#include <PH_Types.h>

namespace Arkanoid
{
	class PaddleCmd_c;
	typedef boost::shared_ptr<PaddleCmd_c> PaddleCmdPtr_t;

	class PaddleCmd_c: public Phobos::IPlayerCmd_c
	{
		public:
			inline PaddleCmd_c(Phobos::Float_t strafe)
			{							
				fpStrafe = strafe;
			}		
			
			inline Phobos::Float_t GetStrafe() const;

		private:			
			Phobos::Float_t fpStrafe;
	};

	inline Phobos::Float_t PaddleCmd_c::GetStrafe() const
	{
		return fpStrafe;
	}

	#define PH_MOUSE_THUMB_CMD_NAME "mouseThumb"

	class PaddleCommandProducer_c: public Phobos::IPlayerCommandProducer_c, Phobos::ContextVarListener_c
	{
		public:
			PaddleCommandProducer_c(Phobos::Context_c *context = NULL);

			virtual Phobos::IPlayerCmdPtr_t CreateCmd();

			virtual void Enable();
			virtual void Disable();

			virtual void EnableMouse();
			virtual void DisableMouse();
			
			void SetMouseSensitivity(Phobos::Float_t v);

		private:
			void OnVariableValueChanged(const Phobos::ContextVar_c &var);

			void CmdNullMouseThumb(const Phobos::StringVector_t &args, Phobos::Context_c &);			

		private:			
			Phobos::AxisButton_c	clStrafeButton;			
			Phobos::Thumb_c			clMouseThumb;
			Phobos::ContextCmd_c	cmdNullMouseThumb;
					
			Phobos::Float_t			fpMouseSensitivity;

			bool			fMouseActive;
	};
}

#endif