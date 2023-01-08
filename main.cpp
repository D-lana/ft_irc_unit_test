#include "Library.hpp"

int main() {
  std::map<int, User>		map_users;
  User user_1;
  user_1.setUserName("Roma");
  user_1.setLogin("Roma_login");
  user_1.setUserFd(4);
  User user_2;
  user_2.setUserName("Masha");
  user_2.setLogin("Masha_login");
  user_2.setUserFd(5);
  User user_3;
  user_3.setUserName("Lusi");
  user_3.setLogin("Lusi_login");
  user_3.setUserFd(6);
  map_users.insert(std::pair<int, User>(user_1.getUserFd(), user_1));
  map_users.insert(std::pair<int, User>(user_2.getUserFd(), user_2));
  map_users.insert(std::pair<int, User>(user_3.getUserFd(), user_3));
  ChannelsStorage storage(map_users);

  storage.joinToCannel("JOIN #WE", &user_1);
  storage.joinToCannel("JOIN #WE", &user_2);
  storage.joinToCannel("JOIN #WE", &user_3);
  //storage.addNewChannel("channel_name", &user_1);
  //storage.addNewChannel("channel_name", &user_2);
  std::cout << "User_1:" << user_1.getUserName() << std::endl;
  std::cout << "Size channel: " << storage.getChannels().size() << std::endl;
  std::cout << "Size list_users: " << storage.getChannels().begin()->second.list_users.size() << std::endl;

  storage.kickUser("KICK #WE Masha_login", &user_1);
  storage.updateChannels("Masha_login", "Pretty_Masha", CHANGE_NICK);
  storage.joinToCannel("JOIN #WE", &user_1);
  storage.joinToCannel("JOIN #WE", &user_2);
  storage.addTopicToCannel("TOPIC #WE Hello", &user_2);
}
