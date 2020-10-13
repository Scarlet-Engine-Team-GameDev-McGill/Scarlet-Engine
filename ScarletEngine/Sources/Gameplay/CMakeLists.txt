set(Gameplay_PrivateDeps
	"Core"
	"RenderModule")

set(Gameplay_PublicDeps
	"ECS")

DECLARE_MODULE(
	MODULE_NAME "Gameplay" 
	PRIVATE_DEPS ${Gameplay_PrivateDeps} 
	PUBLIC_DEPS ${Gameplay_PublicDeps})