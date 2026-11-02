//Copyright (C) 2010-2015 Atesion GmbH. All rights reserved.
#ifndef WILDCAT_GUI_BASE_STRING_H
#define WILDCAT_GUI_BASE_STRING_H

#include <Pt/Utf8Codec.h>
#include <Pt/Types.h>
#include <string>
#include <vector>
#include <sstream>
#include <locale>
#include <iomanip>
#include <inttypes.h>

namespace wildcat {
namespace gui {
namespace base {

/**@brief Typedef of a string vector.*/
typedef std::vector<std::string> StringVector;

/**@brief A string helper class.*/
class String
{
    public:
        /**@brief Return true if the string starts with the given string.
        *
        * @param [in] str The base string.
        * @param [in] with The string to compare.
        * @return True if the base string starts with the given string.*/
        template<typename T>
        static bool startsWith(const T& str, const T& with)
        {
            return str.find(with) == 0;
        }

        /**@brief Return true if the string ends with the given string.
        *
        * @param [in] str The base string.
        * @param [in] with The string to compare.
        * @return True if the base string ends by the given string.*/
        template<typename T>
        static bool endsWith(const T& str, const T& with)
        {
            if (str.length() < with.length())
                return false;

            return (0 == str.compare(str.length() - with.length(), with.length(), with));
        }

        /**@brief Return true if the string contains the given string.
        *
        * @param [in] str The base string.
        * @param [in] item The string to compare.
        * @return True if the base string contains the given string.*/
        template<typename T>
        static bool contains(const T& str, const T& item)
        {
            return str.find(item) != T::npos;
        }

        template<typename T>
        static bool contains(const T& str, const T& item, const T& sep)
        {               
            std::vector<T> items = split(item, sep);
            return contains(str, items);            
        }

        template<typename T>
        static bool contains(const T& str, const std::vector<T>& items)
        {
            for (size_t i = 0; i < items.size(); ++i)
            {
                if(!contains(str, items[i]))
                    return false;
            }

            return true;
        }

        /**@brief Return true if the string contains the given string.
        *
        * @param [in] str The base string.
        * @param [in] delim The delimiter.
        * @param [in] empty Do no skip empty and whitespace.
        * @return Array of the splitted strings.*/

        template<typename T>
        static std::vector<T> split(const T& str, const T& delim, bool empty = false)
        {
            std::vector<T> parts;

            if(!empty)
            {
                size_t start, end = 0;

                parts.clear();

                while (end < str.size())
                {
                    start = end;

                    while (start < str.size() && (delim.find(str[start]) != T::npos))
                        start++;  // skip initial whitespace

                    end = start;

                    while (end < str.size() && (delim.find(str[end]) == T::npos))
                        end++; // skip to end of word

                    if (end - start != 0)  // just ignore zero-length strings.
                        parts.push_back(T(str, start, end - start));
                }

                return parts;
            }
            else
            {
                T  line;
                for (typename T::const_iterator it = str.begin(); it != str.end(); ++it)
                {
                    if ((delim.find(*it) != T::npos))
                    {
                        parts.push_back(line);
                        line.clear();
                    }
                    else
                    {
                        line += *it;
                    }
                }

                parts.push_back(line);
            }
            return parts;
        }

   



        /**@brief Trim the given char from the start of the string.
        *
        * @param [in] str The base string.
        * @param [in] with The given char.
        * @return The trimmed string.*/
        template<typename T, typename C>
        static T trimStart(const T& str, C with)
        {
            T res = str;

            while (true)
            {
                if (res[0] != with)
                    break;

                res.erase(res.begin(), res.begin() + 1);
            }
            return res;
        }

        /**@brief Trim the given char from the end of the string.
        *
        * @param [in] str The base string.
        * @param [in] with The given char.
        * @return The trimmed string.*/
        template<typename T, typename C>
        static T trimEnd(const T& str, C with)
        {
            T res = str;

            while (true)
            {
                if (res.empty())
                    break;

                if (res[res.size() - 1] != with)
                    break;

                res.erase(res.end() - 1, res.end());
            }

            return res;
        }

        template<typename T, typename C>
        static T trim(const T& str,C with)
        {
            return trimStart(trimEnd(str, with), with);
        }

        /**@brief Convert the string to lower case.
        *
        * @param [in] str The base string.
        * @return The lower case string.*/        

        template<typename T>    
        static T toLower(const T& str)
        {
            T lowerString = "";
            std::locale loc;

            for (size_t i = 0; i < str.size(); ++i)
                lowerString += std::tolower((wchar_t)str[i], loc);

            return lowerString;
        }


        /**@brief Convert the string to upper case.
        *
        * @param [in] str The base string.
        * @return The upper case string.*/
        static std::string toUpper(const std::string& str)
        {
            std::string upperString = "";
            std::locale loc;

            for (size_t i = 0; i < str.size(); ++i)
                upperString += std::toupper(str[i], loc);

            return upperString;
        }

        /**@brief Replace a substring by the given string.
        *
        * @param [in] str The base string.
        * @param [in] from What to replace.
        * @param [in] with The replacemend string.
        * @return The replaced string.*/
        template<typename T>
        static T replace(const T& str, const T& from, const T& with)
        {
            size_t start_pos = 0;
            T strL = str;

            while ((start_pos = strL.find(from, start_pos)) != T::npos)
            {
                strL.replace(start_pos, from.length(), with);
                start_pos += with.length();
            }

            return strL;
        }


        static std::string read(std::istream& stream, char term = '\0')
        {
            char ch;

            std::string str;
            stream.read(&ch, 1);

            while (ch != term && stream)
            {
                str += ch;
                stream.read(&ch, 1);
            }

            return str;
        }

        static std::wstring readW(std::istream& stream,char term = '\0')
        {            
            wchar_t ch;

            std::wstring str;
            stream.read((char*)&ch, 2);

            while (ch != 0 && stream)
            {
                str += ch;
                stream.read((char*)&ch, 2);
            }

            return str;
        }

        static Pt::String readPt(std::istream& stream, Pt::Char term = '\0')
        {
            uint32_t ch;

            Pt::String str;
            stream.read((char*)&ch, 4);

            while (ch != 0 && stream)
            {
                str += ch;
                stream.read((char*)&ch, 4);
            }

            return str;
        }

        static Pt::String readAllUtf8(std::istream& stream)
        {            
            std::string str = readAll(stream);
            
            return  fromUtf8(str);
        }

        static std::string readAll(std::istream& stream)
        {
            char ch;

            std::string str;
            stream.read((char*)&ch, 1);

            while (stream)
            {
                str += ch;
                stream.read((char*)&ch, 1);
            }

            return  str;
        }

        static std::string read(const uint8_t* data, size_t size)
        {
            std::string str;

            if (size == 0)
                return str;

            for (size_t i = 0; i < size; ++i)
            {
                if (data[i] == 0)
                    return str;

                str += (char)data[i];
            }

            return str;
        }

        static int write(std::ostream& stream, const std::string& str, char term)
        {
            if (!str.empty())
                stream.write(str.c_str(), str.size());

            stream.write(&term, 1);

            return (int)str.size() + 1;
        }

        static int write(std::ostream& stream, const std::string& str)
        {
            if(!str.empty())
                stream.write(str.c_str(), str.size());

            return (int)str.size() + 1;
        }

        static int write(std::ostream& stream, const std::wstring& str)
        {
            wchar_t ch = '\0';

            stream.write((const char*)str.c_str(), str.size());
            stream.write((const char*)&ch, 2);
            return (int)str.size() + 1;
        }

        static int write(std::ostream& stream, const Pt::String& str)
        {
            for(size_t i = 0; i < str.size(); ++i)
            {
                const Pt::Char ch = str[i];
                const uint32_t value = ch.value();    
                stream.write((const char*)&value , 4);
            }   

            return (int)(str.size() + 1)* 4; 
        }

        static int writeUtf8(std::ostream& stream, const Pt::String& str)
        {
            std::string s = Pt::Utf8Codec::encode(str);

            if (!s.empty())
                stream.write(s.c_str(), str.size());

            return (int)s.size();   
        }



        /**@brief Erase a sub string.
        *
        * @param[in] str The main string,
        * @param[in] toErase The string to find and erase.
        * @return The new ersaed string.*/
        static std::string erase(const std::string& str, const std::string& toErase)
        {
            std::string res = str;

            // Search for the substring in string
            size_t pos = res.find(toErase);

            if (pos != std::string::npos)
            {
                // If found then erase it from string
                res.erase(pos, toErase.length());
            }

            return res;
        }

        /**@brief Convert to string.
        *
        * @param [in] no The value to convert.
        * @return The string.*/
        template<typename T>
        static std::string from(T no)
        {
            std::string s;
            std::stringstream ss;
            ss << no;
            ss >> s;
            return s;
        }

        template<typename T>
        static std::string fromHex(T no, bool upperCase = false)
        {
            std::string s;
            std::stringstream ss;

            if(upperCase)
                ss<< std::uppercase;

            ss <<std::hex<< no;
            ss >> s;
            return s;
        }


        /**@brief Convert from string.
          *
          * @param [in] s The string.
          * @return The value.*/
        template<typename T>
        static T to(const std::string& s)
        {
            if (startsWith<std::string>(s, "#x") || startsWith<std::string>(s, "0x")|| startsWith<std::string>(s, "#"))
                return toHex<T>(s);

            T value;
            std::stringstream ss;
            ss << s;
            ss >> value;
            return value;
        }


        /**@brief Convert from hex string.
        *
        * @param [in] s The string.
        * @return The value.*/
        template<typename T>
        static T toHex(const std::string& s)
        {
            std::string data = replace<std::string>(s, "#x", "");

            data = replace<std::string>(s, "#", "");

            data = replace<std::string>(data, "0x", "");

            T value;
            std::stringstream ss;
            ss << data;
            ss >> std::hex >> value;
            return value;
        }

        /**@brief Convert the string to UTF8
        *
        * @param [in] str The base string.
        * @return The UTF8 string.*/
        static std::string toUtf8(const Pt::String& str)
        {
            return Pt::Utf8Codec::encode(str);
        }

        /**@brief Decode a UTF8 string
        *
        * @param [in] str The UTF8 string.
        * @return The encoded unicode string.*/
        static Pt::String fromUtf8(const std::string& str)
        {
            return Pt::Utf8Codec::decode(str);
        }

        /**@brief Convert a byte stream to base 64 string.
        *
        * @param[in] input The byte stream.
        * @param[in] size The size of the byte stream
        * @return The base 64 string.*/
        static std::string toBase64(const uint8_t* input, size_t size)
        {
            static const char base64Chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
            static const char padCharacter = '=';

            std::string encodedString;
            encodedString.reserve(((size / 3) + (size % 3 > 0)) * 4);

            uint32_t temp{};
            const uint8_t* it = &input[0];

            for (std::size_t i = 0; i < size / 3; ++i)
            {
                temp = (*it++) << 16;
                temp += (*it++) << 8;
                temp += (*it++);
                encodedString.append(1, base64Chars[(temp & 0x00FC0000) >> 18]);
                encodedString.append(1, base64Chars[(temp & 0x0003F000) >> 12]);
                encodedString.append(1, base64Chars[(temp & 0x00000FC0) >> 6]);
                encodedString.append(1, base64Chars[(temp & 0x0000003F)]);
            }

            switch (size % 3)
            {
                case 1:
                    temp = (*it++) << 16;
                    encodedString.append(1, base64Chars[(temp & 0x00FC0000) >> 18]);
                    encodedString.append(1, base64Chars[(temp & 0x0003F000) >> 12]);
                    encodedString.append(2, padCharacter);
                break;

                case 2:
                    temp = (*it++) << 16;
                    temp += (*it++) << 8;
                    encodedString.append(1, base64Chars[(temp & 0x00FC0000) >> 18]);
                    encodedString.append(1, base64Chars[(temp & 0x0003F000) >> 12]);
                    encodedString.append(1, base64Chars[(temp & 0x00000FC0) >> 6]);
                    encodedString.append(1, padCharacter);
                break;
            }

            return encodedString;
        }

        static void toBase64(const uint8_t* input, size_t size, std::ostream& os)
        {
            static const char base64Chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
            static const char padCharacter = '=';

            uint32_t temp{};
            const uint8_t* it = &input[0];

            for (std::size_t i = 0; i < size / 3; ++i)
            {
                temp = (*it++) << 16;
                temp += (*it++) << 8;
                temp += (*it++);

                os.put(base64Chars[(temp & 0x00FC0000) >> 18]);
                os.put(base64Chars[(temp & 0x0003F000) >> 12]);
                os.put(base64Chars[(temp & 0x00000FC0) >> 6]);
                os.put(base64Chars[(temp & 0x0000003F)]);
            }

            switch (size % 3)
            {
                case 1:
                    temp = (*it++) << 16;
                    os.put(base64Chars[(temp & 0x00FC0000) >> 18]);
                    os.put(base64Chars[(temp & 0x0003F000) >> 12]);
                    os.put(padCharacter);
                    os.put(padCharacter);
                break;

                case 2:
                    temp = (*it++) << 16;
                    temp += (*it++) << 8;
                    os.put(base64Chars[(temp & 0x00FC0000) >> 18]);
                    os.put(base64Chars[(temp & 0x0003F000) >> 12]);
                    os.put(base64Chars[(temp & 0x00000FC0) >> 6]);
                    os.put(padCharacter);
                break;
            }
        }

        static void toBase64(std::istream& is, std::ostream& os)
        {
            static const char base64Chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
            static const char padCharacter = '=';

            is.seekg(0, std::ios::end);

            size_t size = is.tellg();

            is.seekg(0, std::ios::beg);

            uint32_t temp{};
            uint32_t it = is.get();

            for (std::size_t i = 0; i < size / 3; ++i)
            {
                temp = it << 16;
                it = is.get();

                temp += it << 8;
                it = is.get();

                temp += it;
                it = is.get();

                os.put(base64Chars[(temp & 0x00FC0000) >> 18]);
                os.put(base64Chars[(temp & 0x0003F000) >> 12]);
                os.put(base64Chars[(temp & 0x00000FC0) >> 6]);
                os.put(base64Chars[(temp & 0x0000003F)]);
            }

            switch (size % 3)
            {
            case 1:
                temp = it << 16;
                it = is.get();
                os.put(base64Chars[(temp & 0x00FC0000) >> 18]);
                os.put(base64Chars[(temp & 0x0003F000) >> 12]);
                os.put(padCharacter);
                os.put(padCharacter);
                break;

            case 2:
                temp = it << 16;
                it = is.get();

                temp += it << 8;
                it = is.get();

                os.put(base64Chars[(temp & 0x00FC0000) >> 18]);
                os.put(base64Chars[(temp & 0x0003F000) >> 12]);
                os.put(base64Chars[(temp & 0x00000FC0) >> 6]);
                os.put(padCharacter);
                break;
            }
        }

        /**@brief Decode a base 64 stream.
        *
        * @param[in] encodedString The base 64 string.
        * @return The byte stream.*/
        static std::vector<Pt::uint8_t> fromBase64(const std::string& encodedString)
        {
            std::vector<Pt::uint8_t> ret;

            static const std::string base64Chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                "abcdefghijklmnopqrstuvwxyz"
                "0123456789+/";

            int in_len = (int)encodedString.size();
            int i = 0;
            int j = 0;
            int in_ = 0;
            unsigned char charArray4[4], charArray3[3];

            while (in_len-- && (encodedString[in_] != '=') && isBase64Char(encodedString[in_]))
            {
                charArray4[i++] = encodedString[in_];
                in_++;

                if (i == 4)
                {

                    for (i = 0; i < 4; i++)
                        charArray4[i] = (unsigned char)base64Chars.find(charArray4[i]);

                    charArray3[0] = (charArray4[0] << 2) + ((charArray4[1] & 0x30) >> 4);
                    charArray3[1] = ((charArray4[1] & 0xf) << 4) + ((charArray4[2] & 0x3c) >> 2);
                    charArray3[2] = ((charArray4[2] & 0x3) << 6) + charArray4[3];

                    for (i = 0; (i < 3); i++)
                        ret.push_back(charArray3[i]);

                    i = 0;
                }
            }

            if (i)
            {
                for (j = i; j < 4; j++)
                    charArray4[j] = 0;

                for (j = 0; j < 4; j++)
                    charArray4[j] = (unsigned char)base64Chars.find(charArray4[j]);

                charArray3[0] = (charArray4[0] << 2) + ((charArray4[1] & 0x30) >> 4);
                charArray3[1] = ((charArray4[1] & 0xf) << 4) + ((charArray4[2] & 0x3c) >> 2);
                charArray3[2] = ((charArray4[2] & 0x3) << 6) + charArray4[3];

                for (j = 0; (j < i - 1); j++)
                    ret.push_back(charArray3[j]);
            }

            return ret;
        }

        static std::vector<Pt::uint8_t> fromBase64(std::istream& stream)
        {
            std::vector<Pt::uint8_t> ret;

            static const std::string base64Chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                "abcdefghijklmnopqrstuvwxyz"
                "0123456789+/";


            stream.seekg(0, std::ios::end);

            int in_len = (int)stream.tellg();

            stream.seekg(0, std::ios::beg);

            int i = 0;
            int j = 0;
            unsigned char charArray4[4], charArray3[3];

            while (in_len-- && (stream.peek() != '=') && isBase64Char(stream.peek()))
            {
                charArray4[i++] = stream.get();

                if (i == 4)
                {

                    for (i = 0; i < 4; i++)
                        charArray4[i] = (unsigned char)base64Chars.find(charArray4[i]);

                    charArray3[0] = (charArray4[0] << 2) + ((charArray4[1] & 0x30) >> 4);
                    charArray3[1] = ((charArray4[1] & 0xf) << 4) + ((charArray4[2] & 0x3c) >> 2);
                    charArray3[2] = ((charArray4[2] & 0x3) << 6) + charArray4[3];

                    for (i = 0; (i < 3); i++)
                        ret.push_back(charArray3[i]);

                    i = 0;
                }
            }

            if (i)
            {
                for (j = i; j < 4; j++)
                    charArray4[j] = 0;

                for (j = 0; j < 4; j++)
                    charArray4[j] = (unsigned char)base64Chars.find(charArray4[j]);

                charArray3[0] = (charArray4[0] << 2) + ((charArray4[1] & 0x30) >> 4);
                charArray3[1] = ((charArray4[1] & 0xf) << 4) + ((charArray4[2] & 0x3c) >> 2);
                charArray3[2] = ((charArray4[2] & 0x3) << 6) + charArray4[3];

                for (j = 0; (j < i - 1); j++)
                    ret.push_back(charArray3[j]);
            }

            return ret;
        }

        static void fromBase64(std::istream& stream, std::ostream& os)
        {
            static const std::string base64Chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                "abcdefghijklmnopqrstuvwxyz"
                "0123456789+/";

            stream.seekg(0, std::ios::end);

            int in_len = (int)stream.tellg();

            stream.seekg(0, std::ios::beg);

            int i = 0;
            int j = 0;
            unsigned char charArray4[4], charArray3[3];

            while (in_len-- && (stream.peek() != '=') && isBase64Char(stream.peek()))
            {
                charArray4[i++] = stream.get();

                if (i == 4)
                {

                    for (i = 0; i < 4; i++)
                        charArray4[i] = (unsigned char)base64Chars.find(charArray4[i]);

                    charArray3[0] = (charArray4[0] << 2) + ((charArray4[1] & 0x30) >> 4);
                    charArray3[1] = ((charArray4[1] & 0xf) << 4) + ((charArray4[2] & 0x3c) >> 2);
                    charArray3[2] = ((charArray4[2] & 0x3) << 6) + charArray4[3];

                    for (i = 0; (i < 3); i++)
                        os.put(charArray3[i]);

                    i = 0;
                }
            }

            if (i)
            {
                for (j = i; j < 4; j++)
                    charArray4[j] = 0;

                for (j = 0; j < 4; j++)
                    charArray4[j] = (unsigned char)base64Chars.find(charArray4[j]);

                charArray3[0] = (charArray4[0] << 2) + ((charArray4[1] & 0x30) >> 4);
                charArray3[1] = ((charArray4[1] & 0xf) << 4) + ((charArray4[2] & 0x3c) >> 2);
                charArray3[2] = ((charArray4[2] & 0x3) << 6) + charArray4[3];

                for (j = 0; (j < i - 1); j++)
                    os.put(charArray3[j]);
            }
        }

                static std::string hexdump(const uint8_t* bytes, size_t size)
        {
            std::ostringstream buff;
            char c;

            for (size_t i = 0, j = 0; i < size; i += 16) 
            {
                // Out offset
                buff << std::right << std::setfill('0') << std::setw(4) << i << "  ";

                // Out bytes in hexadecimal
                buff << std::hex;
                for (j = 0; j < 16; j++) 
                {
                    if (i + j < size)
                    {
                        c = bytes[i + j];
                        buff << std::setfill('0') << std::setw(2) << (int(c) & 0xFF);
                        buff << " ";
                    }
                    else 
                    {
                        buff << "   ";
                    }
                }

                buff << std::dec;
                buff << " ";

                // Print printable characters
                for (j = 0; j < 16; j++) 
                {
                    if (i + j < size) 
                    {
                        c = bytes[i + j];

                        if (isprint(c)) 
                            buff << c;
                        else
                            buff << ".";
                    }
                }
                buff << std::endl;
            }

            return buff.str();
        }

        static std::vector<uint8_t> parseIp(const std::string& v)
        {
            base::StringVector arr = base::String::split<std::string>(v, ".");
            std::vector<uint8_t> data;

            for (size_t i = 0; i < arr.size(); ++i)
                data.push_back(base::String::to<int>(arr[i]));


            return data;
        }


        static std::string ipToString(const uint8_t* ip)
        {
            std::stringstream ss;

            ss << (uint16_t)ip[0] << "." << (uint16_t)ip[1] << "." << (uint16_t)ip[2] << "." << (uint16_t)ip[3];
            return ss.str();
        }

        static bool isEqualNoCase(const std::string& aUtf8, const std::string& bUtf8)
        {
            if( aUtf8.size() != bUtf8.size())
                return false;

            const Pt::String a1 = fromUtf8(aUtf8);
            const Pt::String b1 = fromUtf8(bUtf8);

            if(a1.size()  != b1.size())
                return false;

            for (size_t i = 0; i < a1.size(); ++i)
            {
                if( Pt::tolower(a1[i]) != Pt::tolower(b1[i]))
                    return false;
            }

            return true;
        }

    private:
        static inline bool isBase64Char(unsigned char c)
        {
            return (isalnum(c) || (c == '+') || (c == '/'));
        }
};

template<>
inline bool String::to<bool>(const std::string& s)
{
    if (s == "true")
        return true;

    if (s == "false")
        return false;

    if (s == "1")
        return true;

    return false;
}

template<>
inline std::string String::from<std::string>(std::string no)
{
    return no;
}

template<>
inline std::string String::to<std::string>(const std::string& s)
{
    return s;
}

template<>
inline std::string String::from<Pt::String>(Pt::String no)
{
    return String::toUtf8(no);
}

template<>
inline Pt::String String::to<Pt::String>(const std::string& s)
{
    return String::fromUtf8(s);
}


}}}

#endif
