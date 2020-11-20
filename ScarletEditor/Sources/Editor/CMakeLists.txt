set(Editor_PrivateDeps
	"Core"
	"Engine"
	"UI"
	"SceneGraph"
	"OutputLog"
	"PropertyEditor"
	"AssetView")

set(Editor_PublicDeps
	"Gameplay"
	"ECS")

DECLARE_EDITOR_MODULE(
	MODULE_NAME "Editor" 
	PRIVATE_DEPS ${Editor_PrivateDeps} 
	PUBLIC_DEPS ${Editor_PublicDeps})