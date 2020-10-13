set(ECS_PrivateDeps
	"Core")
set(ECS_PublicDeps
	"")

DECLARE_MODULE(
	MODULE_NAME "ECS" 
	PRIVATE_DEPS ${ECS_PrivateDeps} 
	PUBLIC_DEPS ${ECS_PublicDeps})
