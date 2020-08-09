

#include "po_stream.h"


Postream::Postream(std::ostream* io) {
    m_io = io;
    init();
}


void Postream::init() {

    align_to_byte();
}

void Postream::align_to_byte() {
    m_bits_left = 0;
    m_bits = 0;
}


Postream::Postream(std::string& data) {

}


uint32_t Postream::writeRawData(unsigned char * buf, uint32_t bufsize) {

    m_io->write((const char*)buf, bufsize);
    return bufsize;
}