#include "ini_reader.h"
#include <iostream>

int main()
{
    INIReader reader("config.ini");

    if (reader.ParseError() < 0)
    {
        std::cout << "Can't load 'config.ini'\n";
        return 1;
    }
    std::cout << "Config loaded from 'config.ini': feature_size=" << reader.GetInteger("model", "feature_size", -1)
              << ", input_names=" << reader.Get("tensorflow", "input_names", "UNKNOWN")
              << ", lstm_forget_bias=" << reader.GetReal("model", "lstm_forget_bias", -1)
              << ", wtsv1=" << reader.GetBoolean("tensorrt", "wtsv1", true) << "\n";
    std::cout << "Has values: tensorrt.device_id=" << reader.HasValue("tensorrt", "device_id")
              << ", tensorrt.engine_type=" << reader.HasValue("tensorrt", "engine_type") << "\n";
    std::cout << "Has sections: test=" << reader.HasSection("test") << ", model=" << reader.HasSection("model") << "\n";
    auto dims = reader.GetIntArray("model", "output_dims");
    std::cout << "model.output_dims: ";
    for (auto d : dims)
    {
        std::cout << d << " ";
    }
    std::cout << std::endl;
    return 0;
}