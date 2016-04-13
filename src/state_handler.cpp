#include "state_handler.h"

State_handler::State_handler(const Resource_manager& init_manager, Renderer& init_renderer){
	manager = &init_manager;
	renderer = &init_renderer;
	game_running = true;
	menu_open = false;
	paused = false;
}

void State_handler::handle_events(){
	while (SDL_PollEvent(&ehandler) != 0 ) {
		/* User requests quit */
		if (ehandler.type == SDL_QUIT) {
			game_running = false;
		}
		else if (ehandler.type == SDL_KEYDOWN) {
			if (ehandler.key.keysym.scancode == manager->get_button_map_key("player", QUIT)) {
				game_running = false;
			}
			else if (ehandler.key.keysym.scancode == manager->get_button_map_key("player", A0)) {
				renderer->toggle_aliasing();
			}
			else if (ehandler.key.keysym.scancode == manager->get_button_map_key("player", A1)) {
				renderer->toggle_bloom();
			}
			else if (ehandler.key.keysym.scancode == manager->get_button_map_key("player", A2)) {
				renderer->toggle_ambient_occlusion();
			}
			else if (ehandler.key.keysym.scancode == manager->get_button_map_key("player", A3)) {
				renderer->toggle_shadows();
			}
		}
		else if (ehandler.type == SDL_WINDOWEVENT) {
			switch (ehandler.window.event) {
				case SDL_WINDOWEVENT_SHOWN:
				SDL_Log("Window %d shown", ehandler.window.windowID);
				break;
			case SDL_WINDOWEVENT_HIDDEN:
				SDL_Log("Window %d hidden", ehandler.window.windowID);
				break;
			case SDL_WINDOWEVENT_EXPOSED:
				/* SDL_Log("Window %d exposed", ehandler.window.windowID); */
				break;
			case SDL_WINDOWEVENT_MOVED:
				/* SDL_Log("Window %d moved to %d,%d",
						ehandler.window.windowID, ehandler.window.data1,
						ehandler.window.data2); */
				break;
			case SDL_WINDOWEVENT_RESIZED:
				SDL_Log("Window %d resized to %dx%d",
						ehandler.window.windowID, ehandler.window.data1,
						ehandler.window.data2);
				break;
			case SDL_WINDOWEVENT_SIZE_CHANGED:
				SDL_Log("Window %d size changed to %dx%d",
						ehandler.window.windowID, ehandler.window.data1,
						ehandler.window.data2);
				break;
			case SDL_WINDOWEVENT_MINIMIZED:
				SDL_Log("Window %d minimized", ehandler.window.windowID);
				break;
			case SDL_WINDOWEVENT_MAXIMIZED:
				SDL_Log("Window %d maximized", ehandler.window.windowID);
				break;
			case SDL_WINDOWEVENT_RESTORED:
				SDL_Log("Window %d restored", ehandler.window.windowID);
				break;
			case SDL_WINDOWEVENT_ENTER:
				/* SDL_Log("Mouse entered window %d",
						ehandler.window.windowID); */
				break;
			case SDL_WINDOWEVENT_LEAVE:
				/* SDL_Log("Mouse left window %d", ehandler.window.windowID); */
				break;
			case SDL_WINDOWEVENT_FOCUS_GAINED:
				/* SDL_Log("Window %d gained keyboard focus",
						ehandler.window.windowID); */
				break;
			case SDL_WINDOWEVENT_FOCUS_LOST:
				/* SDL_Log("Window %d lost keyboard focus",
						ehandler.window.windowID); */
				break;
			case SDL_WINDOWEVENT_CLOSE:
				SDL_Log("Window %d closed", ehandler.window.windowID);
				break;
			default:
				SDL_Log("Window %d got unknown event %d",
						ehandler.window.windowID, ehandler.window.event);
				break;
			}
		}
	}
}