//
// Created by users on 12/21/2024.
//

// color_console.h
#ifndef COLOR_CONSOLE_H
#define COLOR_CONSOLE_H

#include <iostream>

class ColorConsole {
public:
    // Colores de texto
    static const std::string RESET;
    static const std::string BLACK;
    static const std::string RED;
    static const std::string GREEN;
    static const std::string YELLOW;
    static const std::string BLUE;
    static const std::string MAGENTA;
    static const std::string CYAN;
    static const std::string WHITE;

    // Colores de fondo
    static const std::string BG_BLACK;
    static const std::string BG_RED;
    static const std::string BG_GREEN;
    static const std::string BG_YELLOW;
    static const std::string BG_BLUE;
    static const std::string BG_MAGENTA;
    static const std::string BG_CYAN;
    static const std::string BG_WHITE;

    // Función para imprimir texto con color
    static void setColor(const std::string& textColor, const std::string& bgColor = RESET) {
        std::cout << bgColor << textColor;
    }

    // Función para restablecer el color en caso de ser necesario
    static void reset() {
        std::cout << RESET;
    }
};

// Definición de los colores(gracias perplexity)
const std::string ColorConsole::RESET = "\033[0m";
const std::string ColorConsole::BLACK = "\033[30m";
const std::string ColorConsole::RED = "\033[31m";
const std::string ColorConsole::GREEN = "\033[32m";
const std::string ColorConsole::YELLOW = "\033[33m";
const std::string ColorConsole::BLUE = "\033[34m";
const std::string ColorConsole::MAGENTA = "\033[35m";
const std::string ColorConsole::CYAN = "\033[36m";
const std::string ColorConsole::WHITE = "\033[37m";

const std::string ColorConsole::BG_BLACK = "\033[40m";
const std::string ColorConsole::BG_RED = "\033[41m";
const std::string ColorConsole::BG_GREEN = "\033[42m";
const std::string ColorConsole::BG_YELLOW = "\033[43m";
const std::string ColorConsole::BG_BLUE = "\033[44m";
const std::string ColorConsole::BG_MAGENTA = "\033[45m";
const std::string ColorConsole::BG_CYAN = "\033[46m";
const std::string ColorConsole::BG_WHITE = "\033[47m";

#endif // COLOR_CONSOLE_H

