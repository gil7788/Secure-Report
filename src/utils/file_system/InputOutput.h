//
// Created by gil on 16/01/20.
//

#ifndef SECURE_REPORT_INPUTOUTPUT_H
#define SECURE_REPORT_INPUTOUTPUT_H

#include <string>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

/**
 * Implementation of input/output API
 *
 * The API supports the following features:
 * 1. Console read/write
 * 2. Filesystem read/write
 * 3. Output level, used for debbuging
 */
class InputOutput {
    const bool _output_to_console;
    const fs::path _output_file_path;
    const int _output_level;
    bool _output_to_file;

public:
    /**
     * InputOutput Constructor
     *
     * @param output_to_console Console print flag
     * @param output_file_path File path of output file
     * @param output_level Permitted output level threshold
     */
    InputOutput(bool output_to_console, const std::string& output_file_path, int output_level);

    /**
     * InputOutput Constructor
     *
     * @param output_to_console Console print flag
     * @param output_file_path File path of output file
     * @param output_level Permitted output level threshold
     *
     * @overload
     */
    InputOutput(bool output_to_console, const fs::path& output_file_path, int output_level);

    /**
     * Output message, if provided Output level is permitted
     *
     * @param message Message to output
     * @param output_level Output level
     *
     * @return meassage output success
     */
    bool output(const std::string& message, int output_level);

    /**
     * Create file in path provided in counstructor, if it doesn't exist, and append message.
     * Output message, if provided output level is permitted
     *
     * @param message Message to output
     * @param output_level Output level
     *
     * @see InputOutput#InputOutput
     * @return File create and message write success
     */
    bool create_or_append_to_file(const std::string& message);

    /**
     * Read file in path provided in counstructor
     *
     * @see InputOutput#InputOutput
     */
    std::string read_file();

    static bool is_file_exist(const std::string file_path);
private:
    bool at_output_level(int output_level);
};


#endif //SECURE_REPORT_INPUTOUTPUT_H
