#include "./headers/configs.h"

Config cfg_default;
Config cfg_user;

int readFiles(){
	try
	{
	    cfg_default.readFile("./../config/default.cfg");
	}
	catch(const FileIOException &fioex)
	{
	    std::cerr << "I/O error while reading file." << std::endl;
	    return(EXIT_FAILURE);
	}
	catch(const ParseException &pex)
	{
	    std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
	              << " - " << pex.getError() << std::endl;
	    return(EXIT_FAILURE);
	}
	  // Read the user defined file
	try
	{
	    cfg_user.readFile("./../config/user.cfg");
	}
	catch(const FileIOException &fioex)
	{
	    std::cerr << "I/O error while reading file." << std::endl;
	    return(EXIT_FAILURE);
	}
	catch(const ParseException &pex)
	{
	    std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
	              << " - " << pex.getError() << std::endl;
	    return(EXIT_FAILURE);
	}
}
float loadConfig(string type,string object,string property){
	const Setting& root_user = cfg_user.getRoot();
	const Setting& root_default = cfg_default.getRoot();
	const Setting &window_user = root_user[type][object][0];
	const Setting &window_default= root_default[type][object][0];
	float property_value;
	if(!(window_user.lookupValue(property,property_value))){
		cout<<"Warning: Loading default value of "<<property <<endl;
		if(!(window_default.lookupValue(property,property_value))){
			cout<<"Couldnt find name" <<endl;
			return(EXIT_FAILURE);
		}
	}
	return property_value;
}