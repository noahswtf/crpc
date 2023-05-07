#include <iostream>
#include <Windows.h>
#include <string>

#include "Include/DiscordRPC/include/discord_register.h"
#include "Include/DiscordRPC/include/discord_rpc.h"

#include <fstream>
#include "Include/json.hpp"
using json = nlohmann::json;

json data = {
	{"applicationID", "1104805889564749945"},
	{"state", "Changing Rich Presence!"},
	{"details", "Custom Rich Presence"},
	{"largeImageKey", "largeimagekey"},
	{"largeImageText", "largeImageText"},
	{"smallImageKey", "smallimagekey"},
	{"smallImageText", "smallImageText"}
};

std::string applicationID, state, details, largeImageKey, largeImageText, smallImageKey, smallImageText;

char breakpoint;

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
	discordPresence.startTimestamp = time(0);
	discordPresence.largeImageKey = largeImageKey.c_str();
	discordPresence.smallImageKey = smallImageKey.c_str();
	discordPresence.largeImageText = largeImageText.c_str();
	discordPresence.smallImageText = smallImageText.c_str();
	Discord_UpdatePresence(&discordPresence);
}

void optionalInput(std::string name, std::string& variable)
{
	std::string choice;

	std::cout << "\n[Optional] Do You Want To Input " << name << "? [y / n]: ";
	std::cin >> choice;

	if (choice == "y")
	{
		std::cout << "Input " << name << ": ";
		std::cin >> variable;
	}
}

bool checkConfig()
{
	std::string choice;

	std::ifstream config("config.json");

	if (!config)
	{
		std::cout << "[-] Config Not Found.";

		std::ofstream outputConfig("config.json");
		outputConfig << std::setw(4) << data << std::endl;
		outputConfig.close();
		return false;
	}

	config.close();

	std::cout << "Do You Want To Load Config? [y/n]: ";
	std::cin >> choice;

	if (choice == "n")
		return false;

	return true;
}


void main()
{
	std::cout << "Welcome To noahsx's Custom Discord Rich Presence Customiser.\n";

	if (!checkConfig())
	{
		std::cout << "\n\n[Required] Input Your Application ID: ";
		std::cin >> applicationID;

		std::cout << "\n[Required] Input State: ";
		std::cin >> state;

		optionalInput("Details", details);

		optionalInput("Large Image Key", largeImageKey);
		optionalInput("Large Image Text", largeImageText);

		optionalInput("Small Image Key", smallImageKey);
		optionalInput("Small Image Text", smallImageText);

		std::string choice;

		std::cout << "Do You Want To Update Your Config File? [y/n]: ";
		std::cin >> choice;

		if (choice == "y")
		{
			data["applicationID"] = applicationID;
			data["state"] = state;
			data["details"] = details;
			data["largeImageKey"] = largeImageKey;
			data["largeImageText"] = largeImageText;
			data["smallImageKey"] = smallImageKey;
			data["smallImageText"] = smallImageText;

			std::ofstream outputConfig("config.json");
			outputConfig << std::setw(4) << data << std::endl;
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
	}

	StartPresence();

	UpdatePresence();

	std::cout << "\nPress ENTER To Exit The Program. ";
	std::cin >> breakpoint;
}