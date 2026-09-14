#include "render_core.c"

#if RENDER_BACKEND == RENDER_BACKEND_OPENGL
#   include "render_opengl.c"
#   if OS_LINUX
#       include "render_egl.c"
#   else
#       error no OpenGL render layer for this platform
#   endif
#else
#   error no render layer for this platform
#endif
