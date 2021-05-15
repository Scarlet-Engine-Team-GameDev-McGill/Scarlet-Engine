set(Editor_PrivateDeps
	"Core"
	"Engine"
	"UI"
	"SceneHierarchy"
	"OutputLog"
	"PropertyEditor"
	"AssetView"
	"ImGuizmo")

set(Editor_PublicDeps
	"Gameplay"
	"ECS")

DECLARE_EDITOR_MODULE(
	MODULE_NAME "Editor" 
	PRIVATE_DEPS ${Editor_PrivateDeps} 
	PUBLIC_DEPS ${Editor_PublicDeps})