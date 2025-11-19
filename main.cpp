#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <regex>

int main()
{
	std::string FileContentStr{ "" };
	// Most variable here do not need to live for the duration of the whole program. We can release them from the stack sooner by tightening their scope.
	// Some of these variables can get relatively sizeable, so it's probably the one time where this practice has obvious advantages.
	{
		std::string filePathToChart{ "./notes.chart" };

		std::stringstream FileContent{ "" };

		std::ifstream theFileItself(filePathToChart);

		FileContent << theFileItself.rdbuf();

		theFileItself.close();

		FileContentStr = FileContent.str();
	}

	auto regexCapture = [&FileContentStr](const std::regex& regexPattern) -> std::string
		{
			// Regex is pretty cool. You make spells, and they WILL find the target string or sequence. Litteral string voodoo is what this is.
			std::smatch MatchedPattern;
			if (std::regex_search(FileContentStr, MatchedPattern, regexPattern))
			{
				// We extract the first capture by giving it the 1 index (if we gave it 0, it would fill the string with every possible match, probably makes no difference in this case).
				return MatchedPattern[1].str();
			}
			else
			{
				std::cerr << "Failed to find pattern.\n";
			}
		};

	auto stoiWithCatches = [](const std::string& stringToTurnIntoInteger) -> int
		{
			try
			{
				return stoi(stringToTurnIntoInteger);
			}
			// stoi throws errors, and if we don't catch them, program will act even weirder. Not like c++ would just let us know about this ofc, but it exists anyways...
			// So anyways, this error is for strings that are not numbers.
			catch (const std::invalid_argument& e)
			{
				std::cerr << "Error : " << e.what() << std::endl;
			}
			// This one is for numbers above or below the limits of an integer, probably if not in this interval -> [2^-31, 2^31-1]
			catch (const std::out_of_range& e)
			{
				std::cerr << "Error : " << e.what() << std::endl;
			}
		};

	int Resolution{ 0 };
	{
		std::regex ResolutionPattern{ "Resolution \= (\\d{1,4})" };
		Resolution = stoiWithCatches(regexCapture(ResolutionPattern));
	}
	std::cout << Resolution << std::endl;

	int TimeSignature{ 0 };
	{
		std::regex TimeSignaturePattern{ "\\[SyncTrack\\]\n\\{\n  \\d{1,5} \= TS (\\d{1,9})" };
		TimeSignature = stoiWithCatches(regexCapture(TimeSignaturePattern));
	}
	std::cout << TimeSignature << std::endl;

	int BeatsPerMinute{ 0 };
	{
		std::regex BeatsPerMinutePattern{ "\\[SyncTrack\\]\n\\{\n  \\d{1,5} \= TS \\d{1,9}\n  0 \= B (\\d{1,9})" };
		BeatsPerMinute = stoiWithCatches(regexCapture(BeatsPerMinutePattern));
	}
	std::cout << BeatsPerMinute << std::endl;

	std::string NoteContent{ "" };
	{ 
		std::regex NoteContentPattern{ "\\[ExpertSingle\\]\\n\\{\\n([\\s\\S]*?)\n\\}" };
		NoteContent = regexCapture(NoteContentPattern);
	}
	// std::cout << NoteContent << std::endl;

	std::regex linePattern("^\\s*\\d{3,6}\\s*=\\s*N (\\d{1,5}).*$");
	for (auto it = std::sregex_iterator(NoteContent.begin(), NoteContent.end(), linePattern); it != std::sregex_iterator(); ++it)
	{
		std::cout << (*it)[1].str() << std::endl;
	}
}