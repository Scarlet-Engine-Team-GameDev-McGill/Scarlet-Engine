set(PropertyEditor_PrivateDeps
	"Core"
	"UI"
	"Editor")

set(PropertyEditor_PublicDeps)

DECLARE_EDITOR_MODULE(
	MODULE_NAME "PropertyEditor" 
	PRIVATE_DEPS ${PropertyEditor_PrivateDeps} 
	PUBLIC_DEPS ${PropertyEditor_PublicDeps})