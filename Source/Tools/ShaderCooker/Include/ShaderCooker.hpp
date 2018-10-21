#pragma once

class Path;

class ShaderCooker
{
    public:
        ShaderCooker() {}
        ~ShaderCooker() {}
        bool Cook(const Path& input, const Path& output);
    private:
};
