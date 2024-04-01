#pragma once


inline
int ErrMessageBoxF(
	_In_z_ _Printf_format_string_ wchar_t const* const _Format,
	...) throw()
{
	wchar_t _Buffer[250];
	int _Result;
	va_list _ArgList;
	__crt_va_start(_ArgList, _Format);
	_Result = vswprintf(_Buffer, 250, _Format, _ArgList);
	__crt_va_end(_ArgList);
	OutputDebugStringW(_Buffer);
	MessageBoxW(nullptr, _Buffer, L"Error", MB_OK);
	return _Result;
}


