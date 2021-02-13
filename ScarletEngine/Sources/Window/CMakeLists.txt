set(Window_PrivateDeps
	"glfw"
	"AssetManager")

set(Window_PublicDeps
	"Core")

DECLARE_MODULE(
	MODULE_NAME "Window" 
	PRIVATE_DEPS ${Window_PrivateDeps} 
	PUBLIC_DEPS ${Window_PublicDeps})