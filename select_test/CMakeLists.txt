#project name
PROJECT(test)

#source directory
AUX_SOURCE_DIRECTORY(. DIR_SRCS)

add_executable(select_test select_test.c)
