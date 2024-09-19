# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\PixelPuppet_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\PixelPuppet_autogen.dir\\ParseCache.txt"
  "PixelPuppet_autogen"
  )
endif()
