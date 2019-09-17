cmake_minimum_required(VERSION 3.10)

ADD_DEFINITIONS(-DUNICODE)
ADD_DEFINITIONS(-D_UNICODE)

file(GLOB INJECTOR_SOURCES "main.cpp")

file(GLOB INTERCEPTOR_SOURCES "msg_iternceptor.cpp")

project(intercept_window_messages)
add_executable(intercept_window_messages ${INJECTOR_SOURCES} )

if(WIN32)
   set_target_properties(intercept_window_messages PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE")
   set_target_properties(intercept_window_messages PROPERTIES COMPILE_DEFINITIONS_DEBUG "_CONSOLE")
   set_target_properties(intercept_window_messages PROPERTIES LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:CONSOLE")
   set_target_properties(intercept_window_messages PROPERTIES COMPILE_DEFINITIONS_RELWITHDEBINFO "_CONSOLE")
   set_target_properties(intercept_window_messages PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:CONSOLE")
   set_target_properties(intercept_window_messages PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:CONSOLE")
endif(WIN32)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT intercept_window_messages)
 

add_library(msg_interceptor SHARED ${INTERCEPTOR_SOURCES} )
