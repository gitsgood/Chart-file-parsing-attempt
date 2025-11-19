#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <regex>

int main()
{
	std::string filePathToChart{ "./notes.chart" };

	std::stringstream FileContent{ "" };

	std::ifstream theFileItself(filePathToChart);

	FileContent << theFileItself.rdbuf();

	theFileItself.close();

	std::string FileContentStr = FileContent.str();

	auto getNumberFromFile = [FileContentStr](const std::string& stringPrecedingValue, int valueDigitCount) -> int
		{
			size_t StringPosition{ FileContentStr.find(stringPrecedingValue) };

			std::string SearchedForNumberString = FileContentStr.substr(StringPosition + stringPrecedingValue.size(), valueDigitCount);
			std::cout << SearchedForNumberString << std::endl;

			try
			{
				int SearchedForNumber{ stoi(SearchedForNumberString) };
				std::cout << SearchedForNumber << std::endl;
				return SearchedForNumber;
			}
			// Catch if the string isn't a valid number
			catch (const std::invalid_argument& e) {
				std::cerr << "Invalid argument: The string is not a number. Error: " << e.what() << std::endl;
			}
			// Catch if the number is too big or small for an int
			catch (const std::out_of_range& e) {
				std::cerr << "Out of range: The number is too large or small. Error: " << e.what() << std::endl;
			}
		};

	getNumberFromFile("Resolution \= ", 3);

	getNumberFromFile("\[SyncTrack\]\n\{\n  0 \= TS ", 1);

	std::stringstream InputForBeatsPerMinuteCall;
	InputForBeatsPerMinuteCall << "\[SyncTrack\]\n\{\n  0 \= TS " << getNumberFromFile("\[SyncTrack\]\n\{\n  0 \= TS ", 1) << "\n  0 \= B ";

	getNumberFromFile(InputForBeatsPerMinuteCall.str(), 6);

	std::regex BlockPattern("\\[ExpertSingle\\]\\n\\{\\n([\\s\\S]*?)\n\\}");
	std::smatch BlockMatch;
	std::string NoteContent;

	if (std::regex_search(FileContentStr, BlockMatch, BlockPattern))
	{
		NoteContent = BlockMatch[1].str();
	}

	//std::cout << NoteContent << std::endl;

	std::regex linePattern("^\\s*\\d{3,6}\\s*=\\s*N (\\d{1,5}).*$");
	for (auto it = std::sregex_iterator(NoteContent.begin(), NoteContent.end(), linePattern); it != std::sregex_iterator(); ++it)
	{
		std::cout << "The Note we want for this event is the number: "
			<< (*it)[1].str() << std::endl;
	}

	//std::string RegexTest{ "^\\s*\\d{3,6}\\s*=\\s*N (\\d{1,5}).*$" };

	////std::cout<<RegexTest.find("*{3,6}");

	//std::regex Pattern{ "\\[ExpertSingle\\]\n\\{\n\\*[^\}]\\*\\d{3,6}" };

	//std::cout << std::endl << std::regex_search(RegexTest, Pattern);

}