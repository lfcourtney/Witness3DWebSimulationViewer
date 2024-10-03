#define WIN32_LEAN_AND_MEAN

#include "OpenFileDialog.h"

#include <Windows.h>
#include <Commdlg.h>

#include <stdlib.h>
#include <string.h>

typedef struct {
	unsigned long ProcessId;
	void* HandleRoot;
	void* HandleFirst;
} FindMainWindowInfo;

static int FindMainWindowCallback(HWND Handle, LPARAM Param) {
	FindMainWindowInfo* Info = reinterpret_cast<FindMainWindowInfo*>(Param);
	unsigned long ProcessId = 0;
	
	GetWindowThreadProcessId(Handle, &ProcessId);
	
	if (Info->ProcessId == ProcessId) {
		Info->HandleFirst = Handle;
		if (GetWindow(Handle, GW_OWNER) == nullptr && IsWindowVisible(Handle)) {
			Info->HandleRoot = Handle;
			return 0;
		}
	}
	return 1;
}

static HWND FindMainWindow() {
	FindMainWindowInfo Info { GetCurrentProcessId() };
	EnumWindows(FindMainWindowCallback, reinterpret_cast<LPARAM>(&Info));
	return static_cast<HWND>(Info.HandleRoot);
}

static int NextFilter(wchar_t *FilterDestination, const wchar_t **Filters) {
	*Filters += wcsspn(*Filters, L"|");
	if (**Filters == '\0') {
		return 0;
	}

	int Len = wcscspn(*Filters, L"|");
	wmemcpy(FilterDestination, *Filters, Len);
	FilterDestination[Len] = '\0';
	*Filters += Len;

	return 1;
}

static const wchar_t* MakeFilter(FDialogOptions DialogOptions) {
	static wchar_t Buffer[1024];
	int Index = 0;

	Buffer[0] = '\0';

	if (!DialogOptions.Filter.IsEmpty()) {
		const wchar_t* Filters;
		wchar_t Filter[32];
		
		const wchar_t* Name = !DialogOptions.FilterName.IsEmpty() ? *DialogOptions.FilterName : *DialogOptions.Filter;
		Index += swprintf_s(Buffer + Index, 1024 - Index, L"%s", Name) + 1;
		
		Filters = *DialogOptions.Filter;
		while (NextFilter(Filter, &Filters)) {
			Index += swprintf_s(Buffer + Index, 1024 - Index, L"%s;", Filter);
		} 

		Buffer[++Index] = '\0';
	}

	Index += swprintf_s(Buffer + Index, 1024 - Index, L"All Files") + 1;
	Index += swprintf_s(Buffer + Index, 1024 - Index, L"*.*");
	Buffer[++Index] = '\0';

	return Buffer;
}

static void InitializeOpenFileName(OPENFILENAME *OpenFileName, FDialogOptions DialogOptions) {
	static wchar_t ResultBuffer[2048];
	static wchar_t Title[256];
	static wchar_t Path[256];
	static wchar_t Extension[256];

	wmemcpy(Title, *DialogOptions.Title, DialogOptions.Title.Len());
	wmemcpy(Path, *DialogOptions.Path, DialogOptions.Path.Len());
	wmemcpy(Extension, *DialogOptions.Extension, DialogOptions.Extension.Len());
	
	ResultBuffer[0] = '\0';
	
	memset(OpenFileName, 0, sizeof(*OpenFileName));
	OpenFileName->hwndOwner        = (HWND) (void*) FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr);
	;
	OpenFileName->lStructSize      = sizeof(*OpenFileName);
	OpenFileName->lpstrFilter      = MakeFilter(DialogOptions);
	OpenFileName->nFilterIndex     = 1;
	OpenFileName->lpstrFile        = ResultBuffer;
	OpenFileName->Flags            = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	OpenFileName->nMaxFile         = sizeof(ResultBuffer) - 1;
	OpenFileName->lpstrInitialDir  = Path;
	OpenFileName->lpstrTitle       = Title;
	OpenFileName->lpstrDefExt      = Extension;
}

FString UOpenFileDialog::OpenFileDialog(FDialogOptions DialogOptions)
{
	OPENFILENAME OpenFileName;
	InitializeOpenFileName(&OpenFileName, DialogOptions);
	return GetOpenFileName(&OpenFileName) ? OpenFileName.lpstrFile : nullptr;
}
