#include "Servicealocator.h"

// Initialize static variables
engine::Null_Sound_System engine::Servicealocator::_default_ss;
engine::SoundSystem* engine::Servicealocator::_ss_instance{ &_default_ss };