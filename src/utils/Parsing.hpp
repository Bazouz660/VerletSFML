/*
** EPITECH PROJECT, 2023
** VerletSFML
** File description:
** Parsing
*/

#ifndef PARSING_HPP_
    #define PARSING_HPP_

    #include "common.hpp"

    namespace vle {
        static std::string removeExtension(const std::string& filename) {
            size_t lastdot = filename.find_last_of(".");
            if (lastdot == std::string::npos) return filename;
            return filename.substr(0, lastdot);
        }
    }

#endif /* !PARSING_HPP_ */
