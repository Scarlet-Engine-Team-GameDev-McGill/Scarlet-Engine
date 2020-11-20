set(Gameplay_PrivateDeps
	"Core")

set(Gameplay_PublicDeps
	"ECS"
	"Engine"
	"RenderModule")

DECLARE_MODULE(
	MODULE_NAME "Gameplay" 
	PRIVATE_DEPS ${Gameplay_PrivateDeps} 
	PUBLIC_DEPS ${Gameplay_PublicDeps})