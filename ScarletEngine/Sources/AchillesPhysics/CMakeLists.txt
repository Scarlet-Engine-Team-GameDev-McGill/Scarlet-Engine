set(AchillesPhysics_PrivateDeps
	"")

set(AchillesPhysics_PublicDeps
	"ECS"
	"Core"
	"Engine"
	"Editor"
	"Gameplay")

DECLARE_MODULE(
	MODULE_NAME "AchillesPhysics" 
	PRIVATE_DEPS ${AchillesPhysics_PrivateDeps} 
	PUBLIC_DEPS ${AchillesPhysics_PublicDeps})
