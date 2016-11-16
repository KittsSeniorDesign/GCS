#if defined(__APPLE__)
#include "Gamepad_macosx.c"
#elif defined(__linux__)
#include "Gamepad_linux.c"
#elif defined(_WIN32)
#include "Gamepad_windows.c"
#else
#error Your platform is not supported.
#endif
