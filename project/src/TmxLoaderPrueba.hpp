#pragma once

#include <vector>
#include <string>
#include <tinyxml2.h>
#include <SFML/Graphics.hpp>
#include <iostream>

namespace tmx {

/**
 * @struct TileLayer
 * @brief Representa una capa de tiles en un mapa TMX.
 */

struct TileLayer 
{
    std::vector<int> data;   ///< Datos de los tiles (IDs).
    int width = 0;           ///< Ancho de la capa en tiles.
    int height = 0;  ///< Alto de la capa en tiles.
    std::vector<bool> collisionMap; // Mapa de colisiones paralelo
    std::string name; // Para identificar capas como "suelo" o "paredes"        

    // const Chunk& getChunk(){
    //     return chunks;
    // }
};


/**
 * @class TMXLoader
 * @brief Cargador y parser de archivos .tmx (Tiled Map Editor).
 */
class TMXLoader 
{
public:
    /**
     * @brief Carga un archivo .tmx y extrae las capas de tiles.
     * @param filepath Ruta al archivo .tmx.
     * @param layers Vector donde se almacenarán las capas parseadas.
     * @return true si la carga fue exitosa, false en caso contrario.
     */
    static bool Load(const std::string& filepath, std::vector<TileLayer>& layers);


    static void DrawTileMap (sf::RenderTarget& target, 
                const sf::Texture& tileset, 
                const tmx::TileLayer& layer, 
                const sf::Vector2u& tileSize,
                int mapOffsetX, 
                int mapOffsetY);
private:
    /**
     * @brief Procesa una capa XML y la convierte en una TileLayer.
     * @param layer Elemento XML de la capa.
     * @param outLayer TileLayer resultante.
     * @return true si el procesamiento fue exitoso.
     */
    static bool ParseLayer(tinyxml2::XMLElement* layer, TileLayer& outLayer);

    /**
     * @brief Procesa los datos de tiles en formato CSV.
     * @param data Elemento XML con los datos.
     * @param outLayer TileLayer a rellenar.
     */
    static void ParseCSVData(tinyxml2::XMLElement* data, TileLayer& outLayer);

};

} // namespace tmx