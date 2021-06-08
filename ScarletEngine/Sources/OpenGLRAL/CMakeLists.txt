set(OpenGLRAL_PrivateDeps
	"Core"
	"RAL"
	"glfw"
	"OpenGL::GL"
	"glad"
	"Window")

set(OpenGLRAL_PublicDeps
	"Engine")

DECLARE_MODULE(
	MODULE_NAME "OpenGLRAL" 
	PRIVATE_DEPS ${OpenGLRAL_PrivateDeps} 
	PUBLIC_DEPS ${OpenGLRAL_PublicDeps})
