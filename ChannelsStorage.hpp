#ifndef CHANNELSSTORAGE_HPP
# define CHANNELSSTORAGE_HPP

# include "Library.hpp"

class ChannelsStorage {

private:
  std::map<std::string, t_channel> channels;
  std::map<int, User>             &map_users;

public:
	ChannelsStorage(std::map<int, User> &map_users);
	~ChannelsStorage();
  //Out func
	std::map<std::string, t_channel> getChannels();
  std::string joinToCannel(std::string msg, User *user);
	std::string	kickUser(std::string msg, User *user);
  std::string	addTopicToCannel(std::string msg, User *user);
  //Topic
  void setTopic(std::string channel_name, std::string topic);
  std::string getTopic(std::string channel_name);
  //Users
  bool addUserToChannel(std::string channel_name, std::string user_name);
  bool checkIfThisUserBanned(std::string channel_name, std::string user_name);
  bool foundUserInThisChannel(std::string channel_name, std::string user_name);
  bool  bannedUserInThisChannel(std::string channel_name, std::string user_name);
  std::string getOwnerChannel(std::string channel_name);
  //Channel
  bool addNewChannel(std::string channel_name);
  t_channel getChannelByName(std::string channel_name);
  //Update
  void updateChannels(std::string user_name, std::string new_user_name, int command);
};

#endif
