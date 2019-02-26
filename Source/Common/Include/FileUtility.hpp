#include "Path.hpp"

#include <vector>

namespace engine
{

    std::string LoadFile(const Path& path);

    std::vector<Uint8> LoadFileVec(const Path& path);

    Bool SaveFile(const Path& path, const Uint8* data, Uint32 size);

} // namespace engine
