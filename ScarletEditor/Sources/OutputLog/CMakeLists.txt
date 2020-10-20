set(OutputLog_PrivateDeps
	"Core"
	"UI")

set(OutputLog_PublicDeps)

DECLARE_EDITOR_MODULE(
	MODULE_NAME "OutputLog" 
	PRIVATE_DEPS ${OutputLog_PrivateDeps} 
	PUBLIC_DEPS ${OutputLog_PublicDeps})