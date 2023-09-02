#pragma once

#if PERFORMANCE_LOGS == 1
#define CHECKERROR(line, code)\
while(glGetError() != GL_NO_ERROR);\
code \
while(true) {\
	unsigned int error = glGetError();\
	if (error == GL_NO_ERROR) {\
		break;\
	}\
	std::cout << "[" << line << "] " << "GLERROR: " << error << "\n";\
}
#else
#define CHECKERROR(line, code) code
#endif