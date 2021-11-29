#ifdef EXERCISE2_DLL_EXPORTS
#define submit_API __declspec(dllexport) 
#else
#define submit_API __declspec(dllimport) 
#endif

int submit_API submit(int(*f)(int, double*, double*, double*));
#pragma once
#pragma once
