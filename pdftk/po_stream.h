#ifndef PO_STREAM_H
#define PO_STREAM_H



#include <ostream>
#include <sstream>
#include <stdint.h>
#include <sys/types.h>

class Postream {
public:

    Postream(std::ostream* ostream);


    Postream(std::string& data);


    uint32_t writeRawData(unsigned char * buf, uint32_t bufsize);


private:
    std::ostream* m_io;
    std::ostringstream m_io_str;
    int m_bits_left;
    uint64_t m_bits;

    void init();

    void align_to_byte();


};


#endif
