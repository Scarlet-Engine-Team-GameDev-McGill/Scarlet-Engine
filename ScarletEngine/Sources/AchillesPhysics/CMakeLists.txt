set(AchillesPhysics_PrivateDeps
	"Engine")

set(AchillesPhysics_PublicDeps
	"ECS"
	"Core"
	"Editor"
	"Gameplay")

DECLARE_MODULE(
	MODULE_NAME "AchillesPhysics" 
	PRIVATE_DEPS ${AchillesPhysics_PrivateDeps} 
	PUBLIC_DEPS ${AchillesPhysics_PublicDeps})
