#ifndef HAF_HOST_CONFIG_INCLUDE_HPP
#define HAF_HOST_CONFIG_INCLUDE_HPP

#include <hlog/include/hlog.hpp>
#include <htypes/include/str.hpp>
#include <htypes/include/vector.hpp>
#include <htypes/include/parpar.hpp>
#include <htypes/include/object.hpp>
#include <htypes/include/parpar.hpp>

#include <fstream>
#include <string>

using namespace htps;

namespace haf::host
{
class HostConfig final
{
public:
    HostConfig(int const argc, char const* const argv[]) :
        params_{parpar::create(argc, argv)}
    {
        parseCommandLineParameters();
    }

    str configuredFirstApp() const { return "haf_app_middleware"; }

    void loadConfigFile()
    {
        std::ifstream config_file{config_file_name_.c_str()};
        DisplayLog::info("Reading config file ", config_file_name_);
        std::string line;
        //        line << config_file;

        while (std::getline(config_file, line))
        {
        }
    }

    void parseCommandLineParameters()
    {
        if (params_.hasParameters())
        {
            static constexpr char kStartFile[] = "cfile";

            auto const start_file = params_.optionValue(kStartFile);

            if (start_file.first)
            {
                config_file_name_ = start_file.second;
                DisplayLog::debug("Parameter ", kStartFile,
                                  " found with value: ", config_file_name_);
            }
        }
    }

private:
    parpar::ParametersParser params_;
    str config_file_name_{"host_config.cfg"};  ///< Default config file name
};

}  // namespace haf::host

#endif
