set(AssetManager_PrivateDeps
	"Core"
	"STBImage")
set(AssetManager_PublicDeps)

DECLARE_MODULE(
	MODULE_NAME "AssetManager" 
	PRIVATE_DEPS ${AssetManager_PrivateDeps} 
	PUBLIC_DEPS ${AssetManager_PublicDeps})