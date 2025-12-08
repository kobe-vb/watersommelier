
#include "WebsiteModel.hpp"

#include <thread>

WebsiteModel::WebsiteModel() : UIModel()
{
    website_button = ButtonModel("Go to website");
}

void WebsiteModel::set_website(std::string website)
{
    this->website = website;

    website_button.set_callback(
        [url = this->website]()
        {
            std::thread([url]()
                        {
                // std::string command = "start \"\" \"" + url + "\"";
                std::string command = "start " + url;
                system(command.c_str()); })
                .detach();
        });
}

void WebsiteModel::set_data(std::string website_data)
{
    this->data = WebsiteData(website_data);
}