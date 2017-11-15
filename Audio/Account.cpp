#include "stdafx.h"
#include "Account.h"

Account::Account()
{
}


Account::~Account()
{
}

const std::string & Account::get_user_name()
{
	return user_name;
}

const std::string & Account::get_pwd()
{
	return pwd;
}

const long & Account::get_user_id()
{
	return user_id;
}

int Account::set_user_name(const std::string & str)
{
	user_name = str;
	return 0;
}

int Account::set_pwd(const std::string & str)
{
	pwd = str;
	return 0;
}

int Account::set_user_id(const long & id)
{
	user_id = id;
	return 0;
}
