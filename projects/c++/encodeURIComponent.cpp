/*
 *   https 协议, 客户端 url 包含某些特殊字符, 可能造成歧义, 导致服务端无法准确识别。
 *   例如 : & 在协议中用于分隔两个字段, 当字段值包含 & 时, 就造成了歧义。
 *   
 *   编码规则 :
 *       百分号和字符的 ascii 码十六进制形式构成的字符串
 *       比如 : & 的 ascii 码是 0x26, 编码后为 "%26"
 *
 *   在线验证工具:
 *       https://www.sojson.com/encodeurl.html
 */

#include <iostream>
//#include <stringstream>
#include <iomanip>        /* setiosflags */

using namespace std;

std::string encodeURIComponent(const std::string &in)
{
    std::stringstream ssUri;
    for (const char &c : in){
        //0-9A-Za-z-_.!~*'() all not need to encode
        if ((c >= 'a' && c <= 'z') 
                || (c >= 'A' && c <= 'Z') 
                || (c >= '0' && c <= '9')
                || c == '!' 
                || (c >= '\'' && c <= '*') 
                || c == '-' 
                || c == '.' 
                || c== '_' 
                || c== '~'){
            ssUri << c;            //也不会对这些ASCII标点符号进行编码
        }
        else{                      //由一个或多个十六进制的转义序列替换的。
            ssUri << "%";
            if ((c & 0xf0) == 0)
                ssUri << 0;
            /*
             *  (c & 0xff)   字符十进制 ascii 值
             *  std::hex     十六进制
             *  std::setiosflags(std::ios::uppercase)   大写
             */
            ssUri << std::setiosflags(std::ios::uppercase) << std::hex << (c & 0xff);
        }
    }
    return ssUri.str();
}

std::string decodeURIComponent(const std::string &in)
{
    int               readFlag = 0;
    int               code     = 0;
    std::stringstream ssUri;
    for (const char &c : in){
        if ((readFlag == 0) && (c == '%')){
            readFlag = 1;
            continue;
        }
        else if (readFlag == 1){
            if (isxdigit(c)){
                if (isdigit(c))
                    code = c - '0';
                else if (c >= 'A' && c <= 'F')
                    code = (0x0a+(c - 'A'));
                else
                    return std::string();
                readFlag = 2;
                continue;
            }
            else{
                return std::string();
            }
        }
        else if (readFlag == 2){
            if (isxdigit(c)){
                code <<= 4;
                if (isdigit(c))
                    code |= (c - '0');
                else if (c >= 'A' && c <= 'F')
                    code |= (0x0a+(c - 'A'));
                else
                    return std::string();
                ssUri << (char) (code & 0xff);
                code     = 0;
                readFlag = 0;
                continue;
            }
            else{
                return std::string();
            }
        }
        else{
            ssUri << c;
        }
    }
    return ssUri.str();
}

int main(int argc, char *argv[])
{
    std::string in = "&";

    std::string out = encodeURIComponent(in);

    cout << out << endl;
    cout << decodeURIComponent(out) << endl;

    return 0; 
}
