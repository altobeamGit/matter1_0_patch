/* See Project CHIP LICENSE file for licensing information. */

#include <platform/logging/LogV.h>

#include <lib/core/CHIPConfig.h>
#include <lib/support/logging/Constants.h>

#include <stdio.h>

extern "C" void Console_SetPolling(int polling);
extern "C" void Console_Putc(unsigned char data);

#ifdef LOG_LOCAL_LEVEL
#undef LOG_LOCAL_LEVEL
#endif

namespace chip {
namespace Logging {
namespace Platform {

void LogV(const char * module, uint8_t category, const char * msg, va_list v)
{
	int i = 0;
	int len = 0;
	char formattedMsg[CHIP_CONFIG_LOG_MESSAGE_MAX_SIZE];
	char outputMsg[CHIP_CONFIG_LOG_MESSAGE_MAX_SIZE+11];
	char *p = &outputMsg[0];

	vsnprintf(formattedMsg, sizeof(formattedMsg), msg, v);
	len = snprintf(outputMsg, sizeof(outputMsg), "\n[%s] %s\r\n", module, formattedMsg);
	Console_SetPolling(1);
	while (i++ < len)
	{
		Console_Putc(*p++);
	}
	Console_SetPolling(0);
}

} // namespace Platform
} // namespace Logging
} // namespace chip
