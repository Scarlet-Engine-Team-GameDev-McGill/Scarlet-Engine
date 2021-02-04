set(RAL_PrivateDeps
	"Core"
	"Window")

set(RAL_PublicDeps
	"AssetManager")

DECLARE_MODULE(
	MODULE_NAME "RAL" 
	PRIVATE_DEPS ${RAL_PrivateDeps} 
	PUBLIC_DEPS ${RAL_PublicDeps})
