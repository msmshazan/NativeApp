#include "main.h"


struct RendererState {
	ImGuiIO* io;
	ImVec4* clearColor;
};

struct PersistantState {
	float f;
	int counter;
	bool AppRunning;
};

struct AppState {
	RendererState* RendererState;
	PersistantState* PersistantState;
};

global bool loading = true;




void AppRenderFunction(AppState* appState) 
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	local_persist bool showDemo = false;
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);

	RendererState* renderState = appState->RendererState;
	ImGuiIO* io = renderState->io;
	ImVec4* clearColor = renderState->clearColor;
	PersistantState* persist = appState->PersistantState;
	
	ImGui::Begin("Test", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus);
	{
		ImGui::SetNextWindowSize(ImVec2(200,60));
		if (ImGui::BeginPopupModal("Alert", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
		{
			ImGui::Text("Connecting.");
			ImGui::EndPopup();
		}
		if (loading) 
		{
			ImGui::OpenPopup("Alert");
		}
		else 
		{

			if (ImGui::BeginMainMenuBar()) {
				ImGui::MenuItem("File");

				ImGui::MenuItem("Settings");
				if (ImGui::BeginMenu("Utils")) {
					if (ImGui::MenuItem("Demo")) showDemo = true;
					ImGui::EndMenu();
				}
				if (ImGui::MenuItem("Quit")) persist->AppRunning = false;
				ImGui::EndMainMenuBar();
			}

			ImGui::Text("This is some useful text.");
			ImGui::SliderFloat("float", &(persist->f), 0.0f, 1.0f);
			ImGui::ColorEdit3("clear color", (float*)clearColor);

			if (ImGui::Button("Style Dark")) ImGui::StyleColorsDark();
			if (ImGui::Button("Style Light")) ImGui::StyleColorsLight();
			if (ImGui::Button("Style Classic")) ImGui::StyleColorsClassic();
			if (ImGui::Button("Counter ++")) persist->counter++;
			ImGui::Text("counter = %d", persist->counter);
		}
		ImGui::End();
	}
	
	ImGui::PopStyleVar();
	if (showDemo) {
		ImGui::ShowDemoWindow(&showDemo);
	}

	ImGui::Render();

	// Rendering
	const float clear_color_with_alpha[4] = { clearColor->x * clearColor->w, clearColor->y * clearColor->w, clearColor->z * clearColor->w, clearColor->w };
	g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
	g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	g_pSwapChain->Present(1, 0);
}


int main(int argc, char** argv)
{

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		printf("Error: %s\n", SDL_GetError());
		return -1;
	}
	SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");

	SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
	SDL_Window* window = SDL_CreateWindow("Native App", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
	
	SDL_SysWMinfo wmInfo = {};
	SDL_VERSION(&wmInfo.version);
	SDL_GetWindowWMInfo(window, &wmInfo);
	HWND hwnd = (HWND)wmInfo.info.win.window;

	// Initialize Direct3D
	if (!CreateDeviceD3D(hwnd))
	{
		CleanupDeviceD3D();
		return 1;
	}
	

	ImGui::CreateContext();
	ImGuiIO io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	io.Fonts->AddFontFromFileTTF("Fonts/LiberationMono-Regular.ttf",16);
	io.Fonts->AddFontFromFileTTF("Fonts/traditional-arabic.ttf", 16);
	ImVec4 clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	ImGui_ImplSDL2_InitForD3D(window);
	ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);
	
	AppState* state =  (AppState *) SDL_malloc(sizeof(AppState));
	state->RendererState = (RendererState*)SDL_malloc(sizeof(RendererState));
	state->PersistantState = (PersistantState*)SDL_malloc(sizeof(PersistantState));
	*state->PersistantState = {};
	state->PersistantState->AppRunning = true;
	state->RendererState->clearColor = (ImVec4*)SDL_malloc(sizeof(ImVec4));
	*state->RendererState->clearColor = clearColor;
	state->RendererState->io = &io;
	while (state->PersistantState->AppRunning)
	{
		SDL_Event event;
		if(SDL_WaitEvent(&event) >= 0)
		{
			ImGui_ImplSDL2_ProcessEvent(&event);
			switch (event.type)
			{
			case SDL_QUIT:
				state->PersistantState->AppRunning = false;
				break;
			case SDL_WINDOWEVENT:
				switch (event.window.event)
				{
				case SDL_WINDOWEVENT_RESIZED:
					if (event.window.windowID == SDL_GetWindowID(window))
					{
						// Release all outstanding references to the swap chain's buffers before resizing.
						CleanupRenderTarget();
						g_pSwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
						CreateRenderTarget();
					}
					break;
				case SDL_WINDOWEVENT_CLOSE:
					if (event.window.windowID == SDL_GetWindowID(window))
					{
						state->PersistantState->AppRunning = false;
					}
					break;
				default:
					break;
				}
				break;
			default:
				break;
			}

			AppRenderFunction(state);

		}
		else 
		{
			SDL_Log("Error processing event!");
		}
		
	}
	// Cleanup
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	CleanupDeviceD3D();
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}