add_library(src
  SHARED
		pidc.c
)

target_include_directories(src
  PUBLIC ${PROJECT_SOURCE_DIR}/inc
)