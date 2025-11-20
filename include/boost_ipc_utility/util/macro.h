#pragma once

// Turns macro to string
#define TO_STRING(x) #x
#define STRINGIFY(x) TO_STRING(x)

// Copied from Windows.h TEXT(x) macro. Turns macro to wstring
#define TO_WSTRING(x) L##x
#define WSTRINGIFY(x) TO_WSTRING(x)
