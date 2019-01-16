
#ifndef __LOGGER_HEADER__
#define __LOGGER_HEADER__

#include <string>
#include <cstdio>
#include <stdarg.h>

namespace luwu
{
	class Logger
	{
		public:
			static void Log(std::string message)
			{
				printf("%s\n",message.c_str());
			}

			static void Log(const char* format,...)
			{
				va_list args;
				va_start(args,format);
				vprintf(format,args);
				va_end(args);
			}

			static void Error(std::string message)
			{
				printf("%s\n",message.c_str());
			}

			static void Error(const char* format, ...)
			{
				va_list args;
				va_start(args,format);
				vprintf(format,args);
				va_end(args);
			}

			static void Fatal(std::string message)
			{
				printf("%s\n",message.c_str());
			}

			static void Fatal(const char* format, ...)
			{
				va_list args;
				va_start(args,format);
				vprintf(format,args);
				va_end(args);
			}
		private:

			Logger();
			~Logger();

			Logger(const Logger& other) = delete;
			void operator=(const Logger& other) = delete;

	};
}

#endif