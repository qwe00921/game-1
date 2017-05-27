#include "Common.h"
#include "Scripts.h"

int ScriptInitValue::HandleScript(){
	memset(history->var, 0, 256);
	return 0;
}