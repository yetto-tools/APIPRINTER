#ifndef MyController_hpp
#define MyController_hpp

#include "MyDTOs.hpp"
#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"
#include "oatpp/core/data/stream/FileStream.hpp"
#include "oatpp/core/data/resource/File.hpp"
/* --------Multipart------------- */
#include "oatpp/web/mime/multipart/InMemoryDataProvider.hpp"
#include "oatpp/web/mime/multipart/Reader.hpp"
#include "oatpp/web/mime/multipart/PartList.hpp"
#include "oatpp/core/utils/ConversionUtils.hpp"

/* --------Multipart------------- */
#include "PrinterControl.hpp"
#include "ToolsOs.hpp"
#include "DecEnc.hpp"
#include "HtmlTemplate.hpp"

#include "oatpp/core/utils/ConversionUtils.hpp"

namespace multipart = oatpp::web::mime::multipart;

#include OATPP_CODEGEN_BEGIN(ApiController) ///< Begin Codegen
  
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
        ENDPOINT("GET", "/", Root) {
        auto response = ResponseFactory::createResponse(Status::CODE_200, HtmlTemplate::Html::root);
        return response;
    }
    ADD_CORS(test)
        ENDPOINT("GET", "/test", test) {
        auto response = ResponseFactory::createResponse(Status::CODE_200, HtmlTemplate::Html::form_post);
        return response;
    }

    ADD_CORS(postForm)
    ENDPOINT("POST", "post-form", postForm, REQUEST(std::shared_ptr<IncomingRequest>, request))
    {
        /* prepare formdata to buffer */
        auto formData = request->readBodyToString();
        /* find paramas */
        const auto queryParams = oatpp::network::Url::Parser::parseQueryParams("?" + formData);
        oatpp::String summary = "data=[" + formData + "]\n\n";
        for (const auto& p : queryParams.getAll()) {
            summary = summary + p.first.toString() + "='" + p.second.toString() + "'\n";
        }
        return createResponse(Status::CODE_200, summary);
    }
    ADD_CORS(upload_response)
    ENDPOINT("GET", "upload", upload_response) {
        auto response = createResponse(Status::CODE_200, "ok");
        response->putHeader(Header::CONTENT_TYPE, "text/html");
        return response;
    }

    ADD_CORS(upload)
        ENDPOINT("POST", "upload", upload, REQUEST(std::shared_ptr<IncomingRequest>, request))
  {

        /*auto formData = request->readBodyToString();
        const auto queryParams = oatpp::network::Url::Parser::parseQueryParams(formData);
        */
        auto headers = request->getHeaders().getAll();
        std::string lenght;
        for (auto& h : headers) {    
            OATPP_LOGD("headers", "%s : %s", h.first.getData(), h.second.getData());

            if (h.first.std_str() == Header::CONTENT_LENGTH) {
                lenght = h.second.std_str();
            }
        }

        OATPP_LOGE("test", "lenght memory='%s'", lenght.c_str());
        //OATPP_LOGD("test", "user-agent='%s'", request->getHeader(Header::CONTENT_LENGTH)->data());
        return createResponse(Status::CODE_200, "ok");

    }

    ADD_CORS(multipart_test)
    ENDPOINT("POST", "multipart_test", multipart_test, REQUEST(std::shared_ptr<IncomingRequest>, request))
    {
        auto content_lenght = request->getHeader(Header::CONTENT_LENGTH)->data();
        OATPP_LOGD("CONTENT LENGHT", "%d", content_lenght);

        /* Prepare multipart container. */
        auto multipart = std::make_shared<multipart::PartList>(request->getHeaders());

        /* Create multipart reader. */
        multipart::Reader multipartReader(multipart.get());

        /* Configure to read part with name "part1" into memory */
        multipartReader.setPartReader("upload-file", multipart::createInMemoryPartReader(250000000 /* max-data-size */));

        /* Read multipart body */
        request->transferBody(&multipartReader);

        /* Print value of "part1" */
        auto part1 = multipart->getNamedPart("upload-file");

        /* Assert part is not null */
        OATPP_ASSERT_HTTP(part1, Status::CODE_400, "upload-file is null");

        /* Print part value */
        OATPP_LOGD("Multipart", "upload-file='%s'", part1->getFilename()->data());
        std::cout << part1->getHeader(Header::CONTENT_LENGTH).getValue("") << std::endl;

        return createResponse(Status::CODE_200, "OK");

    }
    ADD_CORS(printto_pdf)
        ENDPOINT("POST", "pdf", printto_pdf, REQUEST(std::shared_ptr<IncomingRequest>, request)) {

        oatpp::String tail = request->getPathTail();
        OATPP_ASSERT_HTTP(tail, Status::CODE_400, "null param set name printer");

        auto queryParams = oatpp::network::Url::Parser::parseQueryParams(oatpp::String(tail));

        auto printto = UrlDecodecEncode::UrlDecodeEncode().url_decode(queryParams.get("printto")->data());
        auto numberCopies = queryParams.get("numbercopies");

        String file_to_save = "file.pdf";
        oatpp::data::stream::FileOutputStream fileOutputStream(file_to_save->c_str());
        request->transferBodyToStream(&fileOutputStream);
        
        fileOutputStream.close();

        unsigned int number = oatpp::utils::conversion::strToInt32(numberCopies->c_str());
        String cmd;

        cmd = SystemUtilities::ProgressPrintFlow().call_process_printto(printto, file_to_save);

        bool exist_job;
        do {
            exist_job = PrinterControl::PrinterControl().GetJobs(printto);
            if (exist_job == false) {
                system(cmd->c_str());
            }
        } while (exist_job == true);


        auto result = MessageDto::createShared();
        result->statusCode = 200;
        result->message = "Imprimiendo... "+ printto; //"Api Printer on Ready";

        OATPP_LOGD("PRINTER", "%s", cmd->c_str());
        
        return createDtoResponse(Status::CODE_200, result);

    };
    ADD_CORS(post_pdf)
        ENDPOINT("POST", "pdf_print", post_pdf, REQUEST(std::shared_ptr<IncomingRequest>, request)) {

        auto printer = DevicePrinter::createShared();
        printer->getDefaultPrinter();
        String previous_printer;
        previous_printer = printer->currentNamePrinter()->c_str();

        String name = "C:\\Program Files\\WebPluginPrinter\\default.cfg";
        String ticket = SystemUtilities::Config().readLine(name);

        MessageBoxA(NULL, printer->currentNamePrinter()->c_str(), ticket->c_str(), MB_OK);
        if (!ticket->empty()) {
            printer->setDefaultPrinter(ticket);
        } 
       
        std::string file_name_pdf = "file.pdf";        
        std::string file_to_save = SystemUtilities::getPath().home() + file_name_pdf;
        
        OATPP_LOGD("file to save ", "%s", file_to_save.c_str());
        
        oatpp::data::stream::FileOutputStream fileOutputStream(file_to_save.c_str());
        request->transferBodyToStream(&fileOutputStream);
        
        fileOutputStream.close();

        OATPP_LOGD("to save", "%s", file_to_save.c_str());

        std::string cmd_kill = SystemUtilities::ProgressPrintFlow().call_process_print(file_to_save);
        
        auto result = MessageDto::createShared();

        bool exist_job;
        do {
            exist_job = PrinterControl::PrinterControl().GetJobs(previous_printer);
            if (exist_job == false) {
                system(cmd_kill.c_str());
            }
        }
        while (exist_job == true);

        printer->setDefaultPrinter(previous_printer);

        result->statusCode = 200;
        result->message = "Imprimiendo..."; //"Api Printer on Ready";

        return createDtoResponse(Status::CODE_200, result);

    };

    ADD_CORS(ConfigDefautlPrinter)
        ENDPOINT("GET", "config", ConfigDefautlPrinter) {
        std::string name = "C:\\Program Files\\WebPluginPrinter\\default.cfg";
        std::string data =  SystemUtilities::Config().readLine(name);
        OATPP_LOGD("DATA", "%s", data.c_str());

        return createResponse(Status::CODE_200, data.c_str());

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

    // TODO Insert Your endpoints here !!!

};

#include OATPP_CODEGEN_END(ApiController) ///< End Codegen

#endif /* MyController_hpp */