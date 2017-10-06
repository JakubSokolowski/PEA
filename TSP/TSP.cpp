// TSP.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "TSP.h"


// This is an example of an exported variable
TSP_API int nTSP=0;

// This is an example of an exported function.
TSP_API int fnTSP(void)
{
    return 42;
}

// This is the constructor of a class that has been exported.
// see TSP.h for the class definition
CTSP::CTSP()
{
    return;
}
