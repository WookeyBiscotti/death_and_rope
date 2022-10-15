#pragma once

namespace al {

struct EngineOnFrameStart {
	float lastFps;
};

struct EngineOnFrameEnd {};

struct EngineOnFramePreRender {};

struct EngineOnFramePostRender {};

}
