#pragma once

#include <string>

#include "Thread.h"

class FileLoader
{
	public:
		enum FileLoaderState
		{
			Cancelled,
			Failed,
			Loaded,
			Loading,
			Pending,
			Unknown
		};

		FileLoader(const std::wstring& fileName);

		~FileLoader();

		void cancel();

		char* getFileData() const;

		unsigned int getFileLength() const;

		FileLoaderState getState() const;

		void load();

		friend unsigned __stdcall loadFileOnThread(void* data);

		friend std::wostream& operator<<(std::wostream& stream, const FileLoader& fileLoader);

	private:
		std::wstring fileName;

		char* loadedFileData;

		unsigned int loadedFileLength;

		Thread* thread;

		FileLoaderState state;

		void loadData();
};
