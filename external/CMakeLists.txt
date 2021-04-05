# example for a header only lib Linalg
set(LIBRARY_INCLUDES "linalg/")
add_library(linalg INTERFACE)
target_include_directories(linalg SYSTEM INTERFACE ${LIBRARY_INCLUDES}) #header only lib has to be included as INTERFACE


