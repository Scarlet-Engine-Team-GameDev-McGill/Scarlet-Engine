set(VulkanRAL_PrivateDeps
	"Core"
	"RAL"
	"glfw"
	"Window")

set(VulkanRAL_PublicDeps
	"Engine"
	"Vulkan::Vulkan") # temporary. Need to find a way to make this private

DECLARE_MODULE(
	MODULE_NAME "VulkanRAL" 
	PRIVATE_DEPS ${VulkanRAL_PrivateDeps} 
	PUBLIC_DEPS ${VulkanRAL_PublicDeps})
