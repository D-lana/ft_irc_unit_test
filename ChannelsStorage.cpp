#include "Library.hpp"

ChannelsStorage::ChannelsStorage(std::map<int, User> &map_users) : map_users(map_users) {
}

ChannelsStorage::~ChannelsStorage() {
}

std::map<std::string, t_channel> ChannelsStorage::getChannels() {
    return channels;
}

bool ChannelsStorage::addNewChannel(std::string channel_name) {
    t_channel tmp;
    tmp.name = channel_name;
    tmp.topic = "No topic is set";
  
    return(channels.insert(std::pair<std::string, t_channel> (channel_name, tmp)).second);
}

bool ChannelsStorage::addUserToChannel(std::string channel_name, std::string user) {
  std::map<std::string, t_channel>::iterator it_ch = channels.find(channel_name);
  if (it_ch != channels.end()) {
      it_ch->second.list_users.push_back(user);
      return(true);
  }
  return(false);
}

std::string ChannelsStorage::getOwnerChannel(std::string channel_name) {
    std::map<std::string, t_channel>::iterator it_ch = channels.find(channel_name);
    if (it_ch != channels.end()) {
        return(it_ch->second.list_users.front());
    }
    return("");
}

t_channel ChannelsStorage::getChannelByName(std::string channel_name) {
    t_channel channel_empty;
    channel_empty.name = "";
    channel_empty.topic = "";
  
    std::map<std::string, t_channel>::iterator it_ch = channels.find(channel_name);
    if (it_ch != channels.end()) {
        return(it_ch->second);
    }
    return(channel_empty);
}

std::string ChannelsStorage::getTopic(std::string channel_name) {
    std::map<std::string, t_channel>::iterator it_ch = channels.find(channel_name);
    if (it_ch != channels.end()) {
        return(it_ch->second.topic);
    }
    return("No topic is set");
}

void ChannelsStorage::setTopic(std::string channel_name, std::string topic) {
    std::map<std::string, t_channel>::iterator it_ch = channels.find(channel_name);
    if (it_ch != channels.end()) {
       it_ch->second.topic = topic;
    }
}

bool ChannelsStorage::checkIfThisUserBanned(std::string name_channel, std::string user) {
    std::list<std::string> banned_users;
  
    banned_users = getChannelByName(name_channel).banned_users;
    if (std::find(banned_users.begin(), banned_users.end(), user) != banned_users.end()) {
      return(true);
    }
  return(false);
}

bool ChannelsStorage::foundUserInThisChannel(std::string name_channel, std::string user) {
    std::list<std::string> list_users;
  
    list_users = getChannelByName(name_channel).list_users;
    if (std::find(list_users.begin(), list_users.end(), user) != list_users.end()) {
      return(true);
    }
  return(false);
}

std::string	ChannelsStorage::joinToCannel(std::string msg, User *user) {
	std::list<std::string> users_in;
  std::string		owner = "";
  std::string		str = "";
	std::string		topic = "";
	std::string		name_channel = "";
  std::string		user_n = user->getLogin();
	std::vector<std::string> vec_msg = splitString(msg, ' ');
	// for(int i = 0; i < static_cast<int>(vec_msg.size()); i++) {
	// 	std::cout << "<" << vec_msg[i] << "> ";
	// }
  if (vec_msg.size() != 2) {
    str.append(":IRC-kitty " + toString(ERR_NEEDMOREPARAMS) + " " + vec_msg[0] + " :Not enough parameters\n");
  }
	else if (vec_msg[1][0] != '#') {
    str.append(":IRC-kitty " + toString(ERR_NOSUCHCHANNEL) + " " + vec_msg[1] + " :No such channel\n");
  }
  else if (checkIfThisUserBanned(vec_msg[1], user_n) == true) {
    str.append(":IRC-kitty " + toString(ERR_BANNEDFROMCHAN) + " " + vec_msg[1] + " :Cannot join channel (+b)\n");
  }
  else {
		name_channel = vec_msg[1];
    if (foundUserInThisChannel(name_channel, user_n) == false) {
      addNewChannel(name_channel);
      addUserToChannel(name_channel, user_n);
      //Join to channel Success
  		str.append(":" + user_n + "!" + user_n + "@127.0.0.1 ");
  		str.append("JOIN :" + name_channel + "\n");
    }
		//Get Topic
    topic = getTopic(name_channel);
		str.append(":IRC-kitty 331 " + user_n +  " " + name_channel +  " :" + topic + "\n");
    //Get NAMES list in this channel
    users_in = getChannelByName(name_channel).list_users;
		str.append(":IRC-kitty 353 " + user_n + " = " + name_channel + " :@");
    for (std::list<std::string>::iterator it = users_in.begin(); it != users_in.end(); ++it) {
      str.append(*it + " ");
    }
    //End of /NAMES list
		str.append("\n:IRC-kitty 366 " + user_n +  " " + name_channel + " :End of /NAMES list\n");
  }
  std::cout << "\n------ReadyMess JOIN-----:\n" << str << std::endl;
  std::cout << "name_channel: " << name_channel << std::endl;
  std::cout << "user->getUserName(): " << user_n << std::endl;
	return(str);
}

std::string	ChannelsStorage::kickUser(std::string msg, User *user) {
	//Params: KICK <channel> <user> [<comment>]
  size_t 			start_channel = msg.find("#");
	size_t 			end_channel;
	std::string		str = "";
	//std::string		owner = "";
  //std::string		name_channel = "";
  std::string		user_n = user->getLogin();
	std::vector<std::string> vec_msg = splitString(msg, ' ');

	for(int i = 0; i < static_cast<int>(vec_msg.size()); i++) {
		std::cout << "<" << vec_msg[i] << "> ";
	}
	std::cout << "\n";

  if (vec_msg.size() < 3) {
    str.append(":IRC-kitty " + toString(ERR_NEEDMOREPARAMS) + " " + vec_msg[0] + " :Not enough parameters\n");
  }
  else if (foundUserInThisChannel(vec_msg[1], user_n) == false) {
    str.append(":IRC-kitty " + toString(ERR_NOTONCHANNEL) + " " + vec_msg[1] + " :You're not on that channel");
  }
  else if (user->getIsAdminHere() == false && getOwnerChannel(vec_msg[1]) != user_n) {
    str.append(":IRC-kitty " + toString(ERR_CHANOPRIVSNEEDED) + " " + vec_msg[1]
      + " :You're not channel operator");
  }
  else {
    bannedUserInThisChannel(vec_msg[1], vec_msg[2]);
    str.append(":" + user_n + "!" + user_n + "@127.0.0.1 ");
		str.append(msg + " :" + user_n + "\n");
    //:456!456@127.0.0.1 KICK #WE 123 :456
  }
	std::cout << "\n------ReadyMess KICK-----:\n" << str << std::endl;
  std::cout << getOwnerChannel(vec_msg[1]) << std::endl;
  std::cout << user_n << std::endl;
	return(str);
}

std::string	ChannelsStorage::addTopicToCannel(std::string msg, User *user) {
  
  std::list<std::string> users_in;
  std::string		owner = "";
  std::string		str = "";
	std::string		topic = "";
	std::string		num = "";
  std::string		user_n = user->getLogin();
	std::vector<std::string> vec_msg = splitString(msg, ' ');
  
  for(int i = 0; i < static_cast<int>(vec_msg.size()); i++) {
		std::cout << "<" << vec_msg[i] << "> ";
	}
	std::cout << "\n";
  
  if (vec_msg.size() != 2 && vec_msg.size() != 3) {
    str.append(":IRC-kitty " + toString(ERR_NEEDMOREPARAMS) + " " + vec_msg[0] + " :Not enough parameters\n");
  }
  else if (foundUserInThisChannel(vec_msg[1], user_n) == false) {
    str.append(":IRC-kitty " + toString(ERR_NOTONCHANNEL) + " " + vec_msg[1] + " :You're not on that channel");
  }
  else if (getChannelByName(vec_msg[1]).name == "") {
    str.append(":IRC-kitty " + toString(ERR_NOSUCHCHANNEL) + " " + vec_msg[1] + " :No such channel\n");
  }
  else {
    if (vec_msg.size() == 3) {
      setTopic(vec_msg[1], vec_msg[2]);
    }
    //Get Topic
    topic = getTopic(vec_msg[1]);
    if (topic == "No topic is set") {
      num.append("331 ");
    }
    else {
      num.append("332 ");
    }
		str.append(":IRC-kitty " + num + user_n +  " " + vec_msg[1] +  " :" + topic + "\n");
  }
  std::cout << "\n------ReadyMess TOPIC-----:\n" << str << std::endl;
  return(str);
}

bool	ChannelsStorage::bannedUserInThisChannel(std::string channel_name, std::string user_name) {
  std::map<std::string, t_channel>::iterator it_ch = channels.find(channel_name);
  if (it_ch != channels.end()) {
      it_ch->second.list_users.remove(user_name);
      it_ch->second.banned_users.push_back(user_name);
      return(true);
    }
  return(false);
}


void ChannelsStorage::updateChannels(std::string user_name, std::string new_user_name, int command) {
  if (command == DELETE_USER) {
    for (std::map<std::string, t_channel>::iterator it = channels.begin(); it != channels.end(); it++) {
      it->second.list_users.remove(user_name);
      it->second.banned_users.remove(user_name);
    }
  }
  else if(command == CHANGE_NICK) {
    for (std::map<std::string, t_channel>::iterator it = channels.begin(); it != channels.end(); it++) {
      std::replace(it->second.list_users.begin(), it->second.list_users.end(), user_name, new_user_name);
      std::replace(it->second.banned_users.begin(), it->second.banned_users.end(), user_name, new_user_name);
    }
  }
}



//### KICK `<channel>` `<user>` `[<comment>]`

//**??????????????????:** `<channel>` `<user>` `[<comment>]`

// :Dlana!Dlana@127.0.0.1 JOIN :#www
// :irc.ircnet.su 353 Dlana = #www :@Dlana
// :irc.ircnet.su 366 Dlana #www :End of /NAMES list.

// :irc.ircnet.su 001 Dlana :Welcome to the IrcNet.ru IRC Network Dlana!Dlana@127.0.0.1
//https://github.com/clokep/irc-docs/blob/main/draft-mitchell-irc-capabilities-02.html

//
//nc -l 6667
//nc irc.ircnet.ru 6667

		//:Dlana!Dlana@127.0.0.1 JOIN :#ggg
		//str = str_1 + str_2 + str_3;
		//SEND: :IRCat 331 B #SS :No topic is set
		//SEND: :IRCat 353 B = #SS :@B
		//SEND: :IRCat 366 B #SS :End of /NAMES list
