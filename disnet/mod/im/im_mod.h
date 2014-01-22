#ifndef __im_mod_h__
#define __im_mod_h__

#include <string>
#include <functional>


#ifdef IM_EXPORTS
#define IM_API __declspec(dllexport)
#else
#define IM_API __declspec(dllimport)
#endif


class im_mod{
public:
	int startup(std::string conf);
	int stop();

protected:

};

#endif