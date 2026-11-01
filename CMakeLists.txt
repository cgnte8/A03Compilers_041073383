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
	"include/Step1Cell.h"	"src/Step1Cell.c"		"src/Main1Cell.c"
	"include/Step2Tissue.h"	"src/Step2Tissue.c"		"src/Main2Tissue.c"
	"include/Step3Organ.h"	"src/Step3Organ.c"		"src/Main3Organ.c"
  )

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Biology PROPERTY CXX_STANDARD 20)
endif()

