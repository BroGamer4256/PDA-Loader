#pragma once
#include "../Utilities/Math.h"

namespace MLAC::Components
{
	struct Camera
	{
		Utilities::Vec3 Position;
		Utilities::Vec3 Focus;
		float Rotation;
		float HorizontalFov;
		float VerticalFov;
	};
}