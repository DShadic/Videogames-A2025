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
    for (const auto& chunk : layer.chunks)
    {
        if (layer.width <= 0 || layer.height <= 0) 
        {
            std::cerr << "Error: Capa con dimensiones inválidas." << std::endl;
            return;
        }   
        sf::VertexArray vertices(sf::Quads);
        vertices.resize(chunk.width * chunk.height * 4); // 4 vértices por tile
            
        for (int y = 0; y < chunk.height; ++y) 
        {
            for (int x = 0; x < chunk.width; ++x)
            {
                int tileID = chunk.data[y * chunk.width + x];
                if (tileID == 0) continue; // Tile vacío (0 = sin tile)
                
                int tilesPerRow = tileset.getSize().x / tileSize.x;
                // Posición en el tileset
                int tu = (tileID - 1) % (tileset.getSize().x / tileSize.x);
                int tv = (tileID - 1) / (tileset.getSize().x / tileSize.x);

                // Asegurar que el tileID es válido
                if (tu < 0 || tv < 0 || tu >= tilesPerRow) 
                {
                    std::cerr << "Error: Tile ID " << tileID << " inválido." << std::endl;
                    continue;
                }
                // Obtener un puntero al cuadrante actual del tile
                sf::Vertex* quad = &vertices[(x + y * chunk.width) * 4];

                float px = (chunk.x + x) * tileSize.x + mapOffsetX;
                float py = (chunk.y + y) * tileSize.y + mapOffsetY;
                // Definir las coordenadas del tile
                quad[0].position = sf::Vector2f(px, py);
                quad[1].position = sf::Vector2f(px + tileSize.x, py);
                quad[2].position = sf::Vector2f(px + tileSize.x, py + tileSize.y);
                quad[3].position = sf::Vector2f(px, py + tileSize.y);

                // Definir las coordenadas de textura
                quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
                quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
                quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
                quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
            }
        }
        // Dibujar todos los tiles de una vez
        sf::RenderStates states;
        states.texture = &tileset;
        target.draw(vertices, states);
    }
}

bool TMXLoader::ParseLayer(tinyxml2::XMLElement* layer, TileLayer& outLayer) 
{
    if (layer->QueryIntAttribute("width", &outLayer.width) != tinyxml2::XML_SUCCESS ||
        layer->QueryIntAttribute("height", &outLayer.height) != tinyxml2::XML_SUCCESS) 
        {
            return false;
        }

    tinyxml2::XMLElement* data = layer->FirstChildElement("data");
    if (data && data->Attribute("encoding") && std::string(data->Attribute("encoding")) == "csv") 
    {
        for (tinyxml2::XMLElement* chunk = data->FirstChildElement("chunk"); chunk; chunk = chunk->NextSiblingElement("chunk"))
        {
            ParseCSVData(chunk, outLayer);
        }

        return true;
    }

    return false; // Soporta solo CSV por simplicidad
}

void TMXLoader::ParseCSVData(tinyxml2::XMLElement* chunk, TileLayer& outLayer) 
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
            c.data.push_back(std::stoi(token));
        }
    }
    
    outLayer.chunks.push_back(c);
}

} // namespace tmx