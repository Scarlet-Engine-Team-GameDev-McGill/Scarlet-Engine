set(AssetView_PrivateDeps
	"Core"
	"UI"
	"AssetManager"
	"RAL")

set(AssetView_PublicDeps)

DECLARE_EDITOR_MODULE(
	MODULE_NAME "AssetView" 
	PRIVATE_DEPS ${AssetView_PrivateDeps} 
	PUBLIC_DEPS ${AssetView_PublicDeps})