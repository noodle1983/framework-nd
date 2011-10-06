#ifndef RUBYINTERPRETER_H
#define RUBYINTERPRETER_H

#include <string>

namespace Script
{
namespace Ruby
{
	class RubyInterpreter
	{
	public:
		RubyInterpreter();
		virtual ~RubyInterpreter();

		void init();
		void fini();

		bool execute(const std::string& theCmd);
		bool executeFile(const std::string& theFilePath);

	private:
		void logRubyErrors(const std::string& theDesc, int theErrorCode);
	};

}
}

#endif /* RUBYINTERPRETER_H */

