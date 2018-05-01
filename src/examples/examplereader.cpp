#include "protobuf/ssl_referee.pb.h"
#include "protobuf/messages_robocup_ssl_wrapper.pb.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <stdint.h>
#include <endian.h>

struct FileHeader
{
    char name[12]; // SSL_LOG_FILE
    int32_t version; // Default file format is version 1
};

const char* DEFAULT_FILE_HEADER_NAME = "SSL_LOG_FILE";
const int32_t DEFAULT_FILE_VERSION = 1;

struct DataHeader
{
    int64_t timestamp; // Timestamp in ns
    int32_t messageType; // Message type
    int32_t messageSize; // Size of protobuf message in bytes
};

enum MessageType
{
    MESSAGE_BLANK = 0,
    MESSAGE_UNKNOWN = 1,
    MESSAGE_SSL_VISION_2010 = 2,
    MESSAGE_SSL_REFBOX_2013 = 3
};

int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cout << "No input file name specified!" << std::endl;
        std::cout << "Please run \"examplereader <log file>\"." << std::endl;

        return -1;
    }

    const char *filename = argv[1];

    std::ifstream in(filename, std::ios_base::in | std::ios_base::binary);

    if (!in.is_open()) {
        std::cerr << "Error opening log file \"" << filename << "\"!" << std::endl;
    }

    FileHeader fileHeader;
    in.read((char*) &fileHeader, sizeof(fileHeader));
    // Log data is stored big endian, convert to host byte order
    fileHeader.version = be32toh(fileHeader.version);

    if (strncmp(fileHeader.name, DEFAULT_FILE_HEADER_NAME, sizeof(fileHeader.name)) == 0) {
        std::cout << "File format version " << fileHeader.version << " detected." << std::endl;

        if (fileHeader.version == DEFAULT_FILE_VERSION) {
            unsigned long refereePackets = 0;
            unsigned long visionPackets = 0;

            while (!in.eof()) {
                DataHeader dataHeader;
                in.read((char*) &dataHeader, sizeof(dataHeader));
                // Log data is stored big endian, convert to host byte order
                dataHeader.timestamp = be64toh(dataHeader.timestamp);
                dataHeader.messageType = be32toh(dataHeader.messageType);
                dataHeader.messageSize = be32toh(dataHeader.messageSize);

                char* data = new char[dataHeader.messageSize];
                in.read(data, dataHeader.messageSize);

                if (dataHeader.messageType == MESSAGE_SSL_VISION_2010) {
                    SSL_WrapperPacket packet;
                    if (packet.ParseFromArray(data, dataHeader.messageSize)) {
                        visionPackets++;
                        std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(3) << (double) dataHeader.timestamp / 1e9
                                  << ": Read " << visionPackets << " vision packets and " << refereePackets << " referee packets!" << std::endl;
                    } else {
                        std::cerr << "Error parsing vision packet!" << std::endl;
                    }
                } else if (dataHeader.messageType == MESSAGE_SSL_REFBOX_2013) {
                    SSL_Referee packet;
                    if (packet.ParseFromArray(data, dataHeader.messageSize)) {
                        refereePackets++;
                        std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(3) << (double) dataHeader.timestamp / 1e9
                                  << ": Read " << visionPackets << " vision packets and " << refereePackets << " referee packets!" << std::endl;
                    } else {
                        std::cerr << "Error parsing vision packet!" << std::endl;
                    }
                }

                delete data;
            }
        }
    } else {
        std::cerr << "Error log file is unsupported or corrupted!" << std::endl;
    }

    return 0;
}
