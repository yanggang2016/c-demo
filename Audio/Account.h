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

	std::string user_name = ""; // �󶨵�¼�ĶԽ��û���Ϣ��name

	std::string pwd = ""; // �󶨵�¼�ĶԽ��û���Ϣ��password

	long user_id = 0;//�󶨵�¼�ĶԽ��û���Ϣ��uid
};
