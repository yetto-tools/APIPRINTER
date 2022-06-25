#pragma once
#ifndef HtmlTemplate_hpp
#define HtmlTemplate_hpp

namespace HtmlTemplate{
	class Html
	{
		public:

            static constexpr const char* form_post =
                "<!DOCTYPE html>"
                "<html lang=\"es\">"
                "<head>"
                "<meta charset=\"utf-8\"/>"
                "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
                "<link rel=\"stylesheet\" href=\"https://cdn.jsdelivr.net/npm/bootstrap@4.3.1/dist/css/bootstrap.min.css\" integrity=\"sha384-ggOyR0iXCbMQv3Xipma34MD+dH/1fQ784/j6cY/iJTQUOhcWr7x9JvoRxT2MZw1T\" crossorigin=\"anonymous\">"
                "<title>Test Print Document PDF</title>"
                "</head>"
                "<body>"
                "<div class=\"parent\">"
                "<h1>Web Plugin Printer:</h1>"
                "</div>"
                "<div class=\"container\">"
                "<div class=\"row justify-content-center\">"
                "<div class=\"col-xs-1 center-block border mt-5 shadow-lg p-3 mb-5 bg-white rounded\">"
                "<div class=\"card-body\">"
                "<input class=\"form-control-file primary\" type=\"file\" id=\"fileUpload\">"
                "<select id='printers_installed' >"
                "<option id='0' value = '0'> --Seleccione una impresora-- </option>"
                "</select>"
                "<input id=\"numbercopies\" type=\"number\" min=\"1\" value=\"1\">"
                "</div>"
                "<div class=\"card-footer\">"
                "<h4><span class=\"badge bg-success\" hidden>Archivo Recibido e Imprimiendo...</span></h4>"
                "</div>"
                "</div>"
                "</div>"
                "</div>"
                "<div id=\"footer\"><b>2020-2022 &copy; Erick Rashon</b></div>"
                "<script type=\"text/javascript\">"
                "fetch('http://127.0.0.1:8000/printerInfo4').then(function(response) {"
                "if(response.ok) {"
                "response.json().then("
                "data => {"
                "let el_select = document.querySelector('#printers_installed');"
                "for (let index in data['Printer Info 4']){"
                "let opt = document.createElement('option');"
                "opt.value = data['Printer Info 4'][index].Printer_Name;"
                "opt.innerHTML = data['Printer Info 4'][index].Printer_Name;"
                "el_select.appendChild(opt);"
                "}"
                "});"

                "} else {"
                "console.log('Respuesta de red OK pero respuesta HTTP no OK');"
                "}"
                "})"
                ".catch(function(error) {"
                "console.log('Hubo un problema con la petición Fetch:' + error.message);"
                "});"
                
                "function hidden_status() {"
                "if (!document.querySelector('span[class=\"badge bg-success\"]').getAttribute(\"hidden\")) {"
                    "document.querySelector('span[class=\"badge bg-success\"]').setAttribute(\"hidden\", '');"
                "}"
                "}"
                "document.querySelector('#fileUpload').addEventListener('change', event => {"
                "handleImageUpload(event)"
                "});"
                "var status = new Object();"

                "const handleImageUpload = event => {"
                "const files = event.target.files;"
                "const formData = new FormData();"
                "formData.append('myFile', files[0]);"
                "let printername = document.querySelector(\"#printers_installed\").selectedOptions[0].value;"
                "let numbercopies = document.querySelector(\"#numbercopies\").value;"
                 "console.log(\"http://127.0.0.1:8000/pdf?printto=\"+ printername +\"&numbercopies=\"+numbercopies);"   
                "fetch(\"http://127.0.0.1:8000/pdf?printto=\"+ printername +\"&numbercopies=\"+numbercopies, {"
                    "method: 'POST',"
                    "body : formData"
                 "})"
                ".then(response => response.json())"
                ".then(data => {"
                    "document.querySelector('span[class=\"badge bg-success\"]').removeAttribute(\"hidden\");\n"
                    "setTimeout(hidden_status, 3000);\n"
                    "console.log(data);"
                    "document.querySelector('#fileUpload').value=''; "
                "})"
                ".catch (error => {"
                    "console.error(error);"
                "});"
                "}"
                "</script>"
                "<style>"
                "body{" //body
                "height: 100%;"
                "font-family:  Arial, Helvetica, sans-serif;"
                "}" //end body
                ".parent {"
                "height: 200px;"
                "backgroupd: #CCCCCC;"
                "display: flex;"
                "align-items: center;"
                "justify-content:cneter;"
                "}" //parent
                ".child{" // child
                "background-color: #FFFF00;"
                "width: 100px;"
                "height: 100px;"
                "}" // end child
                "#footer {" // footer
                "position:fixed;"
                "left:0px;"
                "bottom:0px;"
                "height:30px;"
                "width:100%;"
                "background:#999;"
                "}" //end footer
                "</style>"
                "</body>"
                "</html>"               
                 "";

            static constexpr const char* root =
                "<!DOCTYPE html>"
                "<html lang=\"es\">"
                "<head>"
                "<meta charset=\"utf-8\"/>"
                "<title>Web Plugin Printer </title>"
                "</head>"
                "<body>"
                "<main>"
                "<h1>Web Plugin Printer:</h1>"
                "<div class=\"parent\">"
                "<div><h4>It's Work!!</4></div>"
                "<div id=\"footer\"><b>2020-2022 &copy; Erick Rashon</b></div>"
                "</div>"
                "<main>"
                "<style>"
                "body{" //body
                "height: 100%;"
                "font-family:  Arial, Helvetica, sans-serif;"
                "}" //end body
                ".parent {"
                "height: 200px;"
                "backgroupd: #CCCCCC;"
                "display: flex;"
                "align-items: center;"
                "justify-content:cneter;"
                "}" //parent
                ".child{" // child
                "background-color: #FFFF00;"
                "width: 100px;"
                "height: 100px;"
                "}" // end child
                "#footer {" // footer
                "position:fixed;"
                "left:0px;"
                "bottom:0px;"
                "height:30px;"
                "width:100%;"
                "background:#999;"
                "}" //end footer
                "</style>"
                "</body>"
                "</html>"
                "";

            static constexpr const char* printer =
                "<form action='/api/v1/form' method='POST' autocomplete='off'>"
                "<select>"
                "<option id='' value=''>Seleccione una impresora</option>"
                "</select>"
                "</form>"
                "";
	};

}


#endif //END PrinterControl



