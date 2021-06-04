set(Engine_PrivateDeps
	"Window")

set(Engine_PublicDeps
	"Core"
	"Gameplay")

DECLARE_MODULE(
	MODULE_NAME "Engine" 
	PRIVATE_DEPS ${Engine_PrivateDeps} 
	PUBLIC_DEPS ${Engine_PublicDeps})