set(Ac_Physics_PrivateDeps
	"")

set(Ac_Physics_PublicDeps
	"ECS"
	"Core")

DECLARE_MODULE(
	MODULE_NAME "Ac_Physics" 
	PRIVATE_DEPS ${Ac_Physics_PrivateDeps} 
	PUBLIC_DEPS ${Ac_Physics_PublicDeps})
