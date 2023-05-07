#include "structs.h"

using namespace engine;
//************************************
// Window information 
// ***********************************
Window::Window(const std::string& title, float width, float height)
	: title { title }
	, width { width }
	, height { height }
{

}

//-----------------------------------------------------------------
// Rectf Constructors
//-----------------------------------------------------------------
Rectf::Rectf()
	:Rectf{ 0.0f, 0.0f, 0.0f, 0.0f }
{
}

Rectf::Rectf(float left, float bottom, float width, float height)
	:left{ left }
	, bottom{ bottom }
	, width{ width }
	, height{ height }
{
}