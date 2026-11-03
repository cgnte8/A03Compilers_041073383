# Makefile for A03

cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Biology")

# Add source to this project's executable.
add_executable (Biology
	"include/Biology.h"		"src/Biology.c"
	"include/Step1Cell.h"	"src/Step1Cell.c"		"src/Main1Cell.c"		"src/Test1Cell.c"
	"include/Step2Tissue.h"	"src/Step2Tissue.c"		"src/Main2Tissue.c"		"src/Test2Tissue.c"
	"include/Step3Organ.h"	"src/Step3Organ.c"		"src/Main3Organ.c"		"src/Test3Organ.c"
)

set_property(TARGET Biology PROPERTY C_STANDARD 99)
set_property(TARGET Biology PROPERTY C_STANDARD_REQUIRED ON)

