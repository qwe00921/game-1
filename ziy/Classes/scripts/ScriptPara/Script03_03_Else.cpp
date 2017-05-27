#include "Common.h"
#include "Scripts.h"
//#include "ScriptElse.h"

int ScriptElse::HandleScript(){
	if(test_result)
		return -1;
	else
		return 0;
}
