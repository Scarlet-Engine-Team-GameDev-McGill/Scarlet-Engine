set(RenderModule_PrivateDeps
	"Core"
	"OpenGLRAL"
	"VulkanRAL"
    "ECS")

set(RenderModule_PublicDeps
	"RAL")

DECLARE_MODULE(
	MODULE_NAME "RenderModule" 
	PRIVATE_DEPS ${RenderModule_PrivateDeps} 
	PUBLIC_DEPS ${RenderModule_PublicDeps})
