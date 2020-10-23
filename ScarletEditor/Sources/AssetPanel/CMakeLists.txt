set(AssetPanel_PrivateDeps
	"Core"
	"UI"
	"AssetManager"
	"RAL")

set(AssetPanel_PublicDeps)

DECLARE_EDITOR_MODULE(
	MODULE_NAME "AssetPanel" 
	PRIVATE_DEPS ${AssetPanel_PrivateDeps} 
	PUBLIC_DEPS ${AssetPanel_PublicDeps})