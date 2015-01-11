set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})

set(LIBSOURCES ${LIBSOURCES}
	libskype_internals.h
	libskype.cpp
	contact.cpp
	chat.cpp
	message.cpp
	dbus_connection.cpp
	dbus_connection.h
	tokenizer.cpp
	tokenizer.h
	)


add_library(skype SHARED ${LIBSOURCES})

add_executable(skypecli skypecli.cpp)
target_link_libraries(skypecli skype)

