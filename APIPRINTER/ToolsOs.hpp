#pragma once
#ifndef ToolsOs_hpp
#define ToolsOs_hpp

#include <Shlobj.h>
#include <iostream>
#include <process.h>
#include <fstream>

namespace SystemUtilities {
	class Config {
	public:
		std::string readLine(std::string name) {
			std::string data = "";
			std::ifstream infile(name.c_str());
			std::getline(infile, data, '\n');
			return data;
		}
		
		std::string readFile(std::string name) {

			std::ifstream infile(name.c_str());

			std::string data;

			if (!infile.bad()) {

				infile.seekg(0, std::ios::end);
				data.reserve(infile.tellg());
				infile.seekg(0, std::ios::beg);
				data.assign(
					(std::istreambuf_iterator<char>(infile)),
					std::istreambuf_iterator<char>()
				);
			}
			OATPP_LOGD("data: ", "%s", data.c_str());
			return data;
		}

	};

	class getPath {
		char tmp[MAX_PATH] = { 0 };
	public:
		std::string documents() {
			SHGetSpecialFolderPathA(NULL, tmp, CSIDL_COMMON_DOCUMENTS, TRUE);
			std::string dir(tmp);
			return dir;
		}
		std::string home() {
			SHGetSpecialFolderPathA(NULL, tmp, CSIDL_PROFILE, TRUE);
			std::string dir(tmp);
			dir.append("\\");
			return dir;
		}
		std::string temp() {
			SHGetSpecialFolderPathA(NULL, tmp, CSIDL_PROFILE, TRUE);
			std::string dir(tmp);
			dir.append("\\AppData\\Local\\Temp\\");
			return dir;
		}

		bool exist_directory(const std::string& path_name) {
			unsigned int file_attrib = GetFileAttributesA(path_name.c_str());
			if (file_attrib == INVALID_FILE_ATTRIBUTES) return false;
			if (file_attrib == FILE_ATTRIBUTE_DIRECTORY) return true;
			return false;
		}
		void create_work_directory(const std::string& path) {
			std::string cmd("MD ");
			cmd.append(path.c_str());
			system(cmd.c_str());
		}


	};
	class ProgressPrintFlow {

	public:
		std::string call_process_printto(std::string printto, std::string absolute_path_name) {
			
			std::string commnad = "\"" + printto + "\"";
			printto = commnad;

			std::string taskkill = "TIMEOUT 9 & TASKKILL /F /PID  ";
			unsigned int PID = 0;


			SHELLEXECUTEINFO ShExecInfo = { 0 };

			ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
			ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
			ShExecInfo.hwnd = NULL;
			ShExecInfo.lpVerb = "printto";
			ShExecInfo.lpFile = absolute_path_name.c_str();
			ShExecInfo.lpParameters = printto.c_str();
			ShExecInfo.lpDirectory = NULL;
			ShExecInfo.nShow = SW_HIDE;
			ShExecInfo.hInstApp = NULL;
			ShellExecuteEx(&ShExecInfo);

			PID = GetProcessId(ShExecInfo.hProcess);
			std::string pid_s = std::to_string(PID);
			
			commnad = "";
			commnad = taskkill + pid_s;
			
			return commnad;
		}

		std::string call_process_print(std::string absolute_path_name ) {
			//ShellExecute(0, "print", "C:\\Temp\\file.pdf", NULL, NULL, SW_HIDE);
			//fileOutputStream.close();
			//system("timeout /t 3 & taskkill /F /IM Acrobat.exe");

			//WaitForSingleObject(ShExecInfo.hProcess, 2800);
			//std::string taskkill = "TIMEOUT /t & TASKKILL /F /PID  ";
			//std::string pid_s = std::to_string(PID);
			//std::string command = taskkill + pid_s;
			
			std::string taskkill = "TIMEOUT 9 & TASKKILL /F /PID  ";
			unsigned int PID = 0;
//			SHELLEXECUTEINFO ShExecInfo = { 0 };
			SHELLEXECUTEINFO ShExecInfo;
			ZeroMemory(&ShExecInfo, sizeof(SHELLEXECUTEINFO));
			ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
			ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
			ShExecInfo.hwnd = NULL;
			ShExecInfo.lpVerb = "print";
			ShExecInfo.lpFile = absolute_path_name.c_str();
			ShExecInfo.lpParameters = NULL;
			ShExecInfo.lpDirectory = NULL;
			ShExecInfo.nShow = SW_HIDE;
			ShExecInfo.hInstApp = NULL;
			ShellExecuteEx(&ShExecInfo);

			PID = ::GetProcessId(ShExecInfo.hProcess);
			std::string pid_s = std::to_string(PID);

			std::string command = taskkill + pid_s;
			//WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
			//CloseHandle(ShExecInfo.hProcess);
			
			OATPP_LOGD("cmd", "%s", command.c_str());
			return command;
		}
		
		//bool Get_Job_Printer() {

		//	HANDLE hPrinter;
		//	char name[] = "EPSON LX-350";
		//	OATPP_LOGD("warning", "%s", name);
		//	OpenPrinter(name, &hPrinter, NULL);
		//	if (OpenPrinter(name, &hPrinter, NULL) == 0)
		//	{
		//		OATPP_LOGD("warning", "%s", "OpenPrinter call failed");
		//		return FALSE;
		//	}

		//	JOB_INFO_1* pJobInfo = NULL;
		//	DWORD bytesNeeded = 0, jobsReturned = 0;
		//	DWORD numJobs = 0;

		//	//Get info about jobs in queue.
		//	EnumJobs(hPrinter, 0, numJobs, 1, (LPBYTE)pJobInfo, 0, &bytesNeeded, &jobsReturned);
		//	pJobInfo = (JOB_INFO_1*)malloc(bytesNeeded);
		//	
		//	EnumJobs(hPrinter, 0, numJobs, 1, (LPBYTE)&pJobInfo, bytesNeeded, &bytesNeeded, &jobsReturned);
		//	OATPP_LOGD("count jobs", "%d", numJobs);
		//	//Loop and delete each waiting job
		//	for (unsigned int count = 0; count < jobsReturned; count++)
		//	{
		//		OATPP_LOGD("count jobs", "%d", pJobInfo[count].JobId);
		//		//if (SetJob(hPrinter, pJobInfo[count].JobId, 0, NULL, JOB_CONTROL_DELETE) != 0)
		//		//{
		//		//	OATPP_LOGD("delete job","%s", "...... Deleted OK");
		//		//}
		//		//else
		//		//{
		//		//	OATPP_LOGD("delete job", "%s", "...... Failed to Delete");	
		//		//}
		//	}



		//	free(pJobInfo);//free now
		//	return TRUE;
		//}
	};

}




#endif // printertool_hpp

