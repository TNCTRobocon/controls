add_library(src
  SHARED
		pidc.c
    omni.c
    im920s.c
)

target_include_directories(src
  PUBLIC ${PROJECT_SOURCE_DIR}/inc
)