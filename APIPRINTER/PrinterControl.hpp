#pragma once
#ifndef PrinterControl_hpp
#define PrinterControl_hpp
#include <Windows.h>
#include <iostream>

namespace PrinterControl {
	class PrinterControl
	{
	public:
		bool GetJobs(std::string name_printer) {
			HANDLE hPrinter;
			DWORD dwNeeded, dwReturned;
			JOB_INFO_2A* pJobInfo;

			if (!OpenPrinter((char*)name_printer.c_str(), &hPrinter, NULL))
			{
				printf("Printer cannot open");
				return false;
			}
			
			if( !EnumJobs( hPrinter, 0, MAXINT32, 2, NULL, 0, &dwNeeded, &dwReturned ) )
			{
				if( GetLastError() != ERROR_INSUFFICIENT_BUFFER )
				{
					ClosePrinter( hPrinter );
 					//printf ("ERROR_INSUFFICIENT_BUFFER \n");
					OATPP_LOGD("ERROR_INSUFFICIENT_BUFFER", "%s", "");
				}
				return false;
			}
			if ((pJobInfo = (JOB_INFO_2*)malloc(dwNeeded)) == NULL)
			{
				ClosePrinter(hPrinter);
				return false;
			}
			
			if (!EnumJobs(hPrinter, 0, MAXINT32, 2, (LPBYTE)pJobInfo,
				dwNeeded, &dwNeeded, &dwReturned))
			{
				ClosePrinter(hPrinter);
				free(pJobInfo);
				return false;
			}
			ClosePrinter(hPrinter);
			if (dwReturned == 0)
			{
				OATPP_LOGI("Information","No jobs in queue");
				return false;
			}

			else
			{
				int  TotalPages = 0;
				// It's easy to loop through the jobs and access each one
				for (unsigned int i = 0;i < dwReturned;i++)
				{
					OATPP_LOGD("uintTime", "%d, %d" , pJobInfo[i].JobId, pJobInfo[i].Time);
					//OATPP_LOGD("Docuement", "%d, %s" , pJobInfo[i].JobId, pJobInfo[i].pDocument);
					//OATPP_LOGD("PagesPrited", "%d, %d", pJobInfo[i].JobId, pJobInfo[i].PagesPrinted);
					//OATPP_LOGD("Total Pages", "%d, %d", pJobInfo[i].JobId, pJobInfo[i].TotalPages);
					TotalPages = TotalPages + pJobInfo[i].TotalPages;
				}
				printf("[%d] \n", TotalPages);
				return true;
			}	

			// Clean up
			free(pJobInfo);

			return false;
		}
	};

}


#endif //END PrinterControl



