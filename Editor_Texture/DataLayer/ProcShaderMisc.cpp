#include	"Editor_Texture/DataLayer/ProcShaderMisc.hpp"

namespace Misc{
std::string convertInt(int number)
{
   std::stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}

}
