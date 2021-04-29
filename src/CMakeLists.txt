add_library(src
  SHARED
		pidc.c
    omni.c
)

target_include_directories(src
  PUBLIC ${PROJECT_SOURCE_DIR}/inc
)