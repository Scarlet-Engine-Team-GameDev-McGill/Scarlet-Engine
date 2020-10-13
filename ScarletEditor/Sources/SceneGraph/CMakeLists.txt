set(SceneGraph_PrivateDeps
	"Core"
	"UI"
	"ECS"
	"Gameplay"
	"Editor")

set(SceneGraph_PublicDeps)

DECLARE_EDITOR_MODULE(
	MODULE_NAME "SceneGraph" 
	PRIVATE_DEPS ${SceneGraph_PrivateDeps} 
	PUBLIC_DEPS ${SceneGraph_PublicDeps})