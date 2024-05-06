#pragma once

#ifdef __cplusplus // only actually define the class if this is C++
class Keyboard_Controller
{
public:
	Keyboard_Controller();
};
#else

// C doesn't know about classes, just say it's a struct
typedef struct Keyboard_Controller Keyboard_Controller;

#endif

// access functions
#ifdef __cplusplus
#define EXPORT_C extern "C"
#else
#define EXPORT_C
#endif

EXPORT_C Keyboard_Controller* Keyboard_Controller_new(void);
EXPORT_C void Keyboard_Controller_TranslateEgoVhcl(Keyboard_Controller*);