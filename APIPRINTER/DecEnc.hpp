#pragma once
#ifndef DecEnc_hpp
#define DecEnc_hpp
#include <cctype>
#include <iomanip>
#include <sstream>
#include <string>

namespace UrlDecodecEncode {
	class UrlDecodeEncode {
    public:
        std::string url_encode(const std::string& value) {
            std::ostringstream escaped;
            escaped.fill('0');
            escaped << std::hex;

            for (std::string::const_iterator i = value.begin(), n = value.end(); i != n; ++i) {
                std::string::value_type c = (*i);

                // Keep alphanumeric and other accepted characters intact
                if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
                    escaped << c;
                    continue;
                }

                // Any other characters are percent-encoded
                escaped << std::uppercase;
                escaped << '%' << std::setw(2) << int((unsigned char)c);
                escaped << std::nouppercase;
            }

            return escaped.str();
        }

        char from_hex(char ch) {
            return isdigit(ch) ? ch - '0' : tolower(ch) - 'a' + 10;
        }

        std::string url_decode(std::string text) {
            char h;
            std::ostringstream escaped;
            escaped.fill('0');

            for (auto i = text.begin(), n = text.end(); i != n; ++i) {
                std::string::value_type c = (*i);

                if (c == '%') {
                    if (i[1] && i[2]) {
                        h = from_hex(i[1]) << 4 | from_hex(i[2]);
                        escaped << h;
                        i += 2;
                    }
                }
                else if (c == '+') {
                    escaped << ' ';
                }
                else {
                    escaped << c;
                }
            }
            return escaped.str();
        }
        /* Converts a hex character to its integer value */
        //char from_hex(char ch) {
        //    return isdigit(ch) ? ch - '0' : tolower(ch) - 'a' + 10;
        //}

        ///* Converts an integer value to its hex character*/
        //char to_hex(char code) {
        //    static char hex[] = "0123456789abcdef";
        //    return hex[code & 15];
        //}

        ///* Returns a url-encoded version of str */
        ///* IMPORTANT: be sure to free() the returned string after use */
        //char* url_encode(char* str) {
        //    char* pstr = str, * buf = malloc(strlen(str) * 3 + 1), * pbuf = buf;
        //    while (*pstr) {
        //        if (isalnum(*pstr) || *pstr == '-' || *pstr == '_' || *pstr == '.' || *pstr == '~')
        //            *pbuf++ = *pstr;
        //        else if (*pstr == ' ')
        //            *pbuf++ = '+';
        //        else
        //            *pbuf++ = '%', * pbuf++ = to_hex(*pstr >> 4), * pbuf++ = to_hex(*pstr & 15);
        //        pstr++;
        //    }
        //    *pbuf = '\0';
        //    return buf;
        //}

        ///* Returns a url-decoded version of str */
        ///* IMPORTANT: be sure to free() the returned string after use */
        //char* url_decode(char* str) {
        //    char* pstr = str, * buf = malloc(strlen(str) + 1), * pbuf = buf;
        //    while (*pstr) {
        //        if (*pstr == '%') {
        //            if (pstr[1] && pstr[2]) {
        //                *pbuf++ = from_hex(pstr[1]) << 4 | from_hex(pstr[2]);
        //                pstr += 2;
        //            }
        //        }
        //        else if (*pstr == '+') {
        //            *pbuf++ = ' ';
        //        }
        //        else {
        //            *pbuf++ = *pstr;
        //        }
        //        pstr++;
        //    }
        //    *pbuf = '\0';
        //    return buf;
        //}

	};
}


#endif