
#ifndef ApiDTOs_hpp
#define ApiDTOs_hpp
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"

#include "oatpp/core/data/mapping/type/Object.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include <Windows.h>
#include <winspool.h>
#include "ConvertTool.hpp"
#include "oatpp/core/base/Environment.hpp"

/* Begin DTO code-generation */
#include OATPP_CODEGEN_BEGIN(DTO)


class MessageDto : public oatpp::DTO {
	DTO_INIT(MessageDto, DTO /* Extends */)
	DTO_FIELD(Int32, statusCode, "code");   // Status code field
	DTO_FIELD(String, message, "information");     // Message field
};

class DevicePrinter : public oatpp::DTO {
	DTO_INIT(DevicePrinter, DTO)
		DTO_FIELD(String, PrinterName, "Default Printer");

	
public:
   /**
	* 
	* Craete Object Device Printer
	* @attribute PrinterName 
	*/
	DevicePrinter() {
		this->PrinterName = currentNamePrinter();
	}

	const char* NOT_AVAILABLE_DEVICE = "Not available device";

   /**
	* Return Current Printer set default 
	* 
	* 
	*/
	oatpp::String currentNamePrinter() {
		String strDefualtPrinterName;
		DWORD sizeBuffer = 0;

		GetDefaultPrinter(NULL, &sizeBuffer);
		char* defaultPrinterName = new char[sizeBuffer];
		bool result = FALSE;
		if (GetDefaultPrinter(defaultPrinterName, &sizeBuffer)) {
			strDefualtPrinterName =  std::string(defaultPrinterName);
		}
		else {
			strDefualtPrinterName = NOT_AVAILABLE_DEVICE;
		}
		delete[] defaultPrinterName;

		return strDefualtPrinterName;
	}
	

	bool getDefaultPrinter() {
		bool result = FALSE;
		if (this->currentNamePrinter() != NOT_AVAILABLE_DEVICE) {
			this->PrinterName = this->currentNamePrinter();
			result = TRUE;
		}
		return result;
	}

	bool setDefaultPrinter(String &_PrinterName) {
		OATPP_LOGD("Current Printer", "%s", this->currentNamePrinter()->data());

		String printer = _PrinterName->data();
		size_t index = 0;

		/* decode html \\ = \  */
		while ((index = printer->find("\\\\\\\\")) != std::string::npos) {
			printer->replace(index, 4, "\\\\");
		}
		index = 0;
		/* decode html %20 = CHARTER ESPACE   */
		while ((index = printer->find("%20")) != std::string::npos) {
			printer->replace(index, 3, " ");
		}
		index = 0;
		/* decode html %22 ="  */
		while ((index = printer->find("%22")) != std::string::npos) {
			printer->replace(index, 3, "");
		}
		index = 0;
		/* decode html %27 = '  */
		while ((index = printer->find("%27")) != std::string::npos) {
			printer->replace(index, 3, "");
		}

		OATPP_LOGD("Set Default Printer", "%s", printer->data());
		
		LPCSTR print = printer->c_str();
		bool result = SetDefaultPrinter(print);
		if (result == TRUE) {
			this->PrinterName = printer->data();
		}

		return result;
	}

};

class PrinterInfo2Dto : public oatpp::DTO {
	// WINAPI  struct	PRINTER_INFO_2 WINSPOOL
	DTO_INIT(PrinterInfo2Dto, DTO /* Extends */)
	DTO_FIELD(Int32, intId, "Id");		// Id Element
	DTO_FIELD(String, strServerName, "Server Name");		// pServerName
	DTO_FIELD(String, strPrinterName, "Printer Name");		// pPrinterName
	DTO_FIELD(String, strShareName, "Share Name");		//  pShareName
	DTO_FIELD(String, strPortName, "Port Name");		//	pPortName
	DTO_FIELD(String, strDriverName, "Driver Name");		//	pDriverName
	DTO_FIELD(String, strComment, "Comment");		//	pDriverName
	DTO_FIELD(String, strLocation, "Location");		//	pLocation
	DTO_FIELD(String, strSepFile, "Sep File");		//	pSepFile
	DTO_FIELD(String, strDatatype, "Data Type");		//	pDatatype
	DTO_FIELD(String, strParameters, "Parameters");		//	pParameters
	DTO_FIELD(Int32, strAttributes, "Attributes");		//	Attributes
	DTO_FIELD(Int32, strPriority, "Priority");		//	Priority
	DTO_FIELD(Int32, strDefaultPriority, "Default Priority");		//	DefaultPriority
	DTO_FIELD(Int32, strStartTime, "Start Time");		//	StartTime
	DTO_FIELD(Int32, strUntilTime, "Until Time");		//	UntilTime
	DTO_FIELD(Int32, strStatus, "Status");		//	Status
	DTO_FIELD(Int32, strcJobs, "cJobs");		//	cJobs
	DTO_FIELD(Int32, strAveragePPM, "Average PPM");		//	AveragePPM
};

class VectorPrinterInfo2dto : public oatpp::DTO {
	DTO_INIT(VectorPrinterInfo2dto, DTO /* Extends */)
		DTO_FIELD(List<Object<PrinterInfo2Dto>>, vector_devices, "Printer Info 2");     // Vector of Object PrinterInfo2Dto

public:
	void getInfo() {
		DWORD flags = PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS;
		DWORD level = 2;
		PRINTER_INFO_2* printerInfo = nullptr;
		DWORD memorySize = 0;
		DWORD printerCount = 0;

		EnumPrinters(flags, nullptr, level, nullptr, 0, &memorySize, &printerCount);
		//if (memorySize < 1) return 0;

		BYTE* bytes = new BYTE[memorySize];
		if (EnumPrinters(flags, nullptr, level, bytes, memorySize, &memorySize, &printerCount)) {
			printerInfo = (PRINTER_INFO_2*)bytes;

			for (DWORD index = 0; index < printerCount; index++) {
				auto DevPrinter = PrinterInfo2Dto::createShared();
				DevPrinter->intId = (int)index;
				DevPrinter->strServerName = printerInfo->pServerName;
				DevPrinter->strPrinterName = printerInfo->pPrinterName;
				DevPrinter->strShareName = printerInfo->pShareName;
				DevPrinter->strPortName = printerInfo->pDriverName;
				DevPrinter->strDriverName = printerInfo->pDriverName;
				DevPrinter->strComment = printerInfo->pComment;
				DevPrinter->strLocation = printerInfo->pLocation;
				DevPrinter->strSepFile = printerInfo->pSepFile;
				DevPrinter->strDatatype = printerInfo->pDatatype;
				DevPrinter->strParameters = printerInfo->pParameters;
				DevPrinter->strAttributes = printerInfo->Attributes;
				DevPrinter->strPriority = printerInfo->Priority;
				DevPrinter->strDefaultPriority = printerInfo->DefaultPriority;
				DevPrinter->strStartTime = printerInfo->StartTime;
				DevPrinter->strUntilTime = printerInfo->UntilTime;
				DevPrinter->strStatus = printerInfo->Status;
				DevPrinter->strcJobs = printerInfo->cJobs;
				DevPrinter->strAveragePPM = printerInfo->AveragePPM;
				this->vector_devices->push_back(DevPrinter);
				printerInfo++;
			}
		}
	}
};

class PrinterInfo4Dto : public oatpp::DTO {
	DTO_INIT(PrinterInfo4Dto, DTO /* Extends */)
	DTO_FIELD(Int32, intId, "Id");		// Id Element
	DTO_FIELD(String, strServerName, "Server Name");		// pServerName
	DTO_FIELD(String, strPrinterName, "Printer Name");		// pPrinterName
	DTO_FIELD(Int32, strAttributes, "Attributes");		//	Attributes
};

class VectorPrinterInfo4dto : public oatpp::DTO {
	DTO_INIT(VectorPrinterInfo4dto, DTO /* Extends */)
	DTO_FIELD(List<Object<PrinterInfo4Dto>>, vector_devices, "Printer Info 4");     // Vector of Object PrinterInfo4Dto

public:
	void getInfo() {
		DWORD flags = PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS;
		DWORD level = 4;
		PRINTER_INFO_4* printerInfo = nullptr;
		DWORD memorySize = 0;
		DWORD  printerCount = 0;

		EnumPrinters(flags, nullptr, level, nullptr, 0, &memorySize, &printerCount);
		//if (memorySize < 1) return 0;

		BYTE* bytes = new BYTE[memorySize];
		if (EnumPrinters(flags, nullptr, level, bytes, memorySize, &memorySize, &printerCount)) {
			printerInfo = (PRINTER_INFO_4*)bytes;
			for (DWORD index = 0; index < printerCount; index++) {
				auto DevPrinter = PrinterInfo4Dto::createShared();
				DevPrinter->intId = (int)index;
				DevPrinter->strPrinterName = printerInfo->pPrinterName;
				DevPrinter->strServerName = printerInfo->pServerName;
				DevPrinter->strAttributes = printerInfo->Attributes;
				this->vector_devices->push_back(DevPrinter);
				printerInfo++;
			}
		}
	}
};

class PrintText : public oatpp::DTO {
	DTO_INIT(PrintText, DTO)
	DTO_FIELD(Fields<String>, text_to_print, "Text to Print");
};

class PrintJPG : public oatpp::DTO {
	DTO_INIT(PrintJPG, DTO)
		DTO_FIELD(String, fileName, "File Name");
public:
	void loadPicture() {
	}

};


#include OATPP_CODEGEN_END(DTO)

#endif /** MyDTOs_hpp */