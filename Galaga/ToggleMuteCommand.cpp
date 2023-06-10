#include "ToggleMuteCommand.h"
#include "Servicealocator.h"

ToggleMuteCommand::ToggleMuteCommand()
	: Command()
{

}
ToggleMuteCommand::~ToggleMuteCommand()
{

}

void ToggleMuteCommand::Execute(float )
{
	auto& soundSystem = engine::Servicealocator::Get_Sound_System();
	soundSystem.ToggleSoundSystemSound();
}