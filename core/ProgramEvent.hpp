#pragma once
#include "Windows.h"
#include <sfml/Graphics.hpp>
class ProgramEvent {
private:
	HANDLE m_Handle;
public:
	ProgramEvent() {
		m_Handle = CreateEvent(
			NULL,               // default security attributes
			FALSE,               // manual-reset event
			FALSE,              // initial state is nonsignaled
			TEXT("WriteEvent")  // object name
		);
	}

	~ProgramEvent() {
		CloseHandle(m_Handle);
	}

	void Signal() {
		SetEvent(m_Handle);
	}

	void Wait() {
		WaitForSingleObject(m_Handle, INFINITE);
	}
};
