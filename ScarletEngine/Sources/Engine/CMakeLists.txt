set(Engine_PrivateDeps
	"")

set(Engine_PublicDeps
	"Core")

DECLARE_MODULE(
	MODULE_NAME "Engine" 
	PRIVATE_DEPS ${Engine_PrivateDeps} 
	PUBLIC_DEPS ${Engine_PublicDeps})