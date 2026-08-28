#include "DNA.h"

#include <fstream>
#include <iostream>
#include <cctype>


// ============================================================
// CONSTRUCTOR
// ============================================================

DNA::DNA(
    const std::string& filePath
)
    : m_sequence(),
      m_loaded(false)
{
    m_loaded =
        loadFASTA(
            filePath
        );
}


// ============================================================
// LOAD FASTA
// ============================================================

bool DNA::loadFASTA(
    const std::string& filePath
)
{
    std::ifstream file(
        filePath
    );


    if (!file.is_open())
    {
        std::cerr
            << "Error: no se pudo abrir el archivo de ADN:\n"
            << filePath
            << '\n';

        return false;
    }


    std::string line;


    while (std::getline(file, line))
    {
        // ====================================================
        // Encabezado FASTA
        // ====================================================

        if (!line.empty() && line[0] == '>')
        {
            continue;
        }


        // ====================================================
        // Leer bases
        // ====================================================

        for (char character : line)
        {
            char base =
                static_cast<char>(
                    std::toupper(
                        static_cast<unsigned char>(
                            character
                        )
                    )
                );


            if (isValidBase(base))
            {
                m_sequence.push_back(
                    base
                );
            }
        }
    }


    file.close();


    // ========================================================
    // Verificación
    // ========================================================

    if (m_sequence.empty())
    {
        std::cerr
            << "Error: el archivo no contiene una secuencia "
               "de ADN válida.\n";

        return false;
    }


    std::cout
        << "ADN cargado correctamente.\n"
        << "Longitud: "
        << m_sequence.size()
        << " bases\n";


    return true;
}


// ============================================================
// IS VALID BASE
// ============================================================

bool DNA::isValidBase(
    char base
) const
{
    return
        base == 'A' ||
        base == 'C' ||
        base == 'G' ||
        base == 'T';
}


// ============================================================
// IS LOADED
// ============================================================

bool DNA::isLoaded() const
{
    return m_loaded;
}


// ============================================================
// GET SEQUENCE
// ============================================================

const std::string& DNA::getSequence() const
{
    return m_sequence;
}


// ============================================================
// GET LENGTH
// ============================================================

std::size_t DNA::getLength() const
{
    return m_sequence.size();
}