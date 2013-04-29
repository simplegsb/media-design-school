#include <map>
#include <sstream>
#include <vector>

#include <windows.h>
#include <shobjidl.h>

#include "FileLoader.h"
#include "resource.h"

HMENU createMenu();
BOOL CALLBACK DialogWndProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam);
bool haveFileLoaderStatesChanged();
void showOpenFileDialog(HWND window, COMDLG_FILTERSPEC* filters, unsigned int filterCount);
int WINAPI WinMain(HINSTANCE instance, HINSTANCE, PSTR, int);

bool loaderClosed = false;
HMENU loadImageMenu;
std::vector<FileLoader*> fileLoaders;
std::map<FileLoader*, FileLoader::FileLoaderState> fileLoaderStates;
HMENU loadSoundMenu;
HMENU stopMenu;

HMENU createMenu()
{
	HMENU menu = CreateMenu();

	HMENU fileMenu = CreatePopupMenu();
	AppendMenu(menu, MF_STRING | MF_POPUP, (UINT) fileMenu, L"File");

	loadImageMenu = CreateMenu();
	AppendMenu(fileMenu, MF_STRING, (UINT) loadImageMenu, L"Load Image");
	loadSoundMenu = CreateMenu();
	AppendMenu(fileMenu, MF_STRING, (UINT) loadSoundMenu, L"Load Sound");

	HMENU exitMenu = CreatePopupMenu();
	AppendMenu(menu, MF_STRING | MF_POPUP, (UINT) exitMenu, L"Exit");

	stopMenu = CreateMenu();
	AppendMenu(exitMenu, MF_STRING, (UINT) stopMenu, L"Stop");

	return menu;
}

BOOL CALLBACK DialogWndProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_CLOSE)
	{
		bool loadInProgress = false;

		for (unsigned int index = 0; index < fileLoaders.size(); index++)
		{
			if (fileLoaders[index]->getState() == FileLoader::Loading ||
				fileLoaders[index]->getState() == FileLoader::Pending)
			{
				loadInProgress = true;
			}
		}

		if (loadInProgress)
		{
			MessageBox(window, L"Either wait for the files to be loaded or stop them before exiting.",
				L"Files are being loaded!", MB_OK);
		}
		else
		{
			DestroyWindow(window);
			loaderClosed = true;
		}

		return true;
	}
	else if (message == WM_INITDIALOG)
	{
		SetMenu(window, createMenu());
	}
	else if (message == WM_COMMAND)
	{
		if (wParam == (UINT) loadImageMenu)
		{
			COMDLG_FILTERSPEC fileFilter[1];
			fileFilter->pszName = L"Image Files";
			fileFilter->pszSpec = L"*.bmp;*.gif;*.jpeg;*.jpg;*.png;*.tiff";

			showOpenFileDialog(window, fileFilter, 1);

			return true;
		}
		else if (wParam == (UINT) loadSoundMenu)
		{
			COMDLG_FILTERSPEC fileFilter[1];
			fileFilter->pszName = L"Sound Files";
			fileFilter->pszSpec = L"*.mp3;*.ogg;*.wav;*.wma";

			showOpenFileDialog(window, fileFilter, 1);

			return true;
		}
		else if (wParam == (UINT) stopMenu)
		{
			for (unsigned int index = 0; index < fileLoaders.size(); index++)
			{
				fileLoaders[index]->cancel();
			}

			return true;
		}
	}

	return false;
}

bool haveFileLoaderStatesChanged()
{
	bool fileLoaderStatesChanged = false;

	for (std::map<FileLoader*, FileLoader::FileLoaderState>::iterator iterator = fileLoaderStates.begin();
		iterator != fileLoaderStates.end();
		iterator++)
	{
		if (iterator->first->getState() != iterator->second)
		{
			fileLoaderStatesChanged = true;
			iterator->second = iterator->first->getState();
		}
	}

	return fileLoaderStatesChanged;
}

void showOpenFileDialog(HWND window, COMDLG_FILTERSPEC* filters, unsigned int filterCount)
{
	IFileOpenDialog *dialog = NULL;
	CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_PPV_ARGS(&dialog));
	dialog->SetOptions(FOS_ALLOWMULTISELECT);
	// Comment the next line to allow all file types.
	dialog->SetFileTypes(filterCount, filters);
	dialog->Show(window);

	IShellItemArray *results;
    if (dialog->GetResults(&results) == S_OK)
	{
		DWORD resultCount;
		results->GetCount(&resultCount);

		for (unsigned int index = 0; index < resultCount; index++)
		{
			IShellItem *result;
			results->GetItemAt(index, &result);
			PWSTR fileName;
			result->GetDisplayName(SIGDN_FILESYSPATH, &fileName);

			FileLoader* fileLoader = new FileLoader(fileName);
			fileLoaders.push_back(fileLoader);
			fileLoaderStates[fileLoader] = FileLoader::Unknown;
			fileLoader->load();
		}
	}
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, PSTR, int)
{
	HWND loader = CreateDialog(instance, MAKEINTRESOURCE(IDD_LOADER), NULL, DialogWndProc);
	ShowWindow(loader, SW_SHOW);

	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

	MSG message;

	do
	{
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			// It's poorly named, but IsDialogMessage actually forwards the message to the dialog.
			if(loader == NULL || !IsDialogMessage(loader, &message))
			{
				TranslateMessage(&message);
				DispatchMessage(&message);
			}
		}

		if (haveFileLoaderStatesChanged())
		{
			std::wstringstream fileLoadStates;
			for (unsigned int index = 0; index < fileLoaders.size(); index++)
			{
				fileLoadStates << *fileLoaders[index];
			}

			SetDlgItemText(loader, IDC_LOADSTATES, fileLoadStates.str().c_str());
		}
	}
	while (!loaderClosed);

	CoUninitialize();

	for (unsigned int index = 0; index < fileLoaders.size(); index++)
	{
		delete fileLoaders[index];
	}

	return 0;
}
