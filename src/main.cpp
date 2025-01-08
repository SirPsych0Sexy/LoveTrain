#include <iostream>
#include <cpr/cpr.h>
#include "colors.h"
#include <fstream>
#include <argparse.hpp>
#include <vector>

//Funcion donde va el banner

inline void printLoveTrain() {
    std::cout << R"(██       ██████  ██    ██ ███████ ████████ ██████   █████  ██ ███    ██     ███████ ██    ██ ███████ ███████ ███████ ██████
██      ██    ██ ██    ██ ██         ██    ██   ██ ██   ██ ██ ████   ██     ██      ██    ██    ███     ███  ██      ██   ██
██      ██    ██ ██    ██ █████      ██    ██████  ███████ ██ ██ ██  ██     █████   ██    ██   ███     ███   █████   ██████
██      ██    ██  ██  ██  ██         ██    ██   ██ ██   ██ ██ ██  ██ ██     ██      ██    ██  ███     ███    ██      ██   ██
███████  ██████    ████   ███████    ██    ██   ██ ██   ██ ██ ██   ████     ██       ██████  ███████ ███████ ███████ ██   ██
                                                                                                                             )" << std::endl;
}


// Función para validar y ajustar la URL ingresada
std::string validateUrl(const std::string& inputUrl) {
    if (inputUrl.find("http://") == 0 || inputUrl.find("https://") == 0) {
        return inputUrl; // La URL ya es válida
    }
    return "http://" + inputUrl; // Agregar esquema por defecto si falta
}

// Función para codificar las rutas
std::string urlEncode(const std::string& value) {
    std::string encoded;
    char hexChars[] = "0123456789ABCDEF";
    for (unsigned char c : value) {
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            encoded += c;
        } else {
            encoded += '%';
            encoded += hexChars[c >> 4];
            encoded += hexChars[c & 0xF];
        }
    }
    return encoded;
}

// Función para limpiar caracteres indeseados de una línea
std::string cleanLine(const std::string& line) {
    std::string cleaned = line;
    cleaned.erase(std::remove_if(cleaned.begin(), cleaned.end(), [](unsigned char c) {
        return c == '\r' || c == '\n';
    }), cleaned.end());
    return cleaned;
}

// Funciones para imprimir el estado de la respuesta HTTP

inline void printOnly200(int statusCode, const std::string& url) {
    if (statusCode >= 200 && statusCode <= 299) {
        ColorConsole::setColor(ColorConsole::GREEN);
        std::cout << "Found: " << url << " - Status: " << statusCode << std::endl;
    }
}

inline void printResponseStatus(int statusCode, const std::string& url) {
    if (statusCode == 404) {
        ColorConsole::setColor(ColorConsole::RED);
        std::cout << "Not Found: " << url << " - Status: " << statusCode << std::endl;
    }
    else if (statusCode >= 200 && statusCode <= 299) {
        ColorConsole::setColor(ColorConsole::GREEN);
        std::cout << "Found: " << url << " - Status: " << statusCode << std::endl;
    } else if (statusCode >= 300 && statusCode <= 399) {
        ColorConsole::setColor(ColorConsole::BLUE);
        std::cout << "Redirected: " << url << " - Status: " << statusCode << std::endl;
    } else if (statusCode >= 400 && statusCode <= 499) {
        ColorConsole::setColor(ColorConsole::RED);
        std::cout << "Client Error: " << url << " - Status: " << statusCode << std::endl;
    } else if (statusCode >= 500 && statusCode <= 599) {
        ColorConsole::setColor(ColorConsole::YELLOW);
        std::cout << "Server Error: " << url << " - Status: " << statusCode << std::endl;
    } else {
        ColorConsole::setColor(ColorConsole::WHITE);
        std::cout << "Unknown Status: " << url << " - Status: " << statusCode << std::endl;
    }
}

// Función principal de fuzzing
void fuzzDirectories(const std::string& baseUrl, const std::vector<std::string>& paths) {
    std::cout << "Comenzando fuzzing en: " << baseUrl << std::endl;
    std::vector<std::future<void>> futures;

    for (const auto& path : paths) {
        std::string encodedPath = urlEncode(path); // Codificar el path
        std::string url = baseUrl + encodedPath;   // Construir la URL completa

        cpr::Response r = cpr::Get(cpr::Url{url}, cpr::Timeout{2340}); // 2.5 segundos de timeout
        // Manejo de errores de la biblioteca cpr
        if (r.error) {
            ColorConsole::setColor(ColorConsole::YELLOW);
            std::cerr << "Error al acceder a " << url << ": " << r.error.message << std::endl;
            continue;
        }


      // Imprimir el resultado usando la función centralizada
        printOnly200(r.status_code, url);
    }
}

int main(int argc, char** argv) {
    printLoveTrain();
    std::string baseUrl;
    std::string fileName;
    // Crear el parser de argumentos
    argparse::ArgumentParser parser("lovetrain");

    // Agregar argumentos
    parser.add_argument("-w", "--wordlist")
        .required()
        .help("Archivo de wordlist para el fuzzing");

    parser.add_argument("-u", "--url")
        .required()
        .help("URL base para el escaneo");


    try {
        // Parsear los argumentos
        parser.parse_args(argc, argv);

        // Obtener los valores de los argumentos
        baseUrl = parser.get<std::string>("--url");
        fileName = parser.get<std::string>("--wordlist");

        std::cout << "URL: " << baseUrl << "\nWordlist: " << fileName << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }



    baseUrl = validateUrl(baseUrl); // Validar y ajustar la URL
    std::cout << "La URL ajustada es: " << baseUrl << std::endl;

    // Asegurarse de que baseUrl termine con '/'
    if (baseUrl.back() != '/') {
        baseUrl += '/';
    }

    std::vector<std::string> paths;
    std::ifstream pathsFile(fileName); // Abre el archivo de texto

    if (!pathsFile) {
        std::cerr << "Error al abrir el archivo introducido recuerda que solo se admiten .txt." << std::endl;
        return 1; // Salir si hay un error al abrir el archivo
    }

    std::string path;
    while (std::getline(pathsFile, path)) { // Leer línea por línea
        path = cleanLine(path); // Limpiar la línea de caracteres indeseados
        if (!path.empty()) { // Ignorar líneas vacías
            paths.push_back(path); // Agregar la ruta al vector
        }
    }

    if (paths.empty()) {
        std::cerr << "No se encontraron rutas en el archivo." << std::endl;
        return 1; // Salir si no hay rutas para procesar
    }

    fuzzDirectories(baseUrl, paths); // Llamar a la función de fuzzing

    return 0;
}
