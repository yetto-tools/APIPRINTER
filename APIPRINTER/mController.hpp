#ifndef mController_hpp
#define mController_hpp


#pragma warning(disable : 4996)
#include "ApiDTOs.hpp"

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"
#include "oatpp/core/data/stream/FileStream.hpp"


#include "oatpp/web/mime/multipart/FileProvider.hpp"
#include "oatpp/web/mime/multipart/InMemoryDataProvider.hpp"
#include "oatpp/web/mime/multipart/PartList.hpp"
#include "oatpp/web/mime/multipart/Reader.hpp"

#include <process.h>


#include OATPP_CODEGEN_BEGIN(ApiController) ///< Begin Codegen

namespace multipart = oatpp::web::mime::multipart;

/**
 * Sample Api Controller.
 */
class MyController : public oatpp::web::server::api::ApiController {
public:
    /**
     * Constructor with object mapper.
     * @param objectMapper - default object mapper used to serialize/deserialize DTOs.
     */
    MyController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
        : oatpp::web::server::api::ApiController(objectMapper)
    {}
public:
    ADD_CORS(Root)
        ENDPOINT("GET", "/", Root, REQUEST(std::shared_ptr<IncomingRequest>, request))
    {
        auto printer = DevicePrinter::createShared();
        std::string html_template = "";

        auto response = ResponseFactory::createResponse(Status::CODE_200, html_template);


        response->putHeader(Header::CONTENT_TYPE, "text/html");

        return response;
    };

    ADD_CORS(Hello)
        ENDPOINT("GET", "/hello", Hello)
    {
        auto msg = MessageDto::createShared();
        msg->statusCode = 200;
        msg->message = Header::Value::SERVER; //"Api Printer on Ready";
        auto response = createDtoResponse(Status::CODE_200, msg);
        return response;
    };

    ADD_CORS(echoDefaultPrinter)
        ENDPOINT("GET", "getDefaultPrinter", echoDefaultPrinter)
    {

            auto printer = DevicePrinter::createShared();

            if (printer->getDefaultPrinter()) {
                return createDtoResponse(Status::CODE_200, printer);
            }
            else{
                printer->PrinterName = printer->NOT_AVAILABLE_DEVICE;
                return createDtoResponse(Status::CODE_404, printer)  ;
            }

    };

    ADD_CORS(GETSetDefaultPrinter)
        ENDPOINT("GET", "/setDefaultPrinter", GETSetDefaultPrinter)
    {
        auto msg = MessageDto::createShared();

        msg->statusCode = 405;
        msg->message = "Method Not Allowed, used method POST";
        return createDtoResponse(Status::CODE_405, msg);
    };


    ADD_CORS(POSTSetDefaultPrinter)
        ENDPOINT("POST", "setDefaultPrinter{set}", POSTSetDefaultPrinter, REQUEST(std::shared_ptr<IncomingRequest>, request), PATH(oatpp::String, params, "set"))
    {
        
        oatpp::String tail = request->getPathTail();
        OATPP_ASSERT_HTTP(tail, Status::CODE_400, "null param set name printer");
        
        auto queryParams = oatpp::network::Url::Parser::parseQueryParams(oatpp::String(tail));
        
        auto queryParameter = queryParams.get("set");
        
        //OATPP_ASSERT(queryParameter->);
        auto printer = DevicePrinter::createShared();

        printer->getDefaultPrinter();

       // PrinterName = queryParameter;

        if (printer->setDefaultPrinter(queryParameter)) {
            return createDtoResponse(Status::CODE_200, printer);
        }
        else {
            printer->PrinterName = printer->NOT_AVAILABLE_DEVICE;
            return createDtoResponse(Status::CODE_404, printer);
        }

        return createDtoResponse(Status::CODE_200, printer);
        
        //auto printer = DefaultPrinter::createShared();
        //if (printer->setDefaultPrinter()) {
        //    return createDtoResponse(Status::CODE_200, printer);
        //}
        //else {
        //    printer->PrinterName = "ERROR_FILE_NOT_FOUND";
        //    return createDtoResponse(Status::CODE_404, printer);
        //}

    };


    ADD_CORS(EchoPrinterInfo2)
    ENDPOINT("GET", "printerInfo2", EchoPrinterInfo2)
    {
            auto VectorDevPrinter = VectorPrinterInfo2dto::createShared();
            VectorDevPrinter->vector_devices = {};
            VectorDevPrinter->getInfo();
            return createDtoResponse( Status::CODE_200, VectorDevPrinter);
        
    };

    ADD_CORS(EchoPrinterInfo4)
    ENDPOINT("GET", "printerInfo4", EchoPrinterInfo4)
    {
            auto VectorDevPrinter = VectorPrinterInfo4dto::createShared();
            VectorDevPrinter->vector_devices = {};
            VectorDevPrinter->getInfo();
            auto response = createDtoResponse(Status::CODE_200, VectorDevPrinter);
            return response;
    };

    ADD_CORS(uploadPost)
    ENDPOINT("POST", "upload", uploadPost, REQUEST(std::shared_ptr<IncomingRequest>, request))
    {
        oatpp::data::stream::FileOutputStream fileOutputStream("C:\\Temp\\t.txt");
        
        request->transferBodyToStream(&fileOutputStream);
      
        auto response = createDtoResponse(Status::CODE_200, oatpp::Fields<oatpp::Any>({
            {"code", oatpp::Int32(200)},
            { "message", oatpp::String("OK") },
            { "size", oatpp::Int32(sizeof(fileOutputStream)) },
            { "file-data",nullptr /* no file data */}
            }));

        return response;
  
    };

    ADD_CORS(POSTTextPrint)
    ENDPOINT("POST", "textPrint", POSTTextPrint, REQUEST(std::shared_ptr<IncomingRequest>, request) )
    {
        
        auto formData = request->readBodyToString();
        const auto queryParams = oatpp::network::Url::Parser::parseQueryParams("?" + formData);

        //oatpp::String summary = "data=" + formData + "\n\n";
        oatpp::String summary;
        for (const auto& p : queryParams.getAll()) {
            summary = summary + p.first.toString() + "='" + p.second.toString() + "'\n";
        }

        return createDtoResponse(Status::CODE_200, summary);

    };


    ADD_CORS(uploadFile)
    ENDPOINT("POST", "file", uploadFile, REQUEST(std::shared_ptr<IncomingRequest>, request)) 
    {

        std::shared_ptr<oatpp::data::stream::BufferOutputStream> m_bufferStream =
            std::make_shared<oatpp::data::stream::BufferOutputStream>();
        
        request->transferBodyToStream(m_bufferStream);

        m_bufferStream->getData();
        
        OATPP_LOGD("getData", "%s", request->getHeader("content-type")->c_str());

        return ResponseFactory::createResponse(Status::CODE_200, "ok");

    }

    ADD_CORS(test)
        ENDPOINT("POST", "test", test, BODY_DTO(oatpp::Object<PrintJPG>, file))
    {
        //auto co = file->statusCode;
        //auto msg = file->message;
        //OATPP_LOGD("FILE",  "%s",msg->c_str());
        //return createDtoResponse(Status::CODE_200, file);

        auto infile = file->fileName;

        return ResponseFactory::createResponse(Status::CODE_200, "OK");
    }

    ADD_CORS(testjson,"*", "POST, OPTIONS", "DNT, User-Agent, X-Requested-With, If-Modified-Since, Cache-Control, Content-Type, Range","1728000")
        ENDPOINT("POST", "testjson", testjson,REQUEST(std::shared_ptr<IncomingRequest>, request))
    {      
      
        OATPP_LOGD("content-type", "%s", "asdf");

        return ResponseFactory::createResponse(Status::CODE_200, "OK");
    }

    ADD_CORS(GET_PDF)
    ENDPOINT("GET", "pdf", GET_PDF, REQUEST(std::shared_ptr<IncomingRequest>, request)) {
        std::string  html_body = \
            "<!DOCTYPE html>"
            "<html>"
            "<head>"
            "<meta charset = 'utf-8'>"
            "<meta name = 'viewport' content = 'width=device-width, initial-scale=1'>"
            "<link rel = 'stylesheet' href = 'https://cdn.jsdelivr.net/npm/bootstrap@4.3.1/dist/css/bootstrap.min.css' integrity = 'sha384-ggOyR0iXCbMQv3Xipma34MD+dH/1fQ784/j6cY/iJTQUOhcWr7x9JvoRxT2MZw1T' crossorigin = 'anonymous'>"
            "<title></title>"
            "</head>"
            "<body>"
            "<div class='container'>"
            "<div class= 'row justify-content-center'>"
            "<div class='col-xs-1 center-block border mt-5 shadow-lg p-3 mb-5 bg-white rounded'>"
            "<div class='card-body'>"
            "<input class='form-control-file primary' type='file' id='fileUpload'/>"
            "</div>"
            "<div class='card-footer'>"
            "<h4><span class = 'badge bg-success' hidden>Archivo Recibido</span></h4>"
            "</div>"
            "</div>"
            "</div>"
            "</div>"
            "<script type = 'text/javascript'>"
            "document.querySelector('#fileUpload').addEventListener('change', event => {"
            "handleImageUpload(event)"
             "}); "

            "var mensaje = new Object();"

            "const handleImageUpload = event => {"
                "const files = event.target.files;"
                    "const formData = new FormData();"
                    "formData.append('myFile', files[0]);"

                "if (!document.querySelector('span[class=\"badge bg-success\"]').getAttribute('hidden')) {"
                        "document.querySelector('span[class=\"badge bg-success\"]').setAttribute('hidden', '');"
                    "}"

                "fetch('http://localhost:8000/pdf', {"
                  "method: 'POST',"
                  "body : formData"
                    "})"
                    ".then(response => response.json())"
                    ".then(data => {"
                    "document.querySelector(\"span[class='badge bg-success']\").removeAttribute('hidden');"
                        "mensaje = data;"
                        "console.log(mensaje);"
                        "alert(mensaje.information);"
                "})"
                   " .catch (error => {"
                        "console.error(error);"
                    "})"
            "}"
            "</script>"
;
        
        
        auto printer = DevicePrinter::createShared();
        auto response = ResponseFactory::createResponse(Status::CODE_200, html_body);
        response->putHeader(Header::CONTENT_TYPE, "text/html");
        return response;
    }



    ADD_CORS(POST_PDF)
    ENDPOINT("POST", "pdf", POST_PDF, REQUEST(std::shared_ptr<IncomingRequest>, request)) {
        
        oatpp::data::stream::FileOutputStream fileOutputStream("C:\\Temp\\file.pdf","wb");
        request->transferBodyToStream(&fileOutputStream); // transfer body chunk by chunk
        auto printer = DevicePrinter::createShared();
        printer->getDefaultPrinter();
        std::string print_name = "/p /h";
        print_name = printer->PrinterName->c_str();
        
        //ShellExecute(0, "print", "C:\\Temp\\file.pdf", print_name.c_str(), ".", SW_HIDE);
        //fileOutputStream.close();
        //system("timeout /t 3 & taskkill /F /IM Acrobat.exe");
        
        fileOutputStream.close();
        SHELLEXECUTEINFO ShExecInfo = { 0 };

        ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
        ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
        ShExecInfo.hwnd = NULL;
        ShExecInfo.lpVerb = "print";
        ShExecInfo.lpFile = "C:\\Temp\\file.pdf";;
        ShExecInfo.lpParameters = NULL;
        ShExecInfo.lpDirectory = ".";
        ShExecInfo.nShow = SW_HIDE;
        ShExecInfo.hInstApp = NULL;

        ShellExecuteEx(&ShExecInfo);

        unsigned int PID = 0;
        PID = GetProcessId(ShExecInfo.hProcess);

        std::string taskkill = "TASKKILL /F /PID  ";
        std::string pid_s = std::to_string(PID);
        std::string command = taskkill + pid_s;


        if (WaitForSingleObject(ShExecInfo.hProcess, 2800) == WAIT_OBJECT_0) {//INFINITE) == ) {
            //ShellExecute(0, "open", "C:\\Windows\\System32\\cmd.exe", command.c_str(), NULL, SW_HIDE);
            system(command.c_str());
            OATPP_LOGD("PID ID", "%s", command.c_str());
        }
        else{
            OATPP_LOGD("lost pid id ", "%s", "No se que pedo");
        }

       
        





        //char buffer[32];
        //ltoa((long)PID, buffer, 10);
        //std::string cmd = "timeout /t 5 && taskkill /F /PID  ";
        //std::string pid_s(buffer);
        //std::string kill = cmd + pid_s;
         
        //system(kill.c_str());
        
        //OATPP_LOGD("RETURN ", "%s", kill.c_str());
        //system(kill.c_str());

        //while (WaitForSingleObject(ShExecInfo.hProcess, INFINITE)) {
        //    OATPP_LOGD("Wait", "%s", "...");
        //}

        //
        auto result = MessageDto::createShared();
        result->statusCode = 200;
        result->message = "Imprimiendo..."; //"Api Printer on Ready";
        
        return createDtoResponse(Status::CODE_200, result);
        
    }
    
    
    
    // TODO Insert Your endpoints here !!!

};

#include OATPP_CODEGEN_END(ApiController) ///< End Codegen

#endif /** MyController_hpp */