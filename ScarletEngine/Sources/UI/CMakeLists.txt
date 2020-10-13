set(UI_PrivateDeps
	"Core"
	"RAL"
	"RenderModule"
	"glfw"
	"glad")

set(UI_PublicDeps
	"ImGUI"
	"IconFontCppHeaders")

DECLARE_MODULE(
	MODULE_NAME "UI" 
	PRIVATE_DEPS ${UI_PrivateDeps} 
	PUBLIC_DEPS ${UI_PublicDeps})