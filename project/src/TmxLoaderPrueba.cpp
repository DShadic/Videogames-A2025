#include "TmxLoaderPrueba.hpp"
#include <sstream>

namespace tmx {

bool TMXLoader::Load(const std::string& filepath, std::vector<TileLayer>& layers) 
{
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(filepath.c_str()) != tinyxml2::XML_SUCCESS) 
    {
        return false;
    }

    tinyxml2::XMLElement* map = doc.FirstChildElement("map");
    if (!map) return false;

    TileLayer currentLayer;
    for (tinyxml2::XMLElement* layer = map->FirstChildElement("layer"); layer; layer = layer->NextSiblingElement("layer")) 
    {
        if (ParseLayer(layer, currentLayer)) 
        {
            layers.push_back(currentLayer);
        }
    }

    return true;
}

void TMXLoader::DrawTileMap(sf::RenderTarget& target, 
                const sf::Texture& tileset, 
                const tmx::TileLayer& layer, 
                const sf::Vector2u& tileSize,
                int mapOffsetX, 
                int mapOffsetY) 
                
{
    
    sf::VertexArray vertices(sf::Quads);
    vertices.resize(layer.width * layer.height * 4);

    for (int y = 0; y < layer.height; ++y) 
    {
        for (int x = 0; x < layer.width; ++x) 
        {
            int tileID = layer.data[y * layer.width + x];
            if (tileID == 0) continue;

            int tu = (tileID - 1) % (tileset.getSize().x / tileSize.x);
            int tv = (tileID - 1) / (tileset.getSize().x / tileSize.x);

            sf::Vertex* quad = &vertices[(x + y * layer.width) * 4];

            float px = x * tileSize.x + mapOffsetX;
            float py = y * tileSize.y + mapOffsetY;

            quad[0].position = sf::Vector2f(px, py);
            quad[1].position = sf::Vector2f(px + tileSize.x, py);
            quad[2].position = sf::Vector2f(px + tileSize.x, py + tileSize.y);
            quad[3].position = sf::Vector2f(px, py + tileSize.y);

            quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
            quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
            quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
            quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
            
        }
    }

    sf::RenderStates states;
    states.texture = &tileset;
    target.draw(vertices, states);
    
}

bool TMXLoader::ParseLayer(tinyxml2::XMLElement* layer, TileLayer& outLayer) 
{
    if (layer->QueryIntAttribute("width", &outLayer.width) != tinyxml2::XML_SUCCESS ||
        layer->QueryIntAttribute("height", &outLayer.height) != tinyxml2::XML_SUCCESS) 
    {
        return false;
    }

    outLayer.name = layer->Attribute("name");
    tinyxml2::XMLElement* data = layer->FirstChildElement("data");

    if (data && data->Attribute("encoding") && std::string(data->Attribute("encoding")) == "csv") 
    {
        const char* text = data->GetText();
        if (!text) return false;

        std::stringstream ss(text);
        std::string token;
        outLayer.data.clear();
        outLayer.data.reserve(outLayer.width * outLayer.height);

        while (std::getline(ss, token, ',')) 
        {
            if (!token.empty()) 
            {
                int tileID = std::stoi(token);
                outLayer.data.push_back(tileID);
            }
        }

        // Procesar colisiones si es la capa es bloques
        if (outLayer.name == "Bloques") 
        {
            outLayer.collisionMap.resize(outLayer.width * outLayer.height, false);
            for (size_t i = 0; i < outLayer.data.size(); ++i) 
            {
                outLayer.collisionMap[i] = (outLayer.data[i] != 0);
            }
        }

        return true;
    }

    return false; // Soporta solo CSV por simplicidad
}



/*void TMXLoader::ParseCSVData(tinyxml2::XMLElement* chunk, TileLayer& outLayer) 
{
    const char* text = chunk->GetText();
    if (!text) return;

    Chunk c;
    chunk->QueryIntAttribute("x", &c.x);
    chunk->QueryIntAttribute("y", &c.y);
    chunk->QueryIntAttribute("width", &c.width);
    chunk->QueryIntAttribute("height", &c.height);

    std::stringstream ss(text);
    std::string token;
    while (std::getline(ss, token, ',')) 
    {
        if (!token.empty()) 
        {
            int tileID = std::stoi(token);
            c.data.push_back(std::stoi(token));

            if (outLayer.name == "floor" && !outLayer.collisionMap.empty()) 
            {
                // Calcula posición relativa dentro del chunk
                int relX = c.data.size() % c.width;
                int relY = c.data.size() / c.width;
                
                // Mapea a coordenadas globales del collisionMap
                int globalX = c.x + relX;
                int globalY = c.y + relY;
                
                //std::cout << "globalX:" << globalX << "\ngobalY:" << globalY << std::endl;
                outLayer.collisionMap[globalY * outLayer.width + globalX] = (tileID != 0);
                // Ajusta a índices no negativos (si es necesario)
                if (globalX >= 0 && globalY >= 0 && 
                    globalX < outLayer.width && 
                    globalY < outLayer.height) 
                {
                    outLayer.collisionMap[globalY * outLayer.width + globalX] = (tileID != 0);
                }
            }
        }
    }
    
    outLayer.chunks.push_back(c);
}*/

} // namespace tmx