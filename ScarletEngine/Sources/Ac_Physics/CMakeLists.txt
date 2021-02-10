set(Ac_Physics_PrivateDeps
	"Core"
	"Ac_Physics")

set(Ac_Physics_PublicDeps
	"")

DECLARE_MODULE(
	MODULE_NAME "Ac_Physics" 
	PRIVATE_DEPS ${Ac_Physics_PrivateDeps} 
	PUBLIC_DEPS ${Ac_Physics_PublicDeps})
