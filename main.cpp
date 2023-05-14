#include <iostream>
#include <fstream>
#include <string>
#include <chrono> // for std::chrono::system_clock
#include <thread> // for std::this_thread::sleep_for

#include "Include/DiscordRPC/include/discord_register.h"
#include "Include/DiscordRPC/include/discord_rpc.h"
#include "Include/json.hpp"

using json = nlohmann::json;

// Default rich presence data
json data = 
{
  {"applicationID", "1104805889564749945"},
  {"state", "Changing Rich Presence!"},
  {"details", "Custom Rich Presence"},
  {"largeImageKey", "largeimagekey"},
  {"largeImageText", "largeImageText"},
  {"smallImageKey", "smallimagekey"},
  {"smallImageText", "smallImageText"}
};

// Global variables for storing rich presence data
std::string applicationID;
std::string state;
std::string details;
std::string largeImageKey;
std::string largeImageText;
std::string smallImageKey;
std::string smallImageText;

void StartPresence() 
{
	DiscordEventHandlers Handler;
	memset(&Handler, 0, sizeof(Handler));
	Discord_Initialize(applicationID.c_str(), &Handler, 1, NULL);
}

void UpdatePresence() 
{
	DiscordRichPresence discordPresence;
	memset(&discordPresence, 0, sizeof(discordPresence));
	discordPresence.state = state.c_str();
	discordPresence.details = details.c_str();
	discordPresence.startTimestamp = std::chrono::system_clock::now().time_since_epoch().count();
	discordPresence.largeImageKey = largeImageKey.c_str();
	discordPresence.smallImageKey = smallImageKey.c_str();
	discordPresence.largeImageText = largeImageText.c_str();
	discordPresence.smallImageText = smallImageText.c_str();
	Discord_UpdatePresence(&discordPresence);
}

void OptionalInput(const std::string&& name, std::string& variable) 
{
	char choice{};
	std::cout << '\n' << "[Optional] Do You Want To Input " << name << "? [y/n]: ";
	std::cin >> choice;

	if (choice == 'y') 
	{
		std::cout << "Input " << name << ": ";
		std::cin >> variable;
	}
}

bool CheckConfig() 
{
	char choice{};
	std::ifstream config("config.json");
	if (!config) {
		std::cout << "[-] Config Not Found.";
		std::ofstream outputConfig("config.json");
		outputConfig << std::setw(4) << data << '\n';
		outputConfig.close();
		return false;
	}
	config.close();
	std::cout << "Do You Want To Load Config? [y/n]: ";
	std::cin >> choice;
	return (choice == 'y');
}

int main()
{
	std::cout << "Welcome to noahsx's Custom Discord Rich Presence Customizer.\n";

	if (!CheckConfig())
	{
		std::cout << "\n\n" << "[Required] Input your Application ID: ";
		std::cin >> applicationID;

		std::cout << "[Required] Input State: ";
		std::cin >> state;

		OptionalInput("Details", details);
		OptionalInput("Large Image Key", largeImageKey);
		OptionalInput("Large Image Text", largeImageText);
		OptionalInput("Small Image Key", smallImageKey);
		OptionalInput("Small Image Text", smallImageText);

		char choice{};
		std::cout << "Do you want to update your config file? [y/n]: ";
		std::cin >> choice;

		if (choice == 'y')
		{
			data["applicationID"] = applicationID;
			data["state"] = state;
			data["details"] = details;
			data["largeImageKey"] = largeImageKey;
			data["largeImageText"] = largeImageText;
			data["smallImageKey"] = smallImageKey;
			data["smallImageText"] = smallImageText;

			std::ofstream outputConfig("config.json");
			outputConfig << std::setw(4) << data << '\n';
			outputConfig.close();
		}
	}
	else
	{
		std::ifstream config("config.json");
		config >> data;

		applicationID = data.value("applicationID", "1104805889564749945");
		state = data.value("state", "Changing Rich Presence!");
		details = data.value("details", "Custom Rich Presence");
		largeImageKey = data.value("largeImageKey", "largeImageKey");
		largeImageText = data.value("largeImageText", "largeImageText");
		smallImageKey = data.value("smallImageKey", "smallImageKey");
		smallImageText = data.value("smallImageText", "smallImageText");

		config.close();
	}

	StartPresence();
	UpdatePresence();

	std::cout << '\n' << "If you press any key the program will STOP!" << '\n';
	system("pause > nul");

	Discord_Shutdown();
	return 0;
}
