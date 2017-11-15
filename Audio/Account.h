#pragma once
#include <string>


class Account
{
public:
	
	Account();

	~Account();

	const std::string& get_user_name();

	const std::string& get_pwd();

	const long& get_user_id();

	int set_user_name(const std::string& str);

	int set_pwd(const std::string& str);

	int set_user_id(const long& id);

private:

	std::string user_name = ""; // 绑定登录的对讲用户信息：name

	std::string pwd = ""; // 绑定登录的对讲用户信息：password

	long user_id = 0;//绑定登录的对讲用户信息：uid
};
