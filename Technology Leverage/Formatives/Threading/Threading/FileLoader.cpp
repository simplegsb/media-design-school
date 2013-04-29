#include <fstream>
#include <ostream>

#include "FileLoader.h"

unsigned __stdcall loadFileOnThread(void* data)
{
	static_cast<FileLoader*>(data)->loadData();
	return 0;
}

FileLoader::FileLoader(const std::wstring& fileName) :
	fileName(fileName),
	loadedFileData(NULL),
	loadedFileLength(0),
	state(Pending),
	thread(NULL)
{
	thread = new Thread(loadFileOnThread, this);
}

FileLoader::~FileLoader()
{
	if (loadedFileData != NULL)
	{
		delete[] loadedFileData;
	}
}

void FileLoader::cancel()
{
	thread->interrupt();
}

char* FileLoader::getFileData() const
{
	return loadedFileData;
}

unsigned int FileLoader::getFileLength() const
{
	return loadedFileLength;
}

FileLoader::FileLoaderState FileLoader::getState() const
{
	return state;
}

void FileLoader::load()
{
	thread->start();
}

void FileLoader::loadData()
{
	this->fileName = fileName;
	state = Loading;

	try
	{
		std::ifstream file(fileName.c_str());
		if (file.fail())
		{
			state = Failed;
			return;
		}

		std::streampos fileSize = file.tellg();
		file.seekg(0, std::ios::end);
		fileSize = file.tellg() - fileSize;

		loadedFileData = new char[fileSize];
		file.seekg(0);

		while (!file.eof())
		{
			if (thread->isInterrupted())
			{
				state = Cancelled;
				return;
			}

			file.read(&loadedFileData[0], 1);
			// Uncomment the next line for artificial slow-down testing.
			//Sleep(5);
		}

		state = Loaded;

		//delete[] loadedFileData;
		//loadedFileData = NULL;
	}
	catch (...)
	{
		state = Failed;
	}
}

std::wostream& operator<<(std::wostream& stream, const FileLoader& fileLoader)
{
	stream << "File: " << fileLoader.fileName.c_str() << std::endl;
	if (fileLoader.getState() == FileLoader::Cancelled)
	{
		stream << "State: Cancelled" << std::endl;
	}
	else if (fileLoader.getState() == FileLoader::Failed)
	{
		stream << "State: Failed" << std::endl;
	}
	else if (fileLoader.getState() == FileLoader::Loaded)
	{
		stream << "State: Loaded" << std::endl;
	}
	else if (fileLoader.getState() == FileLoader::Loading)
	{
		stream << "State: Loading" << std::endl;
	}
	else if (fileLoader.getState() == FileLoader::Pending)
	{
		stream << "State: Pending" << std::endl;
	}
	else
	{
		stream << "State: UNKNOWN!" << std::endl;
	}

	return stream;
}
