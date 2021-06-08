set(SceneHierarchy_PrivateDeps
	"Core"
	"UI"
	"ECS"
	"Gameplay"
	"Editor")

set(SceneHierarchy_PublicDeps)

DECLARE_EDITOR_MODULE(
	MODULE_NAME "SceneHierarchy"
	PRIVATE_DEPS ${SceneHierarchy_PrivateDeps} 
	PUBLIC_DEPS ${SceneHierarcy_PublicDeps})