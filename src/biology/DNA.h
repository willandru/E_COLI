#pragma once

#include <string>


class DNA
{
public:

    // ========================================================
    // Constructor
    // ========================================================

    explicit DNA(
        const std::string& filePath
    );


    // ========================================================
    // Información
    // ========================================================

    bool isLoaded() const;

    const std::string& getSequence() const;

    std::size_t getLength() const;


private:

    // ========================================================
    // Cargar FASTA
    // ========================================================

    bool loadFASTA(
        const std::string& filePath
    );


    // ========================================================
    // Validar base
    // ========================================================

    bool isValidBase(
        char base
    ) const;


private:

    std::string m_sequence;

    bool m_loaded;
};