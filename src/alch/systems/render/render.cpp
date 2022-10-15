#include "render.hpp"

#include "camera.hpp"
//
#include "alch/engine/context.hpp"
//
#include <SDL_syswm.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/platform.h>
// #include <SFML/Graphics.hpp>
//

using namespace alch;

Render::Render(Context& context): _context(context), _target(context.systemRef<WindowSystem>().window()) {
	SDL_SysWMinfo wmi;
	SDL_VERSION(&wmi.version);
	// TODO:check this
	SDL_GetWindowWMInfo(&_target, &wmi);

	bgfx::renderFrame(); // single threaded mode

	bgfx::PlatformData pd{};
#if BX_PLATFORM_WINDOWS
	pd.nwh = wmi.info.win.window;
#elif BX_PLATFORM_OSX
	pd.nwh = wmi.info.cocoa.window;
#elif BX_PLATFORM_LINUX
	pd.ndt = wmi.info.x11.display;
	pd.nwh = (void*)(uintptr_t)(wmi.info.x11.window);
#elif BX_PLATFORM_EMSCRIPTEN
	pd.nwh = (void*)"#canvas";
#endif // BX_PLATFORM_WINDOWS ? BX_PLATFORM_OSX ? BX_PLATFORM_LINUX ?
       // BX_PLATFORM_EMSCRIPTEN

	int sdlw, sdlh;
	SDL_GetWindowSize(&_target, &sdlw, &sdlh);

	bgfx::Init bgfx_init;
	bgfx_init.type = bgfx::RendererType::Count; // auto choose renderer
	bgfx_init.resolution.width = sdlw;
	bgfx_init.resolution.height = sdlh;
	bgfx_init.resolution.reset = BGFX_RESET_VSYNC;
	bgfx_init.platformData = pd;

	bgfx::init(bgfx_init);
}

Render::~Render() {
	bgfx::shutdown();
}

void Render::render() {
	if (_camera) {
		_camera->update();
		// _target.setView(_camera->sf());
	}

	// for (auto& r : _drawables) {
	// r->draw(_target, RenderStates::Default);
	// }
}
