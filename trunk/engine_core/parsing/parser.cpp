#include "parser.hh"

void Parser::parse(const string in&,stirng &out)
{
	
}


int main()
{
	std::cout<<"compile test!\n";
	std::string raw="player=1234;operation=do;param_num=2;param=4;param=3;";
	std::string delimiters=";";
	std::vector<string> tokens;
	Parser::Tokenize(raw,tokens,delimiters);
	copy(tokens.begin(), tokens.end(), ostream_iterator<string>(cout, "\n"));
	return 0;
}
